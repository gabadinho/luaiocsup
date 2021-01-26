#ifndef INC_lis_defs_H
#define INC_lis_defs_H

/* Record names */
#define LIS_AI_REC_NAME        "AI"
#define LIS_AO_REC__NAME       "AO"
#define LIS_BI_REC_NAME        "BI"
#define LIS_BO_REC_NAME        "BO"
#define LIS_MBBI_REC_NAME      "MBBI"
#define LIS_MBBO_REC_NAME      "MBBO"
#define LIS_STRINGIN_REC_NAME  "STRINGIN"
#define LIS_STRINGOUT_REC_NAME "STRINGOUT"
#define LIS_LONGIN_REC_NAME    "LONGIN"
#define LIS_LONGOUT_REC_NAME   "LONGOUT"
#define LIS_WAVEFORM_REC_NAME  "WAVEFORM"
#define LIS_LUASUB_REC_NAME    "LUASUB"

/* Record field names */
#define LIS_INP_FIELD_NAME   "INP"
#define LIS_OUT_FIELD_NAME   "OUT"
#define LIS_RECFLD_SEPARATOR "."

/* Keywords for INP/OUT arguments */
#define LIS_SCRIPT_PARAM_ID    "@id="
#define LIS_SCRIPT_PARAM_CLASS "@table="

/* Global table for all Lua IOC Support fields */
#define LIS_GLOBAL_TABLE_NAME "luaiocsup"

/* Global arg from INP/OUT field (and container) */
#define LIS_GLOBAL_ARG_NAME "arg"
#define LIS_ARGS_TABLE_NAME "_args"

/* Glocal current-record closure (and container) */
#define LIS_RECORD_SELF_NAME    "self"
#define LIS_CLOSURES_TABLE_NAME "_records"

#define LIS_LUASUCCESS_RET "LUASUCCESS"

/* Tables for definitions coming from EPICS fields */
#define LIS_DBMENU_TABLE_NAME    "DBMENU"
#define LIS_DBCHOICES_TABLE_NAME "CHOICES"
#define LIS_DBFIELD_TABLE_NAME   "FIELD"
#define LIS_DBDEVICE_TABLE_NAME  "DBDEVICE"

/* Field names for DBLINK table  */
#define LIS_DBLINK_TABLE_NAME      "DBLINK"
#define LIS_DBLINK_TABLE_FIELDTYPE "FIELDTYPE"
#define LIS_DBLINK_TABLE_LINKTYPE  "LINKTYPE"
#define LIS_DBLINK_TABLE_LINK      "LINK"
#define LIS_DBLINK_TABLE_LINKMASK  "LINKMASK"

/* Fields for DBF table  */
#define LIS_DBF_TABLE_NAME "DBF"

/* Fields for LNK table  */
#define LIS_LNK_TABLE_NAME "LNK"

/* Fields for LNK table  */
#define LIS_MSK_TABLE_NAME "MSK"
#define LIS_MSK_NMS        "NMS"
#define LIS_MSK_NMS_VAL    0x0
#define LIS_MSK_MS         "MS"
#define LIS_MSK_MS_VAL     0x1
#define LIS_MSK_MSI        "MSI"
#define LIS_MSK_MSI_VAL    0x2
#define LIS_MSK_MSS        "MSS"
#define LIS_MSK_MSS_VAL    0x3
#define LIS_MSK_PP         "PP"
#define LIS_MSK_PP_VAL     0x4
#define LIS_MSK_CA         "CA"
#define LIS_MSK_CA_VAL     0x8
#define LIS_MSK_CP         "CP"
#define LIS_MSK_CP_VAL     0x10
#define LIS_MSK_CPP        "CPP"
#define LIS_MSK_CPP_VAL    0x20
#define LIS_MSK_FWD        "FWD"
#define LIS_MSK_FWD_VAL    0x40
#define LIS_MSK_INPNAT     "INPNAT"
#define LIS_MSK_INPNAT_VAL 0x80
#define LIS_MSK_INPSTR     "INPSTR"
#define LIS_MSK_INPSTR_VAL 0x100
#define LIS_MSK_OUTNAT     "OUTNAT"
#define LIS_MSK_OUTNAT_VAL 0x200
#define LIS_MSK_OUTSTR     "OUTSTR"
#define LIS_MSK_OUTSTR_VAL 0x400
#define LIS_MSK_TSELTS     "TSELTS"
#define LIS_MSK_TSELTS_VAL 0x800
#define LIS_MSK_NPP        "NPP"

/* Device-support available functions (called from C; not all call Lua) */
#define LIS_DEVSUP_REPORT_FUN "report"
#define LIS_INIT_FUN          "init"
#define LIS_GET_IOINT_FUN     "get_ioint_info"
#define LIS_INIT_REC_FUN      "init_record"
#define LIS_WRITE_FUN         "write"
#define LIS_READ_FUN          "read"
#define LIS_LINCONV_FUN       "special_linconv"
#define LIS_ADD_REC_FUN       "add_record"
#define LIS_DEL_REC_FUN       "del_record"

/* Read/write record-specific (called from C) Lua functions */
#define LIS_AI_PROCESS_METHOD        "read_ai"
#define LIS_AO_PROCESS_METHOD        "write_ao"
#define LIS_BI_PROCESS_METHOD        "read_bi"
#define LIS_BO_PROCESS_METHOD        "write_bo"
#define LIS_MBBI_PROCESS_METHOD      "read_mbbi"
#define LIS_MBBO_PROCESS_METHOD      "write_mbbo"
#define LIS_STRINGIN_PROCESS_METHOD  "read_stringin"
#define LIS_STRINGOUT_PROCESS_METHOD "write_stringout"
#define LIS_LONGIN_PROCESS_METHOD    "read_longin"
#define LIS_LONGOUT_PROCESS_METHOD   "write_longout"
#define LIS_WAVEFORM_PROCESS_METHOD  "read_wf"

/* Logging Lua functions and related constants */
#define LIS_LOGMINOR_FUN "ioclog_minor"
#define LIS_LOGMAJOR_FUN "ioclog_major"
#define LIS_LOGFATAL_FUN "ioclog_fatal"
#define LIS_LOGINFO_FUN  "ioclog_info"
#define LIS_LOG_FUN      "ioclog"

#define LIS_LOGSEV_TABLE_NAME "LOGSEV"
#define LIS_LOGSEV_INFO       "INFO"
#define LIS_LOGSEV_MINOR      "MINOR"
#define LIS_LOGSEV_MAJOR      "MAJOR"
#define LIS_LOGSEV_FATAL      "FATAL"

/* Lua IOC Support functions not-specific to Lua records */
#define LIS_SCANIOINI_FUN "scanio_init"
#define LIS_SCANIOREQ_FUN "scanio_request"
#define LIS_POSTEVENT_FUN "post_event"
#define LIS_FINDREQ_FUN   "find_record"

/* IOC-related functions*/
#define LIS_IOCRUNNING_FUN "ioc_running"
#define LIS_ATEXIT_FUN     "at_exit"

/* Lua IOC Support for worker-threads */
#define LIS_WORKERTHREADBIND_FUN "bind_luaiocsup"

/* Other functions */
#define LIS_MASK2STRING_FUN "to_string" /* Field of LIS_MSK_TABLE_NAME */

/* Record-closure Lua functions */
#define LIS_RECNAME_FUN      "record_name"
#define LIS_PACT_FUN         "pact"
#define LIS_NORD_FUN         "nord"
#define LIS_RETRIEVEFLD_FUN  "retrieve_field"
/*#define LIS_RETRIEVEFLDS_FUN "retrieve_fields"*/
#define LIS_UPDATEFLD_FUN    "update_field"
/*#define LIS_UPDATEFLDS_FUN   "update_fields"*/
#define LIS_METATABLE_GET    "__index"
#define LIS_METATABLE_SET    "__newindex"
#define LIS_SCANONCE_FUN     "scan_once"
#define LIS_PROCESSREQ_FUN   "process"
#define LIS_SCANLOCK_FUN     "scan_lock"
#define LIS_SCANUNLOCK_FUN   "scan_unlock"

#define LIS_ALLOW_FUN_OR_CHK    0
#define LIS_ALLOW_FUNCTION_ONLY 1
#define LIS_ALLOW_CHUNK_ONLY    2

#define LIS_CODTYP_UNKNOWN  0
#define LIS_CODTYP_ERROR    1
#define LIS_CODTYP_FUNCTION 2
#define LIS_CODTYP_CHUNK    3

#define LIS_LUATABLE_INDEX_START 1

/*
#define LIS_SCANIO_ARGS_MUTEXTRIGGER 1
*/

#define LIS_ALLOW_DEBUG_CODE 1

#endif /* INC_lis_defs_H */
