#include "lis_lua.h"

#include "lis_globals.h"
#include "lis_log.h"
#include "lis_epics.h"
#include "lis_data_cast.h"
#include "lis_luasubrec.h"

#include <stdlib.h>
#include <string.h>

#include <dbScan.h>
#include <dbLock.h>
#include <waveformRecord.h>

/* Fields for DBF table  */
static const char* LIS_DBF_TABLE_FIELDS[] = {
  "DBF_STRING",
  "DBF_CHAR",
  "DBF_UCHAR",
  "DBF_SHORT",
  "DBF_USHORT",
  "DBF_LONG",
  "DBF_ULONG",
  "DBF_FLOAT",
  "DBF_DOUBLE",
  "DBF_ENUM",
  "DBF_MENU",
  "DBF_DEVICE",
  "DBF_INLINK",
  "DBF_OUTLINK",
  "DBF_FWDLINK",
  "DBF_NOACCESS" };

/* Fields for LNK table  */
static const char* LIS_LNK_TABLE_FIELDS[] = {
  "CONSTANT",
  "PV_LINK",
  "VME_IO",
  "CAMAC_IO",
  "AB_IO",
  "GPIB_IO",
  "BITBUS_IO",
  "MACRO_LINK",
  NULL,
  NULL,
  "DB_LINK",
  "CA_LINK",
  "INST_IO",
  "BBGPIB_IO",
  "RF_IO",
  "VXI_IO" };

static int lisIocRunning(lua_State *L) {
  lua_pushboolean(L,lisIsIocRunning!=0);
  return 1; /* 1 parameter returned to Lua */
}

/* The following function can only be called while EPICS initialization is in phase 0 */
static int lisScanIoInit(lua_State *L) {
  NOW_STR;
  struct lisIOIntrList *iointr=NULL;
  int new_ioscan;

  if ((lua_gettop(L)==1)&&(lua_isstring(L,1))) {
    char *s=strdup(lua_tostring(L,1));
    if (s) {
      iointr=lisFetchIoIntr(s,&new_ioscan);
      if ((iointr!=NULL)&&(new_ioscan)) {
        scanIoInit(iointr->ioScan);
      } else if (iointr==NULL) {
        free(s);
      }
    } else {
      lisLogSimple("%s Failed to malloc() inside lisScanIoInit()\n",LIS_LIB_LOG_NAME);
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling scanio_init(): accepts only single <string> argument\n",now_str,LIS_LIB_LOG_NAME);
  }

  lua_pushboolean(L,iointr!=NULL);
  return 1; /* 1 parameter returned to Lua */
}

static int lisScanIoRequest(lua_State *L) {
  NOW_STR;
  struct lisIOIntrList *iointr=NULL;
  const char *ioscan_id;
  struct lisRecordList *rec_l;
  struct lisRecordState *rec_st;
  int stack_top,i,args_ok=TRUE,lock_master_state=FALSE,has_args=FALSE;

  stack_top=lua_gettop(L);
  if ((stack_top>0)&&(lua_isstring(L,1))) {
    has_args=(stack_top>1);
    for (i=2; i<=stack_top; i++) {
      if ((lua_isnil(L,i))||(lua_isboolean(L,i))||(lua_isinteger(L,i))||(lua_isnumber(L,i))||(lua_islightuserdata(L,i))) {
	  } else if (lua_isstring(L,i)) {
        lock_master_state=TRUE;
	  } else {
        args_ok=FALSE;
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling scanio_request(): accepts only pass-by-value arguments\n",now_str,LIS_LIB_LOG_NAME);
        break;
	  }
	}

    if (args_ok) {
      ioscan_id=lua_tostring(L,1);
      iointr=lisGetIoIntr(ioscan_id);
      if (iointr) {
        if (has_args) {
          rec_l=iointr->records;
          while (rec_l!=NULL) {
            rec_st=rec_l->recordState;
          
            if (lock_master_state) {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Locking %s state- and stack-mutexes for arguments-exchange\n",now_str,LIS_LIB_LOG_NAME,rec_st->recordName);
              lisMutexLock2(rec_st->lisState->stateLock,rec_st->recordRuntime->stackLock);
            } else {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Locking %s stack-mutex for arguments-exchange\n",now_str,LIS_LIB_LOG_NAME,rec_st->recordName);
              lisMutexLock(rec_st->recordRuntime->stackLock);
            }

            lisPushArgsExchange(L,rec_st->recordRuntime->coroutStack,"worker-thread",rec_st->recordName,2,stack_top);

#ifndef LIS_SCANIO_ARGS_MUTEXTRIGGER
            if (lock_master_state) {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unlocking %s state- and stack-mutexes\n",now_str,LIS_LIB_LOG_NAME,rec_st->recordName);
              lisMutexUnlock2(rec_st->lisState->stateLock,rec_st->recordRuntime->stackLock);
            } else {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unlocking %s stack-mutex\n",now_str,LIS_LIB_LOG_NAME,rec_st->recordName);
              lisMutexUnlock(rec_st->recordRuntime->stackLock);
            }
#endif

            rec_l=rec_l->next;
          }
        }

        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Triggering I/O event %s\n",now_str,LIS_LIB_LOG_NAME,ioscan_id);
        scanIoRequest(*(iointr->ioScan));

#ifdef LIS_SCANIO_ARGS_MUTEXTRIGGER
        if (has_args) {
          rec_l=iointr->records;
          while (rec_l!=NULL) {
            if (lock_master_state) {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unlocking %s state- and stack-mutexes\n",now_str,LIS_LIB_LOG_NAME,rec_l->recordState->recordName);
              lisMutexUnlock2(rec_l->recordState->lisState->stateLock,rec_l->recordState->recordRuntime->stackLock);
            } else {
              lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unlocking %s stack-mutex\n",now_str,LIS_LIB_LOG_NAME,rec_l->recordState->recordName);
              lisMutexUnlock(rec_l->recordState->recordRuntime->stackLock);
            }
            rec_l=rec_l->next;
          }
        }
#endif
        
	  }
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling scanio_request(): missing I/O scan id argument\n",now_str,LIS_LIB_LOG_NAME);
  }

  lua_pushboolean(L,iointr!=NULL);
  return 1; /* 1 parameter returned to Lua */
}

static int lisAtExit(lua_State *L) {
  NOW_STR;
  int stack_top,args_ok=FALSE;
  struct lisAtExitList *at_exit;

  stack_top=lua_gettop(L);
  if ((stack_top>0)&&(lua_isfunction(L,1))) {
    at_exit=lisCreateAtExit();
    if (at_exit) {
      at_exit->atExitStack=lisCreateStack(L,&(at_exit->atExitStackReference));
      if (at_exit->atExitStack) {
        lua_xmove(L,at_exit->atExitStack,stack_top);
        args_ok=TRUE;
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Registered at-exit callback %p function with %d arguments\n",now_str,LIS_LIB_LOG_NAME,lua_topointer(at_exit->atExitStack,1),stack_top-1);
      }
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisAtExit(): first argument must be <function>\n",now_str,LIS_LIB_LOG_NAME);
  }

  lua_pushboolean(L,args_ok);
  return 1; /* 1 parameters returned to Lua */
}

static int lisFindRecord(lua_State *L) {
  NOW_STR;
  int pushed=0;
  const char *rec_name;

  if (!lua_isstring(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong type for finding record (record name must be string)! \n",now_str,LIS_LIB_LOG_NAME);
    lua_pushnil(L);
    pushed=1;
  } else {
    rec_name=lua_tostring(L,-1);  
    pushed=lisPushStdRecordClosure(L,rec_name);
    if (pushed==0) {
      lua_pushnil(L);
      pushed=1;
	}
  }

  return pushed; /* #Parameters returned to Lua */
}

static int lisRetrieveMyField(lua_State *L) {
  struct dbCommon *prec;
  NOW_STR;
  const char *field_name;
  char *rec_name;
  int pushed=0;
  dbAddr db_addr;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  rec_name=prec->name;

  if (!lua_isstring(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong type for retrieving %s field name (must be string)! \n",now_str,LIS_LIB_LOG_NAME,rec_name);
    lua_pushnil(L);
    pushed=1;
  } else {
    field_name=lua_tostring(L,-1);  

    if (lisGetField(rec_name,field_name,&db_addr)!=NULL) {
      pushed=lisRetrieveFieldToStackDbGet(L,&db_addr,NULL);
      if (pushed<0) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisRetrieveMyField(): unable to get values from field %s.%s\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
        lua_pushnil(L);
        pushed=1;
      }
    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisRetrieveMyField(): field %s.%s not found\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
      lua_pushnil(L);
      pushed=1;
    }
  }

  return pushed; /* #Parameters returned to Lua */
}

static int lisUpdateMyField(lua_State *L) {
  struct dbCommon *prec;
  NOW_STR;
  const char *field_name;
  char *rec_name;
  dbAddr db_addr;
  int updated=0;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  rec_name=prec->name;

  if (!lua_isstring(L,-2)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong type for updating %s field name (must be string)! \n",now_str,LIS_LIB_LOG_NAME,rec_name);
  } else {
    field_name=lua_tostring(L,-2);

    if (lisGetField(rec_name,field_name,&db_addr)!=NULL) {
      if (lua_isinteger(L,-1)) {
        updated=lisUpdateFieldFromIntegerDbPut(L,&db_addr);

      } else if (lua_isnumber(L,-1)) {
        updated=lisUpdateFieldFromNumberDbPut(L,&db_addr);

      } else if (lua_isstring(L,-1)) {
        updated=lisUpdateFieldFromStringDbPut(L,&db_addr);

      } else if (lua_istable(L,-1)) {
        updated=lisUpdateFieldFromTableDbPut(L,&db_addr,NULL);

      } else {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unknown lua value type for field %s.%s\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
      }

    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisUpdateMyField(): field %s.%s not found\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
    }
  }

  lua_pushboolean(L,updated>0);
  return 1; /* 1 parameter returned to Lua */
}

static int lisMyRecordName(lua_State *L) {
  struct dbCommon *prec;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  lua_pushstring(L,prec->name);

  return 1; /* 1 parameter returned to Lua */
}

static int lisScriptLog(lua_State *L) {
  NOW_STR;
  char *string_buffer;
  int i,t,level;

  if (lisDebugLevel>LIS_LOGLVL_NONE) {
    lisGenLogTime(now_str,sizeof(now_str));

    if (lua_gettop(L)>0) {
      if (lua_isnumber(L,1)) {
        level=lua_tonumber(L,1);
        lisGenLogTime(now_str,sizeof(now_str));

        t=lua_gettop(L);
        lisAllocString(&string_buffer);
        for (i=2; i<=t; i++) {
          const char *log_line=lua_tostring(L,i);
          lisAppendString(string_buffer,log_line);
          if (i!=t) {
            lisAppendString(string_buffer," ");
          }
        }
        errlogSevPrintf(level,"%s %s %s\n",now_str,LIS_SCRIPT_LOG_NAME,string_buffer);
        lisDiscardString(string_buffer);

	  } else {
        errlogSevPrintf(errlogMajor,"%s %s Error calling %s(): first argument <level> must be a number!\n",now_str,LIS_LIB_LOG_NAME,LIS_LOG_FUN);
      }
    } else {
      errlogSevPrintf(errlogMajor,"%s %s Error calling %s(): missing first argument <level>!\n",now_str,LIS_LIB_LOG_NAME,LIS_LOG_FUN);
    }
  }

  return 0; /* Nothing returned to Lua */
}

static int lisScriptLogInfo(lua_State *L) {
  NOW_STR;
  char *string_buffer;
  int i,t;

  if (lisDebugLevel>LIS_LOGLVL_NONE) {
    lisGenLogTime(now_str,sizeof(now_str));

    t=lua_gettop(L);
    lisAllocString(&string_buffer);
    for (i=1; i<=t; i++) {
      const char *log_line=lua_tostring(L,i);
      lisAppendString(string_buffer,log_line);
      if (i!=t) {
        lisAppendString(string_buffer," ");
	  }
	}
	errlogSevPrintf(errlogInfo,"%s %s %s\n",now_str,LIS_SCRIPT_LOG_NAME,string_buffer);
	lisDiscardString(string_buffer);
  }

  return 0; /* Nothing returned to Lua */
}

static int lisScriptLogMinor(lua_State *L) {
  NOW_STR;
  char *string_buffer;
  int i,t;

  if (lisDebugLevel>LIS_LOGLVL_NONE) {
    lisGenLogTime(now_str,sizeof(now_str));

    t=lua_gettop(L);
    lisAllocString(&string_buffer);
    for (i=1; i<=t; i++) {
      const char *log_line=lua_tostring(L,i);
      lisAppendString(string_buffer,log_line);
      if (i!=t) {
        lisAppendString(string_buffer," ");
	  }
	}
	errlogSevPrintf(errlogMinor,"%s %s %s\n",now_str,LIS_SCRIPT_LOG_NAME,string_buffer);
	lisDiscardString(string_buffer);
  }

  return 0; /* Nothing returned to Lua */
}

static int lisScriptLogMajor(lua_State *L) {
  NOW_STR;
  char *string_buffer;
  int i,t;

  if (lisDebugLevel>LIS_LOGLVL_NONE) {
    lisGenLogTime(now_str,sizeof(now_str));

    t=lua_gettop(L);
    lisAllocString(&string_buffer);
    for (i=1; i<=t; i++) {
      const char *log_line=lua_tostring(L,i);
      lisAppendString(string_buffer,log_line);
      if (i!=t) {
        lisAppendString(string_buffer," ");
	  }
	}
	errlogSevPrintf(errlogMajor,"%s %s %s\n",now_str,LIS_SCRIPT_LOG_NAME,string_buffer);
	lisDiscardString(string_buffer);
  }

  return 0; /* Nothing returned to Lua */
}

static int lisScriptLogFatal(lua_State *L) {
  NOW_STR;
  char *string_buffer;
  int i,t;

  if (lisDebugLevel>LIS_LOGLVL_NONE) {
    lisGenLogTime(now_str,sizeof(now_str));

    t=lua_gettop(L);
    lisAllocString(&string_buffer);
    for (i=1; i<=t; i++) {
      const char *log_line=lua_tostring(L,i);
      lisAppendString(string_buffer,log_line);
      if (i!=t) {
        lisAppendString(string_buffer," ");
	  }
	}
	errlogSevPrintf(errlogFatal,"%s %s %s\n",now_str,LIS_SCRIPT_LOG_NAME,string_buffer);
	lisDiscardString(string_buffer);
  }

  return 0; /* Nothing returned to Lua */
}

static int lisDummyScanIoInit(lua_State *L) {
  NOW_STR;
  lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Called 'dummy' version of %s, real version must be called during init phase 0!\n",now_str,LIS_LIB_LOG_NAME,LIS_SCANIOINI_FUN);
  lua_pushnil(L);
  return 1; /* 1 parameter returned to Lua */
}

static int lisMyPact(lua_State *L) {
  struct dbCommon *prec;
  NOW_STR;
  int top,set_pact,get_pact;

  top=lua_gettop(L);
  if ((top>1) || ((top==1)&&(!lua_isinteger(L,1)))) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling pact(): accepts only optional <integer> argument\n",now_str,LIS_LIB_LOG_NAME);
  } else {
    prec=lua_touserdata(L,lua_upvalueindex(1));
    if (top) {
      set_pact=lua_tointeger(L,1);
      lua_pop(L,1);
	}
	get_pact=prec->pact;
    lua_pushinteger(L,get_pact);
    if (top) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Setting %s.PACT to %d (was %d)\n",now_str,LIS_LIB_LOG_NAME,prec->name,set_pact,get_pact);
      prec->pact=set_pact;
    } else {
      lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Returning %s.PACT as %d\n",now_str,LIS_LIB_LOG_NAME,prec->name,get_pact);
	}
  }

  return 1; /* 1 parameter returned to Lua */
}

static int lisMyNord(lua_State *L) {
  struct dbCommon *prec;
  NOW_STR;
  int top;
  unsigned set_nord,get_nord;
  waveformRecord *wf_rec;

  top=lua_gettop(L);
  if ((top>1) || ((top==1)&&(!lua_isinteger(L,1)))) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling nord(): accepts only optional <integer> argument\n",now_str,LIS_LIB_LOG_NAME);
  } else {
    prec=lua_touserdata(L,lua_upvalueindex(1));
    wf_rec=(waveformRecord *)prec;
    if (top) {
      set_nord=lua_tointeger(L,1);
      lua_pop(L,1);
	}
	get_nord=wf_rec->nord;
    lua_pushinteger(L,get_nord);
    if (top) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Setting %s.NORD to %u (was %d)\n",now_str,LIS_LIB_LOG_NAME,prec->name,set_nord,get_nord);
      wf_rec->nord=set_nord;
    } else {
      lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Returning %s.NORD as %d\n",now_str,LIS_LIB_LOG_NAME,prec->name,get_nord);
	}
  }

  return 1; /* 1 parameter returned to Lua */
}

static int lisScanOnce(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Scan-once request for record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
  scanOnce(prec);

  return 0; /* Nothing returned to Lua */
}

static int lisDbProcess(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Processing of record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
  dbProcess(prec);

  return 0; /* Nothing returned to Lua */
}

static int lisScanLock(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Locking record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
  dbScanLock(prec);

  return 0; /* Nothing returned to Lua */
}

static int lisScanUnlock(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unlocking record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
  dbScanUnlock(prec);

  return 0; /* Nothing returned to Lua */
}

static int lisPostEvent(lua_State *L) {
  NOW_STR;
  short ev_number;

  if ((lua_gettop(L)==1)&&(lua_isinteger(L,1))) {
    ev_number=lua_tointeger(L,1);
    post_event(ev_number);
    lua_pushboolean(L,1);
  } else {
    lua_pushboolean(L,0);
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisPostEvent(): accepts only single <integer> argument\n",now_str,LIS_LIB_LOG_NAME);
  }

  return 1; /* 1 parameter returned to Lua */
}

static int lisDoWorkerThreadBindings(lua_State *L) {
  NOW_STR;

  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Binding Lua IOC Support for external worker-thread\n",now_str,LIS_LIB_LOG_NAME);

    lua_createtable(L,0,14);
    lua_setglobal(L,LIS_GLOBAL_TABLE_NAME);
    lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);

    lua_pushstring(L,LIS_LUASUCCESS_RET);
    lua_pushinteger(L,0);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_IOCRUNNING_FUN);
    lua_pushcfunction(L,lisIocRunning);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_SCANIOREQ_FUN);
    lua_pushcfunction(L,lisScanIoRequest);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGINFO_FUN);
    lua_pushcfunction(L,lisScriptLogInfo);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGMINOR_FUN);
    lua_pushcfunction(L,lisScriptLogMinor);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGMAJOR_FUN);
    lua_pushcfunction(L,lisScriptLogMajor);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGFATAL_FUN);
    lua_pushcfunction(L,lisScriptLogFatal);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOG_FUN);
    lua_pushcfunction(L,lisScriptLog);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_ATEXIT_FUN);
    lua_pushcfunction(L,lisAtExit);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_POSTEVENT_FUN);
    lua_pushcfunction(L,lisPostEvent);
    lua_rawset(L,-3);

    lua_pushliteral(L,LIS_LOGSEV_TABLE_NAME);
    lua_createtable(L,0,4);

    lua_pushstring(L,LIS_LOGSEV_INFO);
    lua_pushinteger(L,0);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGSEV_MINOR);
    lua_pushinteger(L,1);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGSEV_MAJOR);
    lua_pushinteger(L,2);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGSEV_FATAL);
    lua_pushinteger(L,3);
    lua_rawset(L,-3);

    lua_rawset(L,-3); /* Set table LOGSEV as field of LIS_GLOBAL_TABLE_NAME */
    lua_pop(L,1); /* Pop LIS_GLOBAL_TABLE_NAME */

    lua_pushboolean(L,1);
  } else {
    lua_pop(L,1);
    lua_pushboolean(L,0);
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global %s already exists!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
  }

  return 1; /* 1 parameter returned to Lua */
}

static int lisMaskToString(lua_State *L) {
  NOW_STR;
  char mask_str[9]=""; /* Beware: hardwired string buffer length! */
  unsigned mask;

  if ((lua_gettop(L)==1)&&(lua_isinteger(L,1))) {
    mask=lua_tointeger(L,1);

	if (mask & LIS_MSK_PP_VAL) strcpy(mask_str,LIS_MSK_PP);
	else if (mask & LIS_MSK_CA_VAL) strcpy(mask_str,LIS_MSK_CA);
	else if (mask & LIS_MSK_CP_VAL) strcpy(mask_str,LIS_MSK_CP);
	else if (mask & LIS_MSK_CPP_VAL) strcpy(mask_str,LIS_MSK_CPP);
	else strcpy(mask_str,LIS_MSK_NPP);
    strcat(mask_str," ");

	if (mask & LIS_MSK_MSS_VAL) strcat(mask_str,LIS_MSK_MSS);
	else if (mask & LIS_MSK_MSI_VAL) strcat(mask_str,LIS_MSK_MSI);
	else if (mask & LIS_MSK_MS_VAL) strcat(mask_str,LIS_MSK_MS);
	else strcat(mask_str,LIS_MSK_NMS);

    lua_pushstring(L,mask_str);
  } else {
    lua_pushnil(L);
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisMaskToString(): accepts only single <integer> argument\n",now_str,LIS_LIB_LOG_NAME);
  }

  return 1; /* 1 parameter returned to Lua */
}

lua_State * lisCreateLuaState() {
  return luaL_newstate();
}

/* Returns: FALSE=error TRUE=ok */
int lisInitializeLuaState(lua_State *L) {
  NOW_STR;
  int i,res=FALSE;

  /* Create LUAIOCSUP */
  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (lua_isnil(L,-1)) {
    luaL_openlibs(L);

    lua_pop(L,1);
    lua_createtable(L,0,15);
    lua_setglobal(L,LIS_GLOBAL_TABLE_NAME);
    lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);

    lua_pushstring(L,LIS_CLOSURES_TABLE_NAME);
    lua_createtable(L,0,1); /* Create _records table */
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_ARGS_TABLE_NAME);
    lua_createtable(L,0,1); /* Create _args table */
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LUASUCCESS_RET);
    lua_pushinteger(L,0);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_IOCRUNNING_FUN);
    lua_pushcfunction(L,lisIocRunning);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_SCANIOINI_FUN);
    lua_pushcfunction(L,lisScanIoInit);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGINFO_FUN);
    lua_pushcfunction(L,lisScriptLogInfo);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGMINOR_FUN);
    lua_pushcfunction(L,lisScriptLogMinor);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGMAJOR_FUN);
    lua_pushcfunction(L,lisScriptLogMajor);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOGFATAL_FUN);
    lua_pushcfunction(L,lisScriptLogFatal);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_LOG_FUN);
    lua_pushcfunction(L,lisScriptLog);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_ATEXIT_FUN);
    lua_pushcfunction(L,lisAtExit);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_POSTEVENT_FUN);
    lua_pushcfunction(L,lisPostEvent);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_WORKERTHREADBIND_FUN);
    lua_pushcfunction(L,lisDoWorkerThreadBindings);
    lua_rawset(L,-3);

    lua_pushliteral(L,LIS_LOGSEV_TABLE_NAME); /* Table for severity levels */
    lua_createtable(L,0,4);

    lua_pushstring(L,LIS_LOGSEV_INFO);
    lua_pushinteger(L,0);
    lua_rawset(L,-3); 

    lua_pushstring(L,LIS_LOGSEV_MINOR);
    lua_pushinteger(L,1);
    lua_rawset(L,-3); 

    lua_pushstring(L,LIS_LOGSEV_MAJOR);
    lua_pushinteger(L,2);
    lua_rawset(L,-3); 

    lua_pushstring(L,LIS_LOGSEV_FATAL);
    lua_pushinteger(L,3);
    lua_rawset(L,-3); 

    lua_rawset(L,-3); /* Set table LOGSEV as field of LIS_GLOBAL_TABLE_NAME */

    lua_pushliteral(L,LIS_DBLINK_TABLE_NAME); /* Table for DBLINK table fields */
    lua_createtable(L,0,4);

    lua_pushstring(L,LIS_DBLINK_TABLE_FIELDTYPE);
    lua_pushstring(L,LIS_DBLINK_TABLE_FIELDTYPE);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINKTYPE);
    lua_pushstring(L,LIS_DBLINK_TABLE_LINKTYPE);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINK);
    lua_pushstring(L,LIS_DBLINK_TABLE_LINK);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINKMASK);
    lua_pushstring(L,LIS_DBLINK_TABLE_LINKMASK);
    lua_rawset(L,-3);

    lua_rawset(L,-3); /* Set table DBLINK as field of LIS_GLOBAL_TABLE_NAME */

    lua_pushliteral(L,LIS_DBF_TABLE_NAME); /* Table for DBF types */
    lua_createtable(L,15,1);

    for (i=LIS_LUATABLE_INDEX_START; i<=15; i++) {
      lua_pushstring(L,LIS_DBF_TABLE_FIELDS[i]);
      lua_rawseti(L,-2,i);
    }
    lua_pushinteger(L,0);
    lua_pushstring(L,LIS_DBF_TABLE_FIELDS[0]);
    lua_rawset(L,-3);

    lua_rawset(L,-3); /* Set table DBF as field of LIS_GLOBAL_TABLE_NAME */

    lua_pushliteral(L,LIS_LNK_TABLE_NAME); /* Table for LNK types */
    lua_createtable(L,7,7);

    for (i=LIS_LUATABLE_INDEX_START; i<=7; i++) {
      lua_pushstring(L,LIS_LNK_TABLE_FIELDS[i]);
      lua_rawseti(L,-2,i);
    }
    lua_pushinteger(L,0);
    lua_pushstring(L,LIS_LNK_TABLE_FIELDS[0]);
    lua_rawset(L,-3);
    for (i=10; i<=15; i++) {
      lua_pushinteger(L,i);
      lua_pushstring(L,LIS_LNK_TABLE_FIELDS[i]);
      lua_rawset(L,-3);
    }

    lua_rawset(L,-3); /* Set table LNK as field of LIS_GLOBAL_TABLE_NAME */

    lua_pushliteral(L,LIS_MSK_TABLE_NAME); /* Table for MSK table fields */
    lua_createtable(L,0,15);

    lua_pushstring(L,LIS_MASK2STRING_FUN);
    lua_pushcfunction(L,lisMaskToString);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_MSK_NMS);
    lua_pushinteger(L,LIS_MSK_NMS_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_MS);
    lua_pushinteger(L,LIS_MSK_MS_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_MSI);
    lua_pushinteger(L,LIS_MSK_MSI_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_MSS);
    lua_pushinteger(L,LIS_MSK_MSS_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_PP);
    lua_pushinteger(L,LIS_MSK_PP_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_CA);
    lua_pushinteger(L,LIS_MSK_CA_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_CP);
    lua_pushinteger(L,LIS_MSK_CP_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_CPP);
    lua_pushinteger(L,LIS_MSK_CPP_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_FWD);
    lua_pushinteger(L,LIS_MSK_FWD_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_INPNAT);
    lua_pushinteger(L,LIS_MSK_INPNAT_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_INPSTR);
    lua_pushinteger(L,LIS_MSK_INPSTR_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_OUTNAT);
    lua_pushinteger(L,LIS_MSK_OUTNAT_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_OUTSTR);
    lua_pushinteger(L,LIS_MSK_OUTSTR_VAL);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_MSK_TSELTS);
    lua_pushinteger(L,LIS_MSK_TSELTS_VAL);
    lua_rawset(L,-3);

    lua_rawset(L,-3); /* Set table MSK as field of LIS_GLOBAL_TABLE_NAME */

    res=TRUE;
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global %s already exists!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
  }

  lua_pop(L,1); /* Pop LIS_GLOBAL_TABLE_NAME */
  
  return res;
}

void lisDisposeLuaState(lua_State *L) {
  lua_close(L);
}

lua_State * lisCreateStack(lua_State *L,int *stack_ref) {
  lua_State *stack=NULL;
  NOW_STR;

  if (stack_ref!=NULL) {
    stack=lua_newthread(L);
    if (stack==NULL) {
      lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Failed to create stack in lisCreateStack()\n",now_str,LIS_LIB_LOG_NAME);
    } else {
      *stack_ref=luaL_ref(L,LUA_REGISTRYINDEX);

      if (lisMinStackSize>0) {
        if (!lua_checkstack(L,lisMinStackSize)) {
          lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unable to grow stack to %d in lisCreateStack()\n",now_str,LIS_LIB_LOG_NAME,lisMinStackSize);
        }
      }
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Missing parameter for stack reference count in lisCreateStack()\n",now_str,LIS_LIB_LOG_NAME);
  }

  return stack;
}

void lisDisposeStack(lua_State *L,int stack_ref) {
  luaL_unref(L,LUA_REGISTRYINDEX,stack_ref);
}

void lisCheckStackNotEmpty(lua_State *L) {
  int top;
  NOW_STR;

  top=lua_gettop(L);
  if (top>0) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s State %p stack is not empty (%d items)\n",now_str,LIS_LIB_LOG_NAME,L,top);
  } else if (top<0) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s State %p stack is invalid (%d items)\n",now_str,LIS_LIB_LOG_NAME,L,top);
  }
}

void lisPrintStackTypes(lua_State *L,char *title) {
  int i,top;
  NOW_STR;

  top=lua_gettop(L);
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s --- (start %d) --------------------------- %s\n",now_str,LIS_LIB_LOG_NAME,top,title);
  if (top==0) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p has an empty stack\n",now_str,LIS_LIB_LOG_NAME,L);
  } else {
    for (i=1;i<=top;i++) {
      if (lua_isnil(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d nil\n",now_str,LIS_LIB_LOG_NAME,L,-i);
      } else if (lua_isboolean(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d boolean %d\n",now_str,LIS_LIB_LOG_NAME,L,-i,lua_toboolean(L,-i));
      } else if (lua_isinteger(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d integer %d\n",now_str,LIS_LIB_LOG_NAME,L,-i,(int)lua_tointeger(L,-i));
      } else if (lua_isnumber(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d number %f\n",now_str,LIS_LIB_LOG_NAME,L,-i,lua_tonumber(L,-i));
      } else if (lua_isstring(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d string %s\n",now_str,LIS_LIB_LOG_NAME,L,-i,lua_tostring(L,-i));
      } else if (lua_isfunction(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d function\n",now_str,LIS_LIB_LOG_NAME,L,-i);
      } else if (lua_isuserdata(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d userdata\n",now_str,LIS_LIB_LOG_NAME,L,-i);
      } else if (lua_isthread(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d thread\n",now_str,LIS_LIB_LOG_NAME,L,-i);
      } else if (lua_istable(L,-i)) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s State %p stack index %d table\n",now_str,LIS_LIB_LOG_NAME,L,-i);
      }
    }
  }
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s --- (end) ----------------------------- %s\n",now_str,LIS_LIB_LOG_NAME,title);
}

static int lisRecordClosureIndex(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;
  const char *field_name;
  char *rec_name;
  int pushed=0;
  dbAddr db_addr;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  rec_name=prec->name;

  if (!lua_isstring(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong type for retrieving %s field name (must be string)! \n",now_str,LIS_LIB_LOG_NAME,rec_name);
    lua_pushnil(L);
    pushed=1;
  } else {
    field_name=lua_tostring(L,-1);  

    if (lisGetField(rec_name,field_name,&db_addr)!=NULL) {
      pushed=lisRetrieveFieldToStackDbGet(L,&db_addr,NULL);
      if (pushed<0) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisRecordClosureIndex(): unable to get values from field %s.%s\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
        lua_pushnil(L);
        pushed=1;
      }
    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisRecordClosureIndex(): field %s.%s not found\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
      lua_pushnil(L);
      pushed=1;
    }
  }

  return pushed;
}

static int lisRecordClosureNewIndex(lua_State *L) {
  NOW_STR;
  struct dbCommon *prec;
  const char *field_name;
  char *rec_name;
  dbAddr db_addr;
  int updated=0;

  prec=lua_touserdata(L,lua_upvalueindex(1));
  rec_name=prec->name;

  if (!lua_isstring(L,-2)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong type for updating %s field name (must be string)! \n",now_str,LIS_LIB_LOG_NAME,rec_name);
  } else {
    field_name=lua_tostring(L,-2);

    if (lisGetField(rec_name,field_name,&db_addr)!=NULL) {
      if (lua_isinteger(L,-1)) {
        updated=lisUpdateFieldFromIntegerDbPut(L,&db_addr);

      } else if (lua_isnumber(L,-1)) {
        updated=lisUpdateFieldFromNumberDbPut(L,&db_addr);

      } else if (lua_isstring(L,-1)) {
        updated=lisUpdateFieldFromStringDbPut(L,&db_addr);

      } else if (lua_istable(L,-1)) {
        updated=lisUpdateFieldFromTableDbPut(L,&db_addr,NULL);

      } else {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unknown lua value type for field %s.%s\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
      }

    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error calling lisUpdateMyField(): field %s.%s not found\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
    }
  }

  lua_pushboolean(L,updated>0);
  return 1; /* 1 parameter returned to Lua */
}

int lisCreateLuaRecordClosure(lua_State *L,char *record_name,char *filename,char **params,int num_params,struct lisRecordState *rec_state) {
  NOW_STR;
  int i;

  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    lua_pop(L,1);
    return FALSE;
  }

  lua_pushstring(L,LIS_CLOSURES_TABLE_NAME);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Closures table %s.%s. not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_CLOSURES_TABLE_NAME);
    lua_pop(L,2);
    return FALSE;
  }

  /* Create luaiocsup._records[<recname>] table for record closure */
  lua_pushstring(L,record_name);
  lua_createtable(L,0,5); // REVIEW 3RD PARAMETER!!!
  lua_rawset(L,-3);

  /* Retrieve table for record closure */
  lua_pushstring(L,record_name);
  lua_rawget(L,-2);

  /* Bind record_name() function */
  lua_pushstring(L,LIS_RECNAME_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisMyRecordName,1);
  lua_rawset(L,-3);

  /* Bind retrieve_field() function */
  lua_pushstring(L,LIS_RETRIEVEFLD_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisRetrieveMyField,1);
  lua_rawset(L,-3);

  /* Bind update_field() function */
  lua_pushstring(L,LIS_UPDATEFLD_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisUpdateMyField,1);
  lua_rawset(L,-3);

  /* Bind pact() function */
  lua_pushstring(L,LIS_PACT_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisMyPact,1);
  lua_rawset(L,-3);

  if (!strcmp(rec_state->recordTypeName,LIS_WAVEFORM_REC_NAME)) {
    lua_pushstring(L,LIS_NORD_FUN);
    lua_pushlightuserdata(L,rec_state->pRec);
    lua_pushcclosure(L,lisMyNord,1);
    lua_rawset(L,-3);
  }

  /* Bind scanonce() function */
  lua_pushstring(L,LIS_SCANONCE_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisScanOnce,1);
  lua_rawset(L,-3);

  /* Bind process() function */
  lua_pushstring(L,LIS_PROCESSREQ_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisDbProcess,1);
  lua_rawset(L,-3);

  /* Bind scanlock() function */
  lua_pushstring(L,LIS_SCANLOCK_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisScanLock,1);
  lua_rawset(L,-3);

  /* Bind scanunlock() function */
  lua_pushstring(L,LIS_SCANUNLOCK_FUN);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisScanUnlock,1);
  lua_rawset(L,-3);

  /* Create metatable for record fields */
  lua_createtable(L,0,2);

  lua_pushstring(L,LIS_METATABLE_GET);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisRecordClosureIndex,1);
  lua_rawset(L,-3);

  lua_pushstring(L,LIS_METATABLE_SET);
  lua_pushlightuserdata(L,rec_state->pRec);
  lua_pushcclosure(L,lisRecordClosureNewIndex,1);
  lua_rawset(L,-3);

  lua_setmetatable(L, -2);

  lua_pop(L,2); /* Pop luaiocsup._records[<recname>] and luaiocsup._records */

  lua_pushstring(L,LIS_ARGS_TABLE_NAME);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s.%s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_ARGS_TABLE_NAME);
    lua_pop(L,2);
    return FALSE;
  }

  /* Create luaiocsup._args[<recname>] table for arg */
  lua_pushstring(L,record_name);
  lua_createtable(L,0,1); 
  lua_rawset(L,-3);

  /* Retrieve table for arg */
  lua_pushstring(L,record_name);
  lua_rawget(L,-2);

  /* Push filename */
  lua_pushinteger(L,0);
  lua_pushstring(L,filename);
  lua_rawset(L,-3);
  /* Push script command-line parameters */
  for (i=0;i<num_params;i++) {
    lua_pushinteger(L,i+1);
    lua_pushstring(L,params[i]);
    lua_rawset(L,-3);
  }

  lua_pop(L,3); /* Pop luaiocsup._args[<recname>] and luaiocsup._args and luaiocsup */

  return TRUE;
}

int lisRetrieveLuaRecordClosure(lua_State *L,char *record_name) {
  NOW_STR;

  /* Define arg from LUAIOCSUP._args */
  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    lua_pop(L,1);
    return FALSE; /* Beware: return in the middle of the function! */
  }

  lua_pushstring(L,LIS_ARGS_TABLE_NAME);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s.%s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_ARGS_TABLE_NAME);
    lua_pop(L,2);
    return FALSE;
  }

  lua_pushstring(L,record_name);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Record arguments %s.%s[%s] not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_ARGS_TABLE_NAME,record_name);
    lua_pop(L,3);
    return FALSE;
  }
  lua_setglobal(L,LIS_GLOBAL_ARG_NAME);
  lua_pop(L,1); /* Pop LUAIOCSUP._args */

  /* Define LUAIPCSUP.self from LUAIOCSUP._records */
  lua_pushstring(L,LIS_CLOSURES_TABLE_NAME);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Table %s.%s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_CLOSURES_TABLE_NAME);
    lua_pop(L,2);
    return FALSE;
  }

  lua_pushstring(L,record_name);
  lua_rawget(L,-2);
  if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Record closure %s.%s[%s] not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_CLOSURES_TABLE_NAME,record_name);
    lua_pop(L,3);
    return FALSE;
  }

  lua_pushstring(L,LIS_RECORD_SELF_NAME);
  lua_replace(L,-3);
  lua_rawset(L,-3);

  lua_pop(L,1); /* Pop luaiocsup */

  return TRUE;
}

int lisPushStdRecordClosure(lua_State *L,const char *rec_name) {
  NOW_STR;
  dbAddr db_addr;
  int pushed=0;
  struct dbCommon *prec;

  if (dbNameToAddr(rec_name,&db_addr)==0) {
    prec=db_addr.precord;
    lua_createtable(L,0,1); // REVIEW 3RD PARAMETER!!!

    lua_pushstring(L,LIS_RECNAME_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisMyRecordName,1);
    lua_rawset(L,-3);

    /* Bind retrieve_field() function */
    lua_pushstring(L,LIS_RETRIEVEFLD_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisRetrieveMyField,1);
    lua_rawset(L,-3);

    /* Bind update_field() function */
    lua_pushstring(L,LIS_UPDATEFLD_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisUpdateMyField,1);
    lua_rawset(L,-3);

    /* Bind scanonce() function */
    lua_pushstring(L,LIS_SCANONCE_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisScanOnce,1);
    lua_rawset(L,-3);

    /* Bind process() function */
    lua_pushstring(L,LIS_PROCESSREQ_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisDbProcess,1);
    lua_rawset(L,-3);

    /* Bind scanlock() function */
    lua_pushstring(L,LIS_SCANLOCK_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisScanLock,1);
    lua_rawset(L,-3);

    /* Bind scanunlock() function */
    lua_pushstring(L,LIS_SCANUNLOCK_FUN);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisScanUnlock,1);
    lua_rawset(L,-3);

    /* Create metatable for record fields */
    lua_createtable(L,0,2);

    lua_pushstring(L,LIS_METATABLE_GET);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisRecordClosureIndex,1);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_METATABLE_SET);
    lua_pushlightuserdata(L,prec);
    lua_pushcclosure(L,lisRecordClosureNewIndex,1);
    lua_rawset(L,-3);

    lua_setmetatable(L, -2);

    pushed=1;
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Unable to find record %s\n",now_str,LIS_LIB_LOG_NAME,rec_name);
  }

  return pushed;
}

/* Returns: -1=error 0=already-loaded 1=loaded */
int lisLoadScriptIntoState(struct lisLuaState *lua_state,char *directory,char *filename,int force) {
  int res=0;
  char *full_filename=NULL;
  const char *error_msg;
  NOW_STR;
  int new_file;
  lua_State *L=lua_state->masterState;

  lua_state->loadedFiles=lisAddToLoadedFiles(lua_state->loadedFiles,filename,&new_file);

  if ((new_file)||force) {
      full_filename=lisJoinDirectoryFilename(directory,filename);
      
      if (new_file) {
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Loading %s\n",now_str,LIS_LIB_LOG_NAME,full_filename);
      } else {
		lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Re-loading %s\n",now_str,LIS_LIB_LOG_NAME,full_filename);
	  }
      res=luaL_dofile(L,full_filename);
      if (res) {
        error_msg=lua_tostring(L,-1);
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s %s\n",now_str,LIS_LIB_LOG_NAME,error_msg);
        lua_pop(L,1); /* Pop error message */
        res=-1;
      } else {
        res=1;
	  }
      free(full_filename);
  } else {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s File %s already loaded\n",now_str,LIS_LIB_LOG_NAME,filename);
  }

  return res;
}

/*
  luaiocsup.DBMENU.menuIvoa.menuIvoaSet_output_to_IVOV
  luaiocsup.DBMENU.menuIvoa.CHOISES[2]
  luaiocsup.DBMENU.FIELD["LUA:AO1.IVOA"].menuIvoaSet_output_to_IVOV
  luaiocsup.DBMENU.FIELD["LUA:AO1.IVOA"].CHOISES[2]
*/
/* Beware: return in the middle of the function! */
static int lisDbMenu2Table(lua_State *L,dbMenu *menu,char *recfld_name) {
  NOW_STR;
  int i,entries;

  entries=menu->nChoice;

  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (!lua_istable(L,-1)) {
    lua_pop(L,1);
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    return FALSE;
  }

  /* Retrieve/create LUAIOCSUP.DBMENU table */
  lua_pushstring(L,LIS_DBMENU_TABLE_NAME);
  lua_rawget(L,-2);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lua_pushstring(L,LIS_DBMENU_TABLE_NAME);
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_DBMENU_TABLE_NAME);
    lua_rawget(L,-2);
  } else {
    if (!lua_istable(L,-1)) {
      lua_pop(L,2); /* Pop whatever+LUAIOCSUP */
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Field %s.%s not a table!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBMENU_TABLE_NAME);
      return FALSE;
    }
  }

  /* Create/confirm LUAIOCSUP.DBMENU.FIELD table */
  lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
  lua_rawget(L,-2);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
    lua_newtable(L);
    lua_rawset(L,-3);
  } else {
    if (!lua_istable(L,-1)) {
      lua_pop(L,3); /* Pop whatever+DBMENU+LUAIOCSUP */
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Field %s.%s.%s not a table!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBMENU_TABLE_NAME,LIS_DBFIELD_TABLE_NAME);
      return FALSE;
    }
    lua_pop(L,1);
  }

  /* Create menu table, shared by all fields */
  lua_pushstring(L,menu->name);
  lua_rawget(L,-2);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);

    /* Create table for menu options */
    lua_pushstring(L,menu->name);
    lua_newtable(L);
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Creating menu table %s.%s.%s for field %s\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBMENU_TABLE_NAME,menu->name,recfld_name);

    /* Create entries */
    lisGenLogTime(now_str,sizeof(now_str));
    for (i=0; i<entries; i++) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,NULL,"%s %s   %s = %d\n",now_str,LIS_LIB_LOG_NAME,menu->papChoiceName[i],i);
      lua_pushstring(L,menu->papChoiceName[i]);
      lua_pushinteger(L,i);
      lua_rawset(L,-3); 
    }

    /* Create CHOISES sub-table and entries */
    lua_pushstring(L,LIS_DBCHOICES_TABLE_NAME);
    lua_newtable(L);
    for (i=0; i<entries; i++) {
      lua_pushinteger(L,i);
      lua_pushstring(L,menu->papChoiceValue[i]);
      lua_rawset(L,-3);
    }
    lua_rawset(L,-3); /* Set CHOISES field in menu->name table */

    lua_rawset(L,-3); /* Set menu->name field in LUAIOCSUP.DBMENU table */
  } else {
    if (!lua_istable(L,-1)) {
      lua_pop(L,3); /* Pop whatever+DBMENU+LUAIOCSUP */
      lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Field %s.%s.%s not a table\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBMENU_TABLE_NAME,menu->name);
      return FALSE;
    }
    lua_pop(L,1);
  }

  /* Link this specific field to shared menu table */
  lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
  lua_rawget(L,-2);
  lua_pushstring(L,recfld_name);
  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME); /* Retrieve menu->name field from LUAIOCSUP.DBMENU */
  lua_pushstring(L,LIS_DBMENU_TABLE_NAME);
  lua_rawget(L,-2);
  lua_remove(L,-2); /* Remove LUAIOCSUP */
  lua_pushstring(L,menu->name);
  lua_rawget(L,-2);
  lua_remove(L,-2); /* Remove DBMENU */
  lua_rawset(L,-3);

  lua_pop(L,3); /* Pop RECORD+DBMENU+LUAIOCSUP */

  return TRUE;
}

/*
  luaiocsup.DBDEVICE.FIELD["LUA:AO1.DTYP"]["Soft Channel"]
  luaiocsup.DBDEVICE.FIELD["LUA:AO1.DTYP"].CHOISES[0]
*/
/* Beware: return in the middle of the function! */
static int lisDbDevice2Table(lua_State *L,dbDeviceMenu *devmenu,char *recfld_name) {
  NOW_STR;
  int i,entries;
  char **strings;

  entries=devmenu->nChoice;
  strings=devmenu->papChoice;

  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (!lua_istable(L,-1)) {
    lua_pop(L,1);
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s not found!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    return FALSE;
  }

  /* Retrieve/create LUAIOCSUP.DBDEVICE table */
  lua_pushstring(L,LIS_DBDEVICE_TABLE_NAME);
  lua_rawget(L,-2);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lua_pushstring(L,LIS_DBDEVICE_TABLE_NAME);
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_DBDEVICE_TABLE_NAME);
    lua_rawget(L,-2);
  } else {
    if (!lua_istable(L,-1)) {
      lua_pop(L,2); /* Pop whatever+LUAIOCSUP */
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Field %s.%s not a table!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBDEVICE_TABLE_NAME);
      return FALSE;
    }
  }

  /* Retrieve/create LUAIOCSUP.DBDEVICE.FIELD table */
  lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
  lua_rawget(L,-2);
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushstring(L,LIS_DBFIELD_TABLE_NAME);
    lua_rawget(L,-2);
  } else {
    if (!lua_istable(L,-1)) {
      lua_pop(L,3); /* Pop whatever+DBDEVICE+LUAIOCSUP */
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Field %s.%s.%s not a table!\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBDEVICE_TABLE_NAME,LIS_DBFIELD_TABLE_NAME);
      return FALSE;
    }
  }

  /* Create table for device options */
  lua_pushstring(L,recfld_name);
  lua_newtable(L);
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Creating device table %s.%s.%s[%s]\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME,LIS_DBDEVICE_TABLE_NAME,LIS_DBFIELD_TABLE_NAME,recfld_name);

  lisGenLogTime(now_str,sizeof(now_str));
  for (i=0; i<entries; i++) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,NULL,"%s %s   %s = %d\n",now_str,LIS_LIB_LOG_NAME,strings[i],i);
    lua_pushstring(L,strings[i]);
    lua_pushinteger(L,i);
    lua_rawset(L,-3);
  }
  lua_rawset(L,-3); /* Set full_name field in LUAIOCSUP.DBDEVICE.FIELD table */

  /* Create CHOISES sub-table and entries */
  lua_pushstring(L,LIS_DBCHOICES_TABLE_NAME);
  lua_newtable(L);
  for (i=0; i<entries; i++) {
    lua_pushinteger(L,i);
    lua_pushstring(L,strings[i]);
    lua_rawset(L,-3);
  }
  lua_rawset(L,-3); /* Set CHOISES field in LUAIOCSUP.DBDEVICE.FIELD table */

  lua_pop(L,3); /* Pop RECORD+DBDEVICE+LUAIOCSUP */

  return TRUE;
}

void lisCreateMenusAndDevices(lua_State *L,struct lisRecordState *rec_state) {
  dbFldDes **papFldDes,*fld;
  short no_fields;
  int i;
  char recfld_name[LIS_GETFLD_LEN];

  papFldDes=rec_state->pRec->rdes->papFldDes;
  no_fields=rec_state->pRec->rdes->no_fields;
  for (i=0;i<no_fields;i++) {
    fld=papFldDes[i];
    if ((fld->field_type==DBF_MENU)||(fld->field_type==DBF_DEVICE)) {
      strcpy(recfld_name,rec_state->recordName);
      strcat(recfld_name,LIS_RECFLD_SEPARATOR);
      strcat(recfld_name,fld->name);
      if (fld->field_type==DBF_MENU) {
        lisDbMenu2Table(L,fld->ftPvt,recfld_name);
      } else {
        lisDbDevice2Table(L,fld->ftPvt,recfld_name);
	  }
    }
  }
}

/* Returns: FALSE=error TRUE=ok */
int lisPostInitMasterBindings(lua_State *L) {
  NOW_STR;
  int res=TRUE;

  lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
  if (lua_isnil(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Unknown global table %s\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    res=FALSE;
  } else if (!lua_istable(L,-1)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Global table %s not a table\n",now_str,LIS_LIB_LOG_NAME,LIS_GLOBAL_TABLE_NAME);
    res=FALSE;
  }

  if (res) {
    lua_pushstring(L,LIS_SCANIOINI_FUN);
    lua_pushcfunction(L,lisDummyScanIoInit);
    lua_rawset(L,-3);

    lua_pushstring(L,LIS_SCANIOREQ_FUN);
    lua_pushcfunction(L,lisScanIoRequest);
    lua_rawset(L,-3);

    /* SHOULD THIS BE MOVED TO BEFORE POSTINIT?!?!?! */
    lua_pushstring(L,LIS_FINDREQ_FUN);
    lua_pushcfunction(L,lisFindRecord);
    lua_rawset(L,-3);
  }
  lua_pop(L,1); /* Pop LUAIOCSUP */

  return res;
}

/* Returns: -1=error 0=nothing-pushed 1=rec-pushed */
int lisPrepareLuaEnvironment(lua_State *L,char *record_name,int pop_rec_clos,int stack_depth) {
  NOW_STR;
  int pushed=-1;

  if (lisRetrieveLuaRecordClosure(L,record_name)) {
    pushed=0;
    if (!pop_rec_clos) {
      /* Push luaiocsup.self into stack */
      lua_getglobal(L,LIS_GLOBAL_TABLE_NAME);
      lua_pushstring(L,LIS_RECORD_SELF_NAME);
      lua_rawget(L,-2);
      lua_remove(L,-2);
      if (stack_depth>0) {
        lua_insert(L,-stack_depth-1);
	  }
      pushed=1;
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Unable to retrieve record closure or specific arguments\n",now_str,LIS_LIB_LOG_NAME);
  }

  return pushed;
}

/* Returns: -1=error 1=method-pushed */
int lisPushMethod(lua_State *L,char *record_name,char *table_name,char *method_name,int stack_depth) {
  NOW_STR;
  int pushed=-1;

  if (table_name!=NULL) {
    lua_getglobal(L,table_name);
    if (lua_istable(L,-1)) {
      lua_pushstring(L,method_name);
      lua_rawget(L,-2);
      if (!lua_isfunction(L,-1)) {
        lua_pop(L,2);
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error pushing %s function '%s.%s' (%s not a function name)\n",now_str,LIS_LIB_LOG_NAME,record_name,table_name,method_name,method_name);
      } else {
        lua_remove(L,-2);
        pushed=1;
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushed %s function '%s.%s'\n",now_str,LIS_LIB_LOG_NAME,record_name,table_name,method_name);
      }
    } else {
      lua_pop(L,1);
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error pushing %s function '%s.%s' (%s not a global table)\n",now_str,LIS_LIB_LOG_NAME,record_name,table_name,method_name,table_name);
    }
  } else {
    lua_getglobal(L,method_name);
    if (!lua_isfunction(L,-1)) {
      lua_pop(L,1);
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error pushing %s function '%s' (not a function name)\n",now_str,LIS_LIB_LOG_NAME,record_name,method_name);
    } else {
      pushed=1;
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushed %s function '%s'\n",now_str,LIS_LIB_LOG_NAME,record_name,method_name);
    }
  }

  if ((pushed==1)&&(stack_depth>0)) {
    lua_insert(L,-stack_depth-1);
  }

  return pushed;
}

/* Returns: -1=error 1=code-pushed */
int lisPushChunk(lua_State *L,char *record_name,char *lua_code) {
  NOW_STR;
  int pushed=-1;
  const char *error_msg;

  if (!luaL_loadstring(L,lua_code)) {
    pushed=1;
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushed %s code chunk '%s'\n",now_str,LIS_LIB_LOG_NAME,record_name,lua_code);
  } else {
	error_msg=lua_tostring(L,-1);
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error loading Lua code from string (%s)\n",now_str,LIS_LIB_LOG_NAME,error_msg);
    lua_pop(L,1);
  }

  return pushed;
}

int lisPrepLuaAndPCall(lua_State *L,char *record_name,char *class_name,char *method_name,int allowed_types) {
  NOW_STR;
  int pushed_rec_arg,pcall_res,stack_top,num_ret_args;
  int res=LIS_EPICS_SUCCESS,is_chunk=FALSE;

  stack_top=lua_gettop(L);

  if ((allowed_types==LIS_ALLOW_FUN_OR_CHK)||(allowed_types==LIS_ALLOW_FUNCTION_ONLY)) {
    if (lisPushMethod(L,record_name,class_name,method_name,0)==-1) {
      is_chunk=TRUE;
      if (allowed_types==LIS_ALLOW_FUN_OR_CHK) {
        if (lisPushChunk(L,record_name,method_name)==-1) {
          res=S_db_BadSub;
        }
      } else {
        res=S_db_BadSub;
      }
	}
  } else {
    is_chunk=TRUE;
    if (lisPushChunk(L,record_name,method_name)==-1) {
      res=S_db_BadSub;
    }
  }

  if (res==LIS_EPICS_SUCCESS) {
    pushed_rec_arg=lisPrepareLuaEnvironment(L,record_name,is_chunk,0);
    if (pushed_rec_arg!=-1) {
      pcall_res=lua_pcall(L,pushed_rec_arg,LUA_MULTRET,0);
      if (pcall_res==0) {
        num_ret_args=lua_gettop(L)-stack_top;
        if (num_ret_args>0) {
          if (lua_isinteger(L,-num_ret_args)) {
            res=lua_tointeger(L,-num_ret_args);
            if (num_ret_args>1) {
              lisLog(LIS_LOGLVL_VERBOSE,errlogMinor,now_str,"%s %s Too many return parameters by %s of record %s\n",now_str,LIS_LIB_LOG_NAME,method_name,record_name);
			}

          } else {
            lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Wrong type returned by %s of record %s (must be a single integer)\n",now_str,LIS_LIB_LOG_NAME,method_name,record_name);
            res=S_db_errArg;
          }
		}

	  } else {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Failed calling %s of record %s\n",now_str,LIS_LIB_LOG_NAME,method_name,record_name);
        res=S_db_BadSub;
	  }

	} else {
	  lisLog(LIS_LOGLVL_VERBOSE,errlogMinor,now_str,"%s %s Unable to prepare %s environment of record %s\n",now_str,LIS_LIB_LOG_NAME,method_name,record_name);
      res=S_db_BadSub;
	}
  }

  lua_settop(L,stack_top);

  return res;
}

int lisPopArgsExchange(lua_State *L,char *st_name,int *num_args,int mandatory_args) {
  NOW_STR;
  int n_args=0,res=TRUE;

  if (lua_gettop(L)>0) {
    if (lua_isinteger(L,-1)) {
      n_args=lua_tointeger(L,-1);
      if (n_args<1) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error popping arguments-exchange for <%s>!\n",now_str,LIS_LIB_LOG_NAME,st_name);
        lisPrintStackTypes(L,"lisPopArgsExchange");
        res=FALSE;
      } else {
        if (num_args) {
          *num_args=n_args;
        }
        lua_pop(L,1);
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Found %d arguments-exchange for <%s>\n",now_str,LIS_LIB_LOG_NAME,n_args,st_name);
      }
    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error popping arguments-exchange for <%s>!\n",now_str,LIS_LIB_LOG_NAME,st_name);
      lisPrintStackTypes(L,"lisPopArgsExchange");
      res=FALSE;
    }
  } else {
    if (mandatory_args) {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error popping arguments-exchange for <%s>!\n",now_str,LIS_LIB_LOG_NAME,st_name);
      res=FALSE;
    } else {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s No arguments-exchange found for <%s>\n",now_str,LIS_LIB_LOG_NAME,st_name);
      if (num_args) {
        *num_args=0;
      }
    }
  }

  return res;
}

void lisPushArgsExchange(lua_State *from_L,lua_State *to_L,char *from_name,char *to_name,int from_idx,int to_idx) {
  NOW_STR;
  int i,num_args;

  num_args=to_idx-from_idx+1;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushing arguments-exchange: %d args from <%s> to <%s>\n",now_str,LIS_LIB_LOG_NAME,num_args,from_name,to_name);

  for (i=from_idx; i<=to_idx; i++) {
    if (lua_isnil(from_L,i)) {
      lua_pushnil(to_L);
    } else if (lua_isboolean(from_L,i)) {
      lua_pushboolean(to_L,lua_toboolean(from_L,i));
    } else if (lua_isinteger(from_L,i)) {
      lua_pushinteger(to_L,lua_tointeger(from_L,i));
    } else if (lua_isnumber(from_L,i)) {
      lua_pushnumber(to_L,lua_tonumber(from_L,i));
    } else if (lua_isstring(from_L,i)) {
      lua_pushstring(to_L,lua_tostring(from_L,i));
    } else if (lua_islightuserdata(from_L,i)) {
      lua_pushlightuserdata(to_L,lua_touserdata(from_L,i));
    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Unknown args-exchange type, pushing nil instead!\n",now_str,LIS_LIB_LOG_NAME);
      lua_pushnil(to_L);
    }
  }
  lua_pushinteger(to_L,num_args);
}

void lisMoveArgsExchange(lua_State *from_L,lua_State *to_L,char *from_name,char *to_name,int num_args) {
  NOW_STR;

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Moving arguments-exchange: %d args from <%s> to <%s>\n",now_str,LIS_LIB_LOG_NAME,num_args,from_name,to_name);
  lua_xmove(from_L,to_L,num_args);
  lua_pushinteger(to_L,num_args);
}
