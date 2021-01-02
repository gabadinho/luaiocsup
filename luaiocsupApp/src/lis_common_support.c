#include "lis_common_support.h"

#include "lis_lua.h"
#include "lis_log.h"
#include "lis_string_manip.h"
#include "lis_epics.h"
#include "lis_luasubrec.h"
#include "lis_data_cast.h"

#include <dbAccess.h>
#include <recSup.h>
#include <epicsExit.h>

void lisCallAtExitCallbacks(void *arg) {
  struct lisAtExitList *at_exit=lisGetAtExit();
  while (at_exit) {
    lisLogSimple("%s Executing at-exit callback %p function with %d arguments\n",LIS_LIB_LOG_NAME,lua_topointer(at_exit->atExitStack,1),lua_gettop(at_exit->atExitStack)-1);
    lua_pcall(at_exit->atExitStack,lua_gettop(at_exit->atExitStack)-1,LUA_MULTRET,0);
    at_exit=at_exit->next;
  }
}

void lisDevSupPhase1Init() {
  struct lisLuaStateList *lua_states;
  NOW_STR;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Performing device support phase-1 bindings\n",now_str,LIS_LIB_LOG_NAME);

  lisPutLinkLock=lisMutexCreate();
  callbackSetCallback(lisUpdateLinkFieldCallback,&lisPutLinkCallback);

  epicsAtExit(lisCallAtExitCallbacks,NULL);

  lua_states=lisGetLuaStates();
  while (lua_states!=NULL) {
    lisPostInitMasterBindings(lua_states->lisState->masterState);
    lua_states=lua_states->next;
  }
}

static DBLINK * lisGetScriptDbLink(struct dbCommon *precord,char *dblink_name) {
  DBLINK *db_link=NULL;
  dbFldDes *fld;
  int i;
  size_t record_addr;
  dbFldDes **papFldDes=precord->rdes->papFldDes;
  short no_fields=precord->rdes->no_fields;
  
  if (dblink_name!=NULL) {
    for (i=0;i<no_fields;i++) {
      fld=papFldDes[i];
      if (!strcmp(dblink_name,fld->name)) {
        record_addr=(size_t)precord+fld->offset;
        db_link=(DBLINK *)record_addr;
      }
    }
  }

  return db_link;
}

static void lisCallbackWrapper(CALLBACK *rec_callback) {
  NOW_STR;
  struct lisRecordState *priv;
  lua_State *L;
  dbCommon *prec;
  struct typed_rset *prset;
  int num_args,pcall_res,num_ret_args=0,stack_base,extra_args;
  const char *error_msg;
  char *rec_name;

  callbackGetUser(priv,rec_callback);
  rec_name=priv->recordName;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Entering callback-wrapper for record %s\n",now_str,LIS_LIB_LOG_NAME,rec_name);

  if (priv->recordRuntime->markedForDeletion) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Skipping callback, record %s is marked for deletion\n",now_str,LIS_LIB_LOG_NAME,rec_name);
    return; /* Beware: return in the middle of the function! */
  }

  prec=priv->pRec;
  dbScanLock(prec);
  lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

  L=priv->recordRuntime->callbackStack;

  extra_args=lisPopArgsExchange(L,"callback",&num_args,TRUE); /* Beware: num_args includes the function */
  if (extra_args==FALSE) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Found corrupted stack inside callback-wrapper of record %s\n",now_str,LIS_LIB_LOG_NAME,rec_name);
    lua_settop(L,0);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    dbScanUnlock(prec);
    return;
  }

  if (lisPrepareLuaEnvironment(L,rec_name,FALSE,num_args-1)==-1) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unable to prepare environment for callback of record %s\n",now_str,LIS_LIB_LOG_NAME,rec_name);
    lua_pop(L,num_args);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    dbScanUnlock(prec);
    return;
  }
  stack_base=lua_gettop(L)-(num_args+1);

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Executing callback of record %s with <rec> plus %d extra parameters\n",now_str,LIS_LIB_LOG_NAME,rec_name,num_args-1);
  pcall_res=lua_pcall(L,num_args,LUA_MULTRET,0);
  if (pcall_res!=LIS_EPICS_SUCCESS) {
    error_msg=lua_tostring(L,-1);
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s %s\n",now_str,LIS_LIB_LOG_NAME,error_msg);
    lua_pop(L,1);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    dbScanUnlock(prec);
    return;
  } 
  
  num_ret_args=lua_gettop(L)-stack_base;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Callback of record %s returned %d arguments\n",now_str,LIS_LIB_LOG_NAME,rec_name,num_ret_args);
  if (num_ret_args>0) {
    lisMoveArgsExchange(L,priv->recordRuntime->coroutStack,"callback","processing",num_ret_args);
  }

  /* Re-process record */
  prset=(struct typed_rset *)prec->rset;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Re-processing record %s inside callback-wrapper\n",now_str,LIS_LIB_LOG_NAME,rec_name);
  (*prset->process)(prec);
  lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
  dbScanUnlock(prec);

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Exiting callback-wrapper of record %s\n",now_str,LIS_LIB_LOG_NAME,rec_name);
}

static int lisParseProcMethodResult(lua_State *L,char *rec_name,int stack_base,int *callback_found,int *num_cb_ret_params) {
  NOW_STR;
  int res=LIS_EPICS_SUCCESS,num_ret_params;

#ifdef LIS_ALLOW_DEBUG_CODE
  lisPrintStackTypes(L,"lisParseProcMethodResult");
#endif

  if (callback_found) {
    *callback_found=FALSE;
  }
  if (num_cb_ret_params) {
    *num_cb_ret_params=0;
  }

  /* Search for return code */
  num_ret_params=lua_gettop(L)-stack_base;
  if ((num_ret_params>0)&&(lua_isinteger(L,-num_ret_params))) {
    res=lua_tointeger(L,-num_ret_params);
    lua_remove(L,-num_ret_params);    
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Return code from %s processing is %d",now_str,LIS_LIB_LOG_NAME,rec_name,res);
    num_ret_params-=1;
  }

  /* Check for callback */
  if (num_ret_params>0) {
    if (lua_isfunction(L,-num_ret_params)) {
      num_ret_params-=1;
      if (callback_found) {
        *callback_found=TRUE;
      }
      if (num_cb_ret_params) {
        *num_cb_ret_params=num_ret_params;
      }
    }  else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Invalid returned arguments from %s, must be callback-function followed by its arguments!\n",now_str,LIS_LIB_LOG_NAME,rec_name);
      lua_pop(L,num_ret_params); /* Clear stack of invalid parameters */
      res=S_db_errArg;
    }
  }

  return res;
}

long lisReport(int interest,char *rec_report_type) {
  struct lisRecordList *rec,*rec2;
  struct lisLuaState *lua_state;
  struct lisRecordState *rec_state;
  struct lisFilenameList *loaded_files;
  int sharing;
  char *string_buffer;

  lisMutexLock(lisRecordsAddDelLock);

#ifdef LIS_ALLOW_DEBUG_CODE
  interest=1;
#endif

  if (interest) {
    rec=lisGetSupportRecords();
    while (rec!=NULL) {
      if (!strcmp(rec->recordState->recordTypeName,rec_report_type)) {
        rec_state=rec->recordState;
        lua_state=rec_state->lisState;
        lisLogSimple("  %s.%s \"%s\"\n",rec->recordState->recordName,rec->recordState->recordConfig->dbLinkFieldName,rec->recordState->recordConfig->configString);
        if (lua_state->stateId!=NULL) {
          sharing=0;
          lisLogSimple("    State id: %s\n",lua_state->stateId);

          lisFormatString(&string_buffer,"%s","    Files   :");
          loaded_files=lua_state->loadedFiles;
          while (loaded_files!=NULL) {
            lisAppendString(string_buffer," ");
            lisAppendString(string_buffer,loaded_files->filename);
            loaded_files=loaded_files->next;
          }
          lisLogSimple("%s\n",string_buffer);
          lisDiscardString(string_buffer);

          rec2=lisGetSupportRecords();
          while (rec2!=NULL) {
            if (rec2!=rec) {
              if (rec->recordState->lisState==rec2->recordState->lisState) {
                if (sharing==0) {
                  sharing=1;
                  lisFormatString(&string_buffer,"%s","    Sharing :");
                }
                lisAppendString(string_buffer," ");
                lisAppendString(string_buffer,rec2->recordState->recordName);
              }
            }
            rec2=rec2->next;
          }
          if (sharing) {
            lisLogSimple("%s\n",string_buffer);
            lisDiscardString(string_buffer);
          }

        } else {
          lisLogSimple("    Anonymous state\n");
          lisLogSimple("    File: %s\n",lua_state->loadedFiles->filename);
        }

      }
      rec=rec->next;
    }

  } else {
    rec=lisGetSupportRecords();
    while (rec!=NULL) {
      if (!strcmp(rec->recordState->recordTypeName,rec_report_type)) {
        lisLogSimple("  %s.%s \"%s\"\n",rec->recordState->recordName,rec->recordState->recordConfig->dbLinkFieldName,rec->recordState->recordConfig->configString);
      }
      rec=rec->next;
    }
  }

  lisMutexUnlock(lisRecordsAddDelLock);

  return LIS_EPICS_SUCCESS;
}

long lisAddRecord(struct dbCommon *precord,char *db_link_field,char *proc_meth_name) {
  int res=LIS_EPICS_SUCCESS;
  NOW_STR;

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Adding record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);

  lisMutexLock(lisRecordsAddDelLock);
  res=lisGenerateRecord(precord,db_link_field,LIS_ADD_REC_FUN,proc_meth_name,lisIsIocRunning);
  lisMutexUnlock(lisRecordsAddDelLock);

  return res;
}

int lisDeleteRecord(struct dbCommon *precord) {
  int res=LIS_EPICS_SUCCESS;
  NOW_STR;
  struct lisRecordState *priv;
  struct lisLuaState *state2delete=NULL;

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Deleting record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);

  priv=precord->dpvt;
  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Cannot delete uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);
    res=S_db_errArg;
  } else {
    lisMutexLock(lisRecordsAddDelLock);
    lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    lisPrepLuaAndPCall(priv->recordRuntime->coroutStack,priv->recordName,priv->recordConfig->className,LIS_DEL_REC_FUN,LIS_ALLOW_FUNCTION_ONLY);
  
    if (priv->recordRuntime->isYielded) {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Record %s is yielded when deleting!\n",now_str,LIS_LIB_LOG_NAME,precord->name);
	}

	lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->stackReference);
	lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->cbStackReference);

    state2delete=lisDeleteRecordState(priv);

    /* Delete only anonymous Lua states */
    if ((state2delete)&&(state2delete->stateId==NULL)) {
      lisDeleteLuaState(state2delete);
	} else {
      lisMutexUnlock(priv->lisState->stateLock);
      state2delete=NULL;
	}
    lisMutexUnlock(lisRecordsAddDelLock);
  }

  return res;
}

/*********************************** CODE TO REVIEW BELOW!!! */
/*********************************** TRY TO FREE STUFF WHEN RETURNING DUE TO LACK OF MEMORY!!! */
/* Beware: return in the middle of the function! */
long lisGenerateRecord(struct dbCommon *precord,char *db_link_field,char *init_meth_name,char *proc_meth_name,int force_load) {
  struct lisRecordState *priv;
  struct lisLuaState *lua_st;
  struct lisRecordConfig *rec_config;
  struct lisRecordRuntime *rec_rt;
  char *rec_type_name,*usr_rec_type,*name,*db_link_text,*string_buffer;
  int i,is_new_state=FALSE;
  short db_link_type;
  DBLINK *db_link;
  NOW_STR;

  precord->dpvt=NULL;
  name=precord->name;

  rec_type_name=precord->rdes->name;
  usr_rec_type=lisStrToUpper(rec_type_name);
  if (!usr_rec_type) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing record '%s'\n",now_str,LIS_LIB_LOG_NAME,name);
    return S_db_noMemory; /* Beware: return in the middle of the function! */
  }

  db_link=lisGetScriptDbLink(precord,db_link_field);
  if (!db_link) {
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Unable to find field %s in %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,db_link_field,usr_rec_type,name);
    free(usr_rec_type);
    return S_db_badField; /* Beware: return in the middle of the function! */
  }
  db_link_text=db_link->value.instio.string; //db_link_text=db_link->text;
  db_link_type=db_link->type;

  switch (db_link_type) {
    case (INST_IO):
      rec_config=lisCreateRecordConfig(db_link_field,db_link_text);
      if (!rec_config) {
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Unable to parse %s record %s.%s field (%s)\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name,db_link_field,db_link_text);
        free(usr_rec_type);
        return S_db_errArg; /* Beware: return in the middle of the function! */
      }

      lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Creating %s record %s from file %s\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name,rec_config->scriptFilename);
      if (rec_config->stateId==NULL) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  State id : [anonymous]\n",now_str,LIS_LIB_LOG_NAME);
      } else if (rec_config->stateId==rec_config->scriptFilename) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  State id : [default] %s\n",now_str,LIS_LIB_LOG_NAME,rec_config->stateId);
      } else {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  State id : [specified] %s\n",now_str,LIS_LIB_LOG_NAME,rec_config->stateId);
      }
      if (rec_config->className!=NULL) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Table    : [specified] %s\n",now_str,LIS_LIB_LOG_NAME,rec_config->className);
      }
      if (rec_config->paramsLen>0) {
        lisFormatString(&string_buffer,"%s","Arguments:");
        for (i=0;i<rec_config->paramsLen;i++) {
          lisAppendString(string_buffer," ");
          lisAppendString(string_buffer,rec_config->scriptParams[i]);
        }
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  %s\n",now_str,LIS_LIB_LOG_NAME,string_buffer);
        lisDiscardString(string_buffer);
      }

      priv=lisCreateRecordState((struct dbCommon *)precord,name,usr_rec_type);
      if (!priv) {
        lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
        lisDeleteRecordConfig(rec_config);
        free(usr_rec_type);
        return S_db_noMemory; /* Beware: return in the middle of the function! */
      }
      priv->recordConfig=rec_config;

      lua_st=lisFetchLuaState(rec_config->stateId,name,usr_rec_type,&is_new_state);
      if (!lua_st) {
        lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
        free(priv);
        lisDeleteRecordConfig(rec_config);
        free(usr_rec_type);
        return S_db_noMemory; /* Beware: return in the middle of the function! */
      }
      priv->lisState=lua_st;
      if (is_new_state) {
        lisInitializeLuaState(lua_st->masterState);
      }

      rec_rt=lisCreateRecordRuntime(proc_meth_name);
      if (!rec_rt) {
        lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
        if (is_new_state) {
          lisDeleteLuaState(lua_st);
        } else {
          lisMutexUnlock(lua_st->stateLock);
        }
        free(priv);
        lisDeleteRecordConfig(rec_config);
        free(usr_rec_type);
        return S_db_noMemory; /* Beware: return in the middle of the function! */
	  }

      rec_rt->coroutStack=lisCreateStack(priv->lisState->masterState,&(rec_rt->stackReference));
      if (!rec_rt->coroutStack) {
        lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
        lisDeleteRecordRuntime(rec_rt);
        if (is_new_state) {
          lisDeleteLuaState(lua_st);
        } else {
          lisMutexUnlock(lua_st->stateLock);
        }
        free(priv);
        lisDeleteRecordConfig(rec_config);
        free(usr_rec_type);
        return S_db_noMemory; /* Beware: return in the middle of the function! */
	  }

      rec_rt->callbackStack=lisCreateStack(priv->lisState->masterState,&(rec_rt->cbStackReference));
      if (!rec_rt->callbackStack) {
        lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory for initializing %s record '%s'\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
        lisDisposeStack(priv->lisState->masterState,rec_rt->stackReference);
        lisDeleteRecordRuntime(rec_rt);
        if (is_new_state) {
          lisDeleteLuaState(lua_st);
        } else {
          lisMutexUnlock(lua_st->stateLock);
        }
        free(priv);
        lisDeleteRecordConfig(rec_config);
        free(usr_rec_type);
        return S_db_noMemory; /* Beware: return in the middle of the function! */
	  }

      priv->recordRuntime=rec_rt;

      lisCreateMenusAndDevices(lua_st->masterState,priv);

      lisCreateLuaRecordClosure(lua_st->masterState,name,rec_config->scriptFilename,rec_config->scriptParams,rec_config->paramsLen,priv);
      lisRetrieveLuaRecordClosure(lua_st->masterState,name);

      if ((force_load)||!(lisIsFileLoaded(lua_st->loadedFiles,rec_config->scriptFilename))) {
        if (lisLoadScriptIntoState(lua_st,lisBaseDirectory,rec_config->scriptFilename,force_load)==-1) {
          /* Note: continue even though Lua script wasn't properly loaded into state */
          lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error loading file %s into %s record '%s', continuing nevertheless...\n",now_str,LIS_LIB_LOG_NAME,rec_config->scriptFilename,usr_rec_type,name);
        }
      }

      precord->dpvt=priv; /* No returns from this point on! */
      break;

    default:
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Illegal type for %s record '%s' (must be INST_IO, is %d)\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name,db_link_type);
      return S_db_badField; /* Beware: return in the middle of the function! */
  }

  if (lisAddRecordToSupport(priv)==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unable to add %s record '%s' to support data structures!\n",now_str,LIS_LIB_LOG_NAME,usr_rec_type,name);
  }

  callbackSetProcess(&priv->recordRuntime->asyncCallback,precord->prio,priv);
  callbackSetCallback(lisCallbackWrapper,&priv->recordRuntime->asyncCallback);

  lisCheckStackNotEmpty(lua_st->masterState);

  if ((init_meth_name!=NULL)&&(strlen(init_meth_name)>0)) {
    lisPrepLuaAndPCall(rec_rt->coroutStack,name,rec_config->className,init_meth_name,LIS_ALLOW_FUNCTION_ONLY);
  }

  if ((is_new_state)&&(force_load)) { /* The force_load parameter is in reality the global lisIsIocRunning so use it to trigger post-init bindings */
    lisPostInitMasterBindings(lua_st->masterState);
  }

  lisMutexUnlock2(lua_st->stateLock,rec_rt->stackLock);

  return LIS_EPICS_SUCCESS;
}

/* Beware: return in the middle of the function! */
long lisProcess(struct dbCommon *precord,int allowed_types,int *code_type) {
  NOW_STR;
  struct lisRecordState *priv;
  lua_State *L;
  long res=LIS_EPICS_SUCCESS;
  char *code2run,*name;
  const char *error_msg=NULL;
  int num_extra_args=0,pushed_code=0,is_chunk=FALSE,pushed_rec_arg=0;
  int resume_res,callback_found=FALSE,num_cb_params=0,stack_base,extra_args, resumed_args;
  
  name=precord->name;
  lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Processing record %s\n",now_str,LIS_LIB_LOG_NAME,name);

  if (code_type) *code_type=LIS_CODTYP_UNKNOWN;

  priv=precord->dpvt;
  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Cannot process uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }
  lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

  if (priv->recordRuntime->coroutStack==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Cannot process uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }
  if (priv->recordRuntime->markedForDeletion) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Skipping processing, record %s is marked for deletion\n",now_str,LIS_LIB_LOG_NAME,name);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    return S_db_notFound; /* Beware: return in the middle of the function! */
  }

  code2run=priv->recordRuntime->luaCode2Run;
  L=priv->recordRuntime->coroutStack;

  extra_args=lisPopArgsExchange(L,"processing",&num_extra_args,FALSE);
  if (extra_args==FALSE) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Found corrupted stack inside processing-handler of record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    lua_settop(L,0);
    num_extra_args=0;
  }

  if (priv->recordRuntime->isYielded) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Resuming yielded record %s\n",now_str,LIS_LIB_LOG_NAME,name);

  } else {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Starting processing of record %s\n",now_str,LIS_LIB_LOG_NAME,name);

    if ((allowed_types==LIS_ALLOW_FUN_OR_CHK)||(allowed_types==LIS_ALLOW_FUNCTION_ONLY)) {
      pushed_code=lisPushMethod(L,name,priv->recordConfig->className,code2run,num_extra_args);
      if (pushed_code==-1) {
        if (num_extra_args>0) {
          lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s  Function not found by name for record %s, popping extra args!\n",now_str,LIS_LIB_LOG_NAME,name);
          lua_pop(L,num_extra_args);
          num_extra_args=0;
	    }
        if (allowed_types==LIS_ALLOW_FUN_OR_CHK) {
          is_chunk=TRUE;
          pushed_code=lisPushChunk(L,name,code2run);
          if ((pushed_code==1)&&(code_type)) *code_type=LIS_CODTYP_CHUNK;
	    }
      } else {
        if (code_type) *code_type=LIS_CODTYP_FUNCTION;
      }
	} else {
      if (num_extra_args>0) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s  Code type is Chunk for record %s, popping extra args!\n",now_str,LIS_LIB_LOG_NAME,name);
        lua_pop(L,num_extra_args);
        num_extra_args=0;
	  }
      is_chunk=TRUE;
      pushed_code=lisPushChunk(L,name,code2run);
      if ((pushed_code==1)&&(code_type)) *code_type=LIS_CODTYP_CHUNK;
	}
  }

  if (pushed_code==-1) {
    if (code_type) *code_type=LIS_CODTYP_ERROR;
    lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Unable to process record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }

  /* Pop record table if chunk or yielded */
  pushed_rec_arg=lisPrepareLuaEnvironment(L,name,priv->recordRuntime->isYielded | is_chunk,num_extra_args);
  if (pushed_rec_arg==-1) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Error preparing processing environment for record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    lua_pop(L,pushed_code+num_extra_args);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }

  stack_base=lua_gettop(L)-pushed_code-pushed_rec_arg-num_extra_args;
  resume_res=lua_resume(L,NULL,pushed_rec_arg+num_extra_args,&resumed_args);
  if ((resume_res==LUA_YIELD)||(resume_res==LUA_OK)) {
    priv->recordRuntime->isYielded=(resume_res==LUA_YIELD);

    res=lisParseProcMethodResult(L,name,stack_base,&callback_found,&num_cb_params);
    if (callback_found) {
       if (precord->pact==0) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Warning: forcing asynchronous processing of record %s.PACT to 1\n",now_str,LIS_LIB_LOG_NAME,name);
        precord->pact=1;
      }
      lisMoveArgsExchange(L,priv->recordRuntime->callbackStack,"processing","callback",num_cb_params+1);
      callbackRequest(&priv->recordRuntime->asyncCallback);
	}

    if (resume_res==LUA_YIELD) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Processing yielded for record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    } else if (resume_res==LUA_OK) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Processing finished for record %s\n",now_str,LIS_LIB_LOG_NAME,name);
    }

  } else { /* Something went wrong! */
    priv->recordRuntime->isYielded=FALSE;
    error_msg=lua_tostring(L,-1);
    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s %s\n",now_str,LIS_LIB_LOG_NAME,error_msg);
    lua_pop(L,1);

    lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->stackReference); /* Must re-create execution stack...*/
    priv->recordRuntime->coroutStack=lisCreateStack(priv->lisState->masterState,&priv->recordRuntime->stackReference);
    if (priv->recordRuntime->coroutStack==NULL) {
      lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Not enough memory to re-create stack for record '%s'\n",now_str,LIS_LIB_LOG_NAME,name);
    }

    lisLog(LIS_LOGLVL_STANDARD,errlogFatal,now_str,"%s %s Error processing record %s, re-creating stack!\n",now_str,LIS_LIB_LOG_NAME,name);
    res=S_db_BadSub;
  }

  if ((!callback_found)&&(precord->pact!=0)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Warning: PACT is left at %d after synchronous processing of record %s\n",now_str,LIS_LIB_LOG_NAME,precord->pact,name);
  }

  lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

  return res;
}

/* int after: 0=before_field_update 1=after_field_update */
/* Called when one of LINR,EGUF,EGUL,ROFF is modified */
long lisSpecialLinConv(struct dbCommon *precord,int after) {
  NOW_STR;
  struct lisRecordState *priv;
  int res=LIS_EPICS_SUCCESS,pcall_res,stack_top,num_ret_args;
  lua_State *L;

  priv=precord->dpvt;
  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Call to %s() on uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_LINCONV_FUN,precord->name);
    res=S_db_errArg;
  } else {
    lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    L=priv->recordRuntime->coroutStack;
    stack_top=lua_gettop(L);

    if (lisPushMethod(L,precord->name,priv->recordConfig->className,LIS_LINCONV_FUN,0)!=1) {
      res=S_db_BadSub;
	} else {
      if (lisPrepareLuaEnvironment(L,precord->name,FALSE,0)!=1) {
        res=S_db_BadSub;
	  } else {
        lua_pushinteger(L,after);
        pcall_res=lua_pcall(L,2,LUA_MULTRET,0);
        if (pcall_res==0) {
          num_ret_args=lua_gettop(L)-stack_top;
          if (num_ret_args>0) {
            if (lua_isinteger(L,-num_ret_args)) {
              res=lua_tointeger(L,-num_ret_args);
              if (num_ret_args>1) {
                lisLog(LIS_LOGLVL_VERBOSE,errlogMinor,now_str,"%s %s Too many return parameters by %s() of record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_LINCONV_FUN,precord->name);
              }
            } else {
              lisLog(LIS_LOGLVL_VERBOSE,errlogMajor,now_str,"%s %s Wrong type returned by %s() of record %s (must be a single integer)\n",now_str,LIS_LIB_LOG_NAME,LIS_LINCONV_FUN,precord->name);
              res=S_db_errArg;
            }
		  }
		} else {
          lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Failed calling %s() of record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_LINCONV_FUN,precord->name);
          res=S_db_BadSub;
		}
	  }
	}

    lua_settop(L,stack_top);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
  }

  return res;
}

long lisGetIoIntInfo(int cmd,struct dbCommon *precord,IOSCANPVT *ppvt) {
  NOW_STR;
  int res=LIS_EPICS_SUCCESS,pcall_res,stack_top,num_ret_args;
  struct lisRecordState *priv;
  lua_State *L;
  const char *ioscan_id;
  struct lisIOIntrList *iointr;

  *ppvt=NULL;

  priv=precord->dpvt;
  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Call to %s() on uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_GET_IOINT_FUN,precord->name);
    res=S_db_errArg;
  } else {
    lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    L=priv->recordRuntime->coroutStack;
    stack_top=lua_gettop(L);

    if (cmd==0) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Adding record %s to I/O scan event list\n",now_str,LIS_LIB_LOG_NAME,precord->name);
    } else {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Removing record %s from I/O scan event list\n",now_str,LIS_LIB_LOG_NAME,precord->name);
    }

    if (lisPushMethod(L,precord->name,priv->recordConfig->className,LIS_GET_IOINT_FUN,0)!=1) {
      res=S_db_BadSub;
	} else {
      if (lisPrepareLuaEnvironment(L,precord->name,FALSE,0)!=1) {
        res=S_db_BadSub;
	  } else {
        lua_pushinteger(L,cmd);
        pcall_res=lua_pcall(L,2,LUA_MULTRET,0);
        if (pcall_res==0) {
          num_ret_args=lua_gettop(L)-stack_top;
          if ((num_ret_args>0)&&(lua_isstring(L,-num_ret_args))) {
            if (num_ret_args>1) {
              lisLog(LIS_LOGLVL_VERBOSE,errlogMinor,now_str,"%s %s Too many return parameters by %s() of record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_GET_IOINT_FUN,precord->name);
            }
            ioscan_id=lua_tostring(L,-num_ret_args);

            if (cmd==0) { /* SCAN is being set to "I/O Intr" */
              iointr=lisGetIoIntr(ioscan_id);
              if (iointr!=NULL) {
                lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Registering record %s in I/O scan id=%s\n",now_str,LIS_LIB_LOG_NAME,precord->name,ioscan_id);
                lisIoIntrRegisterRec(priv,iointr);
                *ppvt=*(iointr->ioScan);
              } else {
                lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s I/O scan id=%s not found for record %s\n",now_str,LIS_LIB_LOG_NAME,ioscan_id,precord->name);
                res=S_db_errArg;
              }

            } else { /* SCAN is being unset from "I/O Intr" */
              iointr=lisIoIntrUnregisterRec(priv);
              if (iointr) {
                lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Unregistering record %s from I/O scan id=%s\n",now_str,LIS_LIB_LOG_NAME,precord->name,iointr->ioIntrId);
                *ppvt=*(iointr->ioScan);
              } else {
                res=S_db_errArg;
              }
            }

		  } else {
            lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong arguments returned by %s() of record %s (must be a single string)\n",now_str,LIS_LIB_LOG_NAME,LIS_GET_IOINT_FUN,precord->name);
            res=S_db_errArg;
		  }
		} else {
          lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Failed calling %s() of record %s\n",now_str,LIS_LIB_LOG_NAME,LIS_GET_IOINT_FUN,precord->name);
          res=S_db_BadSub;
		}
	  }
	}

    lua_settop(L,stack_top);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
  }

  return res;
}

long lisLegacyInitRecord(struct dbCommon *precord) {
  NOW_STR;
  struct lisRecordState *priv;
  int res=LIS_EPICS_SUCCESS;

  priv=precord->dpvt;
  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Call to init() on uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);
    res=S_db_errArg;
  } else {
    lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Initializing record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);
    res=lisPrepLuaAndPCall(priv->recordRuntime->coroutStack,precord->name,priv->recordConfig->className,LIS_INIT_REC_FUN,LIS_ALLOW_FUNCTION_ONLY);
    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
  }

  return res;
}
