/**
  * \file lis_base_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS record.
  * \author Jose C.G. Gabadinho
  *
  * BaseRecord is where all other 'luaiocsup' record types derive.
  * The class factory should be use instead of constructors.
  * 
  * Sub-classes must implement their own constructors, to properly extract the link value
  * from the correct field (INP or OUT).
  * If the record type requires the linear convertion support method, the sub-class must
  * implement it.
  */

#ifndef INC_lis_base_record_H
#define INC_lis_base_record_H

#include <string>
#include <map>
#include <vector>
#include <exception>
#include <memory>

#include "lis_lua_state.h"
#include "lis_defs.h"

#include <dbCommon.h>
#include <epicsMutex.h>
#include <devSup.h>



/** Class wrapper for a 'luaiocsup' device-support record.
  */
class BaseRecord {
public:
    /** Class factory method to create a luaiocsup Lua record.
      * Instances are stored in a map (key is record type, value is a vector of unique_ptr<BaseRecord>.
      *
      * \param[in] precord Pointer to EPICS record structure
      * \param[in] force_load true to force reloading of file, even if master state has already loaded it for some other record
      *  
      * \return Raw pointer to created BaseRecord
      */
    static BaseRecord * CreateRecord(struct dbCommon *precord, bool force_file_load);

    /** Class factor method to remove a deleted record from the internal map (which will cause the destructor to be called).
      *  
      * \param[in] precord Pointer to EPICS record structure
      *  
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      */
    static long DeleteRecord(struct dbCommon *precord);

    /** Destructor of a BaseRecord instance.
      */
    virtual ~BaseRecord() {}

    /** Called when iocsh 'dbior' command is executed.
      *
      * Reports information for a device-support record type.
      *
      * \param[in] interest Set to 0 for 1-liner report, >0 for more detailed information
      * \param[in] rec_type Which 'luaiocsup' record type to report
      */
    static void ReportRecords(int interest, std::string rec_type);

    /** Called when the record is to be processed.
      *
      * \param[in] allowed_type Allowed type of Lua code to execute (function name, code chunk, both)
      *
      * \param[out] type_found Type of executed code (function name or code chunk)
      *
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      *         Exceptions: in records that use the linear convertion, ai and ao, if this process
      *                     function returns 0 then VAL is converted according to LINR field; to
      *                     prevent this, return value should be 2
      */
    virtual long Process(luaCodeType allowed_type, luaCodeType *found_type);

    /** Called for records that support the linear convertion hook, ai and ao, when one of LINR,
      * EGUF, EGUL, ROFF is modified.
      * This BaseRecord simply raises a logic_error exception.
      *
      * \param[in] after Set to 0 before field is updated, set to 1 after field is updated
      *
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      */
    virtual long SpecialLinConv(int after) { throw std::logic_error("Not implemented!"); }

    /** Returns which IOSCANPVT (interrupt source) is associated with the record
      * when its scan rate is set to I/O Intr.
      *
      * \param[in] cmd Set to 0 when SCAN is set to I/O Intr, set to 1 when SCAN is
      *                set to any other value
      *
      * \param[out] ppvt Updated IOSCANPVT structure
      *
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      */
    long GetIoIntInfo(int cmd, IOSCANPVT *ppvt);

    /** Called after adding the record and before the IOC starts running.
      *
      * Not called when INP/OUT fields are updated while IOC is running.
      *  
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      */
    long LegacyInit();

    /** Class method to set the configuration from the 'lisConfigure' registrar call.
      *  
      * \param[in] base_directory Directory where the lua scripts are located
      * \param[in] stack_size Minimum stack size of co-routine stacks
      * \param[in] debug_level Debug verbosity level 
      */
    static void SetConfiguration(std::string base_directory, int stack_size, int debug_level) {
        BaseDirectory = base_directory;
        DebugLevel = debug_level;
        MinStackSize = stack_size;
    }

protected:
    /** Constructor for a 'luaiocrec' record.
      * Parses INP/OUT field link value, creates Lua states, etc.
      *  
      * \param[in] precord Pointer to EPICS record structure
      * \param[in] rec_type Record type (ai, ao, luasub, etc.)
      * \param[in] rec_name Record name
      * \param[in] link_value Value of INP/OUT field, INST_IO type, with filename, luaiocsup parameters, and script parameters
      * \param[in] force_load true to force reloading of file, even if master state has already loaded it for some other record
      */
    BaseRecord(struct dbCommon *precord, std::string rec_type, std::string rec_name, std::string link_field, std::string link_value, bool force_load);

    /** Record type: type name + record name. */
    std::pair<std::string, std::string> recordType;

    /** Record configuration: field name + value. */
    std::pair<std::string, std::string> recordConfig;

    /** Pointer to EPICS record structure. */
    struct dbCommon *pRecord;

    /** Lua state for executing init, proc., etc. lua methods. */
    std::unique_ptr<LuaRuntime> syncProcRuntime;
    /** Lua state for executing asynchronous processing method/callback. */
    std::unique_ptr<LuaRuntime> asyncCallbackRuntime;

    /** Class method to parse the record's link field, contaning the user's configuration.
      * Retrieves the Lua script filename, state name/id, and Lua table name for support methods.
      *        * \param[in] precord Pointer to common record structure

      * \param[in] link_value Value of INP/OUT link record field.
      *
      * \param[out] filename Lua script filename, immediately after the '@' character
      * \param[out] state_id Lua master state name/id [optional] <id=name>
      * \param[out] table_name Table containing the support methods [optional] <table=name>
      * 
      * \return true if a filename was found, otherwise false
      */
    static bool ParseLinkField(std::string link_value, std::string& filename, std::string& state_id, std::string& table_name);

    /** Class variable: maps a 'luaiocsup' record type to a vector of BaseRecord instances. */
    static std::map<std::string, std::unique_ptr<std::vector<std::unique_ptr<BaseRecord>>>> LuaRecords;
    /** Class variable: mutex lock for the above map. */
    static epicsMutexId LuaRecordsLock;

    /** Class variable for configuration from registrar: directory where Lua script files are located. */
    static std::string BaseDirectory;
    /** Class variable for configuration from registrar: debug level verbosity. */
    static int DebugLevel;
    /** Class variable for configuration from registrar: co-routine minimum stack size. */
    static int MinStackSize;
};

#endif /* INC_lis_base_record_H */
