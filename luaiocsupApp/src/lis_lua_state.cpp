/**
  * \file lis_lua_state.cpp
  * \brief O.O. abstraction for a Lua state.
  * \author Jose C.G. Gabadinho
  */

#include "lis_lua_state.h"

#include "lis_log.h"



std::map<std::string, std::shared_ptr<LuaMaster>> LuaMaster::MasterStates;
epicsMutexId LuaMaster::MasterStatesLock = epicsMutexCreate();



lua_State * LuaState::lua_newthread(int& ref_regidx, int check_stack) {
    lua_State *L = ::lua_newthread(this->cLuaState);
    if (L == NULL) {
        throw std::bad_alloc();
    }
    ref_regidx = ::luaL_ref(this->cLuaState, LUA_REGISTRYINDEX);

    if (check_stack > 0) {
        if (!lua_checkstack(L, check_stack)) {
            //lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unable to grow stack to %d in lisCreateStack()\n",now_str,LIS_LIB_LOG_NAME,lisMinStackSize);
        }
    }

    return L;
}

void LuaState::luaL_unref(int state_reg_ref) {
    ::luaL_unref(this->cLuaState, LUA_REGISTRYINDEX, state_reg_ref);
}

int LuaState::luaL_dofile2(std::string filename) {
    return luaL_dofile(this->cLuaState, filename.c_str());
}



std::shared_ptr<LuaMaster> LuaMaster::GenerateMaster(std::string master_id, std::string directory, std::string filename, bool reload) {
    std::shared_ptr<LuaMaster> st = nullptr;
    
    epicsMutexLock(LuaMaster::MasterStatesLock);
    try {
        st = LuaMaster::MasterStates.at(master_id);
        epicsMutexUnlock(LuaMaster::MasterStatesLock);
        st->lock();
        st->loadFilename(directory, filename, reload);
    } catch (const std::out_of_range& e) {
        try {
            st = std::make_shared<LuaMaster>(master_id, directory, filename);
        } catch (const std::bad_alloc& e) {
            epicsMutexUnlock(LuaMaster::MasterStatesLock);
            throw e;
        }

        MasterStates[master_id] = st;
        epicsMutexUnlock(LuaMaster::MasterStatesLock);
    } catch (...) {
        epicsMutexUnlock(LuaMaster::MasterStatesLock);
        throw std::bad_alloc();
    }

    return st;
}

std::shared_ptr<LuaMaster> LuaMaster::GetMaster(std::string master_id) {
    std::shared_ptr<LuaMaster> st = nullptr;

    epicsMutexLock(LuaMaster::MasterStatesLock);
    try {
        st = LuaMaster::MasterStates.at(master_id);
    } catch (const std::out_of_range& e) {
        epicsMutexUnlock(LuaMaster::MasterStatesLock);
        throw e; 
    }

    epicsMutexUnlock(LuaMaster::MasterStatesLock);
    return st;
}

LuaMaster::LuaMaster(std::string master_id, std::string directory, std::string filename): masterId(master_id) {    
    this->lock();
    loadFilename(directory, filename, false);
}

bool LuaMaster::loadFilename(std::string directory, std::string filename, bool reload) {
    DECL_NOW_STR;
    bool loaded = false;
    std::string full_filename = directory + filename;
    const char *error_msg;

    auto already_loaded_it = loadedFiles.find(filename);
    bool not_loaded = (already_loaded_it == this->loadedFiles.end());

    if ((not_loaded) || (reload)) {
        if (not_loaded) {
            lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Loading %s in state id=%s\n", now_str, LIS_LIB_LOG_NAME, full_filename.c_str(), masterId.c_str());
        } else {
            lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Reloading %s in state id=%s\n", now_str, LIS_LIB_LOG_NAME, full_filename.c_str(), masterId.c_str());
        }
        
        if (luaL_dofile2(full_filename)) {
            error_msg = lua_tostring(this->cLuaState, -1);
            lisLog(LIS_LOGLVL_STANDARD, errlogMajor, now_str, "%s %s %s\n", now_str, LIS_LIB_LOG_NAME, error_msg);
            lua_pop(this->cLuaState, 1); /* Pop error message */
        } else {
            if (not_loaded) {
                this->loadedFiles.insert(filename);
            }
            loaded = true;
        }
        
    }
    
    return loaded;
}



LuaRuntime::LuaRuntime(std::string master_id, std::string directory, std::string filename, bool reload, int check_stack): LuaState(false) {
    this->lock();
    this->masterState = LuaMaster::GenerateMaster(master_id, directory, filename, reload);
    this->cLuaState = this->masterState->lua_newthread(this->stateRegRef, check_stack);
}

LuaRuntime::LuaRuntime(std::string master_id, int check_stack): LuaState(false) {
    this->lock();
    this->masterState = LuaMaster::GetMaster(master_id); // Can throw out_of_range here!
    this->cLuaState = this->masterState->lua_newthread(this->stateRegRef, check_stack);
}

LuaRuntime::~LuaRuntime() {
    if (this->cLuaState) {
        masterState->lock();
        masterState->luaL_unref(stateRegRef);
        this->cLuaState = NULL;
        masterState->unlock();
    }
}
