/**
  * \file lis_lua_state.h
  * \brief O.O. abstraction for a Lua state.
  * \author Jose C.G. Gabadinho
  *
  * Defines the different perspectives of Lua states from the Lua IOC support point-of-view.
  * 
  * LuaMaster is the main state where scripts are loaded and global variables are defined.
  *
  * LuaRuntime represents a co-routine stack, created from a LuaMaster instance, where code
  * is executed by pushing it into the stack.
  * 
  * Both classes derive from LuaState, which holds the actual Lua state structure, and implements
  * wrappers over the standard-C Lua C-bindings.
  */

#ifndef INC_lis_lua_state_H
#define INC_lis_lua_state_H

#include <memory>
#include <vector>
#include <set>
#include <map>

#include <epicsMutex.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}



/** Class wrapper for a Lua state.
  * Provides an EPICS mutex lock for thread-synchronization.
  */
class LuaState {
public:
    /** Constructor for a LuaState instance: creates a mutex lock and a Lua state if necessary.
      * A bad_alloc exception is thrown if memory allocation fails.
      *  
      * \param[in] create_state If true (default) then a Lua state is created and assigned to cLuaState member
      */
    LuaState(bool create_state = true) {
        this->stateLock = ::epicsMutexCreate(); if (this->stateLock == NULL) throw std::bad_alloc();
        if (create_state) {
            this->cLuaState = ::luaL_newstate();
            if (this->cLuaState == NULL) throw std::bad_alloc();
        }
    }

    /** Destructor of a LuaState instance.
      * Destroys the mutex lock and closes any existing Lua state.
      */
    virtual ~LuaState() {
        ::epicsMutexDestroy(this->stateLock);
        if (this->cLuaState) ::lua_close(this->cLuaState);
    }

    /** Locks the mutex associated with this state.
      *
      * \return Status of the lock (enum: ok, timeout, error)
      */
    epicsMutexLockStatus lock() { return ::epicsMutexLock(this->stateLock); }

    /** Unlocks the mutex associated with this state.
      */
    void unlock() { ::epicsMutexUnlock(this->stateLock); }

    /** Wrappers for the standard-C Lua functions, associated with this specific state.
      */
    lua_State * lua_newthread(int& ref_regidx, int check_stack);
    int luaL_ref(int thread_ref);
    void luaL_unref(int thread_ref);
    int luaL_dofile2(std::string filename);

protected:
    /** Structure representing a Lua state, from the Lua library. */
    lua_State *cLuaState;
    /** Mutex lock, from EPICS. */
    epicsMutexId stateLock;
};



/** Master state: where Lua script files are loaded, and global variables are defined.
  *
  * Usage: do not use constructor directly! Use instead one of the factory methods: GenerateMaster or GetMaster.
  */
class LuaMaster: public LuaState {
public:
    /** Class factory method for the LuaMaster class.
      * Either creates a new state, or returns the already existing state with the given id.
      * A bad_alloc exception is thrown if memory allocation fails.
      *  
      * \param[in] master_id Unique state id name
      * \param[in] directory Directory where Lua script files are located
      * \param[in] filename Lua script filename to be loaded into the state
      * \param[in] reload Force reload of Lua script even if it was loaded already into this state
      * 
      * \return Pointer to state
      */
    static std::shared_ptr<LuaMaster> GenerateMaster(std::string master_id, std::string directory, std::string filename, bool reload);

    /** Class factory method for the LuaMaster class.
      * A bad_alloc exception is thrown if memory allocation fails.
      * A out_of_range exception is thrown if master_id wasn't generated beforehand.
      *  
      * \param[in] master_id Unique state id name
      * 
      * \return Pointer to state
      */
    static std::shared_ptr<LuaMaster> GetMaster(std::string master_id);

    /** Constructor for a LuaMaster instance. Not to be used directly!
      * A bad_alloc exception is thrown if memory allocation fails.
      *  
      * \param[in] master_id Unique state id name
      * \param[in] directory Directory where Lua script files are located
      * \param[in] filename Lua script filename to be loaded into the state
      */
    LuaMaster(std::string master_id, std::string directory, std::string filename);

protected:
    /** Loads a Lua script file into this state.
      *
      * \param[in] directory Directory where Lua script files are located
      * \param[in] filename Lua script filename to be loaded into the state
      * \param[in] reload Force reload of Lua script even if it was loaded already into this state
      *
      * \return true if file was loaded, false otherwise
      */
    bool loadFilename(std::string directory, std::string filename, bool reload);

    /** Name/id of this master state; must be unique. */
    std::string masterId;
    /** Lua script files loaded into this state. */
    std::set<std::string> loadedFiles;

    /** Class variable: maps the state name/id to a LuaMaster instance. */
    static std::map<std::string, std::shared_ptr<LuaMaster>> MasterStates;
    /** Class variable: mutex lock for the above map. */
    static epicsMutexId MasterStatesLock;
};



/** Runtime state: where Lua code is executed.
  */
class LuaRuntime: public LuaState {
public:
    /** Class factory method for the LuaRuntime class.
      * A bad_alloc exception is thrown if memory allocation fails.
      *  
      * \param[in] master_id Unique master state id, associated with this co-routine stack
      * \param[in] directory Directory where Lua script files are located
      * \param[in] filename Lua script filename to be loaded into the state
      * \param[in] reload Force reload of Lua script even if it was loaded already into this state
      * \param[in] check_stack Minimum stack size
      * 
      * \return Pointer to state
      */
    LuaRuntime(std::string master_id, std::string directory, std::string filename, bool reload, int check_stack);

    /** Class factory method for the LuaRuntime class.
      * A bad_alloc exception is thrown if memory allocation fails.
      * A out_of_range exception is thrown if master_id wasn't generated beforehand.
      *  
      * \param[in] master_id Unique master state id, associated with this co-routine stack
      * \param[in] check_stack Minimum stack size
      * 
      * \return Pointer to state
      */
    LuaRuntime(std::string master_id, int check_stack);

    /** Destructor of a LuaRuntime instance.
      * De-references the Lua state from its master.
      */
    ~LuaRuntime();

    /** Locks the mutex associated with the master then with this state.
      */
    void lockWithMaster() { masterState->lock(); this->lock(); }

    /** Unlocks the mutex associated with this state then with its master.
      */
    void unlockWithMaster() { this->unlock(); this->masterState->unlock(); }

protected:
    /** Master of this Lua state. */
    std::shared_ptr<LuaState> masterState;
    /** Register-reference of this state in its master, to avoid being garbage-collected. */
    int stateRegRef;
};

#endif /* INC_lis_lua_state_H */
