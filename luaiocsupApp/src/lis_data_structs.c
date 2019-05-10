#include "lis_data_structs.h"

#include "lis_string_manip.h"
#include "lis_log.h"
#include "lis_lua.h"

#include <string.h>
#include <stdlib.h>

#include <lualib.h>
#include <lauxlib.h>

static struct lisRecordList *lisAllRecords = NULL;

static struct lisLuaStateList *lisLuaStates = NULL;

static struct lisIOIntrList *lisIOInterrupts = NULL;

static struct lisPutLinkList *lisPutLinks = NULL;

struct lisAtExitList *lisAtExits = NULL;

int lisIsFileLoaded(struct lisFilenameList *loaded_files,char *filename) {
  struct lisFilenameList *filename_iter;

  filename_iter=loaded_files;
  while (filename_iter!=NULL) {
    if (!strcmp(filename_iter->filename,filename)) return TRUE;
    filename_iter=filename_iter->next;
  }
  return FALSE;
}

struct lisFilenameList * lisAddToLoadedFiles(struct lisFilenameList *loaded_files,char *filename,int *was_new_file) {
  struct lisFilenameList *filename_iter;
  int found_file=0;

  filename_iter=loaded_files;
  while (filename_iter!=NULL) {
    if (!strcmp(filename_iter->filename,filename)) {
      found_file=1;
      break;
    }
    filename_iter=filename_iter->next;
  }
  if (!found_file) {
    filename_iter=malloc(sizeof(struct lisFilenameList));
    if (filename_iter==NULL) {
      lisLogSimple("%s Failed to malloc() inside lisAddToLoadedFiles()\n",LIS_LIB_LOG_NAME);
    } else {
      filename_iter->next=loaded_files;
      filename_iter->filename=filename;
      loaded_files=filename_iter;
    }
  }

  *was_new_file=1-found_file;
  return loaded_files;
}

static struct lisLuaState * lisNewLuaState(char *state_id) {
  struct lisLuaState *lua_st;
  epicsMutexId state_mutex;

  lua_st=malloc(sizeof(struct lisLuaState));
  if (lua_st==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisNewLuaState()\n",LIS_LIB_LOG_NAME);
  } else {
    state_mutex=lisMutexCreate();
    if (state_mutex==NULL) {
      lisLogSimple("%s Failed to create mutex inside lisNewLuaState()\n",LIS_LIB_LOG_NAME);
      free(lua_st);
    } else {
      lua_st->masterState=lisCreateLuaState();
      if (lua_st->masterState==NULL) {
        lisLogSimple("%s Failed to create Lua state inside lisNewLuaState()\n",LIS_LIB_LOG_NAME);
        lisMutexDestroy(state_mutex);
        free(lua_st);
        lua_st=NULL;
      } else {
        lua_st->stateId=state_id;
        lua_st->loadedFiles=NULL;
        lisMutexLock(state_mutex);
        lua_st->stateLock=state_mutex;
	  }
    }
  }

  return lua_st;
}

struct lisLuaState * lisFetchLuaState(char *state_id,char *record_name,char *record_type,int *new_state_created) {
  struct lisLuaState *lua_st=NULL;
  struct lisLuaStateList *lua_st_l=NULL,*state_iter; 
  NOW_STR;

  *new_state_created=FALSE;

  if ((state_id==NULL)||(strlen(state_id)==0)) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Creating anonymous state for %s record %s\n",now_str,LIS_LIB_LOG_NAME,record_type,record_name);

    lua_st_l=malloc(sizeof(struct lisLuaStateList));
    if (lua_st_l==NULL) {
      lisLogSimple("%s Failed to malloc() inside lisFetchLuaState(,%s)\n",LIS_LIB_LOG_NAME,record_name);
    } else { 
      lua_st=lisNewLuaState(NULL);
      if (lua_st!=NULL) {
        *new_state_created=TRUE;
        lua_st_l->lisState=lua_st;
        lua_st_l->next=lisLuaStates;
        lisLuaStates=lua_st_l;
      } else {
        free(lua_st_l);
      }
    }

  } else {
    if (lisLuaStates==NULL) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Creating state id=%s for %s record %s\n",now_str,LIS_LIB_LOG_NAME,state_id,record_type,record_name);
 
      lua_st_l=malloc(sizeof(struct lisLuaStateList));
      if (lua_st_l==NULL) {
        lisLogSimple("%s Failed to malloc() inside lisFetchLuaState(%s,%s)\n",LIS_LIB_LOG_NAME,state_id,record_name);
      } else {
        lua_st=lisNewLuaState(state_id);
        if (lua_st!=NULL) {
          *new_state_created=TRUE;
          lua_st_l->lisState=lua_st;
          lua_st_l->next=lisLuaStates;
          lisLuaStates=lua_st_l;
        } else {
          free(lua_st_l);
        }
      }
    } else {
      state_iter=lisLuaStates;
      while (state_iter!=NULL) {
        if (state_iter->lisState->stateId!=NULL) {
          if (!strcmp(state_id,state_iter->lisState->stateId)) {
            lua_st=state_iter->lisState;
            break;
          }
        }
        state_iter=state_iter->next;
      }

      if (lua_st==NULL) {
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Creating state id=%s for %s record %s\n",now_str,LIS_LIB_LOG_NAME,state_id,record_type,record_name);

        lua_st_l=malloc(sizeof(struct lisLuaStateList));
        if (lua_st_l==NULL) {
          lisLogSimple("%s Failed to malloc() inside lisFetchLuaState(%s,%s)\n",LIS_LIB_LOG_NAME,state_id,record_name);
        } else {
          lua_st=lisNewLuaState(state_id);
          if (lua_st!=NULL) {
            *new_state_created=TRUE;
            lua_st_l->lisState=lua_st;
            lua_st_l->next=lisLuaStates;
            lisLuaStates=lua_st_l;
          } else {
            free(lua_st_l);
          }
        }

      } else {
        lisMutexLock(lua_st->stateLock);
        lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Reusing state id=%s for %s record %s\n",now_str,LIS_LIB_LOG_NAME,state_id,record_type,record_name);
      }
    }
  }

  return lua_st;
}

static void lisDeleteLuaStateFiles(struct lisFilenameList *fl) {
  struct lisFilenameList *f_next;
  while (fl) {
    f_next=fl->next;
    free(fl);
    fl=f_next;
  }
}

void lisDeleteLuaState(struct lisLuaState *lua_st) {
  NOW_STR;
  struct lisLuaStateList *st_curr=lisLuaStates,*st_prev=NULL;

  if (lua_st->stateId==NULL) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Deleting anonymous Lua state %p\n",now_str,LIS_LIB_LOG_NAME,lua_st);
  } else {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Deleting Lua state id=%s\n",now_str,LIS_LIB_LOG_NAME,lua_st->stateId);
  }

  while (st_curr) {
    if (st_curr->lisState==lua_st) {
      lisDisposeLuaState(lua_st->masterState);
      lisMutexUnlock(lua_st->stateLock);
      lisMutexDestroy(lua_st->stateLock);
      lisDeleteLuaStateFiles(lua_st->loadedFiles);

      if (st_prev==NULL) {
        lisLuaStates=st_curr->next;
      } else {
        st_prev->next=st_curr->next;
      }
	  break;
	} else {
      st_prev=st_curr;
      st_curr=st_curr->next;
	}
  }
  if (st_curr==NULL) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unable to find Lua state!\n",now_str,LIS_LIB_LOG_NAME);
  } else {
    free(lua_st);
  }
}

struct lisLuaStateList * lisGetLuaStates() {
  return lisLuaStates;
}

struct lisRecordRuntime * lisCreateRecordRuntime(char *code2run) {
  struct lisRecordRuntime *rec_rt;

  rec_rt=malloc(sizeof(struct lisRecordRuntime));
  if (rec_rt==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisCreateRecordRuntime()\n",LIS_LIB_LOG_NAME);
  } else {
    rec_rt->stackLock=lisMutexCreate();
    if (rec_rt->stackLock==NULL) {
      free(rec_rt);
      rec_rt=NULL;
	} else {
      rec_rt->luaCode2Run=code2run;
      rec_rt->coroutStack=NULL;
      rec_rt->stackReference=-1; /* Somewhat ugly replacement for not-initialized */

      rec_rt->callbackStack=NULL;
      rec_rt->cbStackReference=-1; /* Somewhat ugly replacement for not-initialized */

      rec_rt->isYielded=FALSE;
      rec_rt->ioIntr=NULL;
      rec_rt->markedForDeletion=FALSE;
      lisMutexLock(rec_rt->stackLock);
    }
  }

  return rec_rt;
}

void lisDeleteRecordRuntime(struct lisRecordRuntime *rec_rt) {
  lisMutexUnlock(rec_rt->stackLock);
  lisMutexDestroy(rec_rt->stackLock);
  free(rec_rt);
}

struct lisRecordList * lisAddRecordToSupport(struct lisRecordState *rec_state) {
  struct lisRecordList *new_rec;

  new_rec=malloc(sizeof(struct lisRecordList));
  if (new_rec==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisAddRecordToSupport(%s)\n",LIS_LIB_LOG_NAME,rec_state->recordName);
  } else {
    new_rec->recordState=rec_state;

    new_rec->next=lisAllRecords;
    lisAllRecords=new_rec;
  }

  return new_rec;
}

struct lisRecordList * lisGetSupportRecords() {
  return lisAllRecords;
}

struct lisRecordState * lisCreateRecordState(struct dbCommon *prec,char *rec_name,char *rec_type_name) {
  struct lisRecordState *rec_st;

  rec_st=malloc(sizeof(struct lisRecordState));
  if (rec_st==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisCreateRecordState()\n",LIS_LIB_LOG_NAME);
  } else {
    rec_st->recordName=rec_name;
    rec_st->recordTypeName=rec_type_name;

    rec_st->lisState=NULL;
    rec_st->recordConfig=NULL;
    rec_st->recordRuntime=NULL;

    rec_st->pRec=prec;
  }

  return rec_st;
}

/* Returns the master state to be disposed, or NULL if shared by other records */
struct lisLuaState * lisDeleteRecordState(struct lisRecordState *rec_state) {
  NOW_STR;
  struct lisRecordList *rec_curr=lisAllRecords,*rec_prev=NULL;
  struct lisLuaState *lis_state=NULL;

  while (rec_curr) {
    if (rec_curr->recordState==rec_state) {
      lis_state=rec_state->lisState;
      if (rec_state->recordConfig) {
        lisDeleteRecordConfig(rec_state->recordConfig);
	  }
      if (rec_state->recordRuntime) {
        lisDeleteRecordRuntime(rec_state->recordRuntime);
	  }

	  if (rec_prev==NULL) {
        lisAllRecords=rec_curr->next;
	  } else {
        rec_prev->next=rec_curr->next;
	  }
	  break;
	} else {
      rec_prev=rec_curr;
      rec_curr=rec_curr->next;
	}
  }

  if (rec_curr==NULL) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Unable to find %s record %s, marking it for deletion...\n",now_str,LIS_LIB_LOG_NAME,rec_state->recordTypeName,rec_state->recordName);
    rec_state->recordRuntime->markedForDeletion=TRUE;
  } else {
	/* Search for records sharing same Lua state */
    rec_prev=lisAllRecords;
    while (rec_prev) {
      if (rec_prev->recordState->lisState==lis_state) {
	    break;
	  }
      rec_prev=rec_prev->next;
	}
	if (rec_prev!=NULL) {
	  lis_state=NULL;
	}
    free(rec_curr);
  }

  return lis_state;
}

struct lisRecordConfig * lisCreateRecordConfig(char *db_link_field,char *rec_field_str) {
  char *script_filename=NULL,*script_parameters=NULL;
  struct lisRecordConfig *rec_cfg=NULL;
  char *str2,*param,*rest,**params=NULL;
  int num_params_found;
  int i;
  int param_id_len,param_class_len;

  param_id_len=strlen(LIS_SCRIPT_PARAM_ID);
  param_class_len=strlen(LIS_SCRIPT_PARAM_CLASS);

  if (rec_field_str!=NULL) {
    script_filename=lisFetchScriptFilename(rec_field_str);
    if (script_filename==NULL) {
      lisLogSimple("%s Unable to parse Lua script filename in lisCreateRecordConfig(%s)\n",LIS_LIB_LOG_NAME,rec_field_str);
      return NULL;
    } else {
      script_parameters=lisFetchScriptParams(rec_field_str);
    }
  } else {
    lisLogSimple("%s Null string passed to lisCreateRecordConfig()\n",LIS_LIB_LOG_NAME);
    return NULL;
  }

  rec_cfg=malloc(sizeof(struct lisRecordConfig));
  if (rec_cfg==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisCreateRecordConfig(%s)\n",LIS_LIB_LOG_NAME,rec_field_str);
    free(script_filename);
    if (script_parameters) {
      free(script_parameters);
    }
    return NULL;
  }

  rec_cfg->dbLinkFieldName=db_link_field;
  rec_cfg->configString=rec_field_str;
  rec_cfg->scriptFilename=script_filename;
  rec_cfg->className=NULL;
  rec_cfg->stateId=script_filename;
  rec_cfg->scriptParams=NULL;
  rec_cfg->paramsLen=0;
  rec_cfg->origParams=NULL;

  if (script_parameters!=NULL) {
    str2=strdup(script_parameters);
    if (str2==NULL) {
      lisLogSimple("%s Failed to strdup() inside lisCreateRecordConfig(%s)\n",LIS_LIB_LOG_NAME,rec_field_str);
      free(rec_cfg);
      free(script_filename);
      free(script_parameters);
      return NULL;
    }

    num_params_found=0;
    rest=str2;
    /*
    param=strsep(&str2," ");
    while (param!=NULL) {
      if (strlen(param)) {
        if (!strncmp(param,LIS_SCRIPT_PARAM_ID,param_id_len)) {
        } else if (!strncmp(param,LIS_SCRIPT_PARAM_CLASS,param_class_len)) {
        } else {
          num_params_found+=1;
        }
      }
      param=strsep(&str2," ");
    }
    */
    while ((param=strtok_r(rest, " ", &rest))) {
      if (strlen(param)) {
        if (!strncmp(param,LIS_SCRIPT_PARAM_ID,param_id_len)) {
        } else if (!strncmp(param,LIS_SCRIPT_PARAM_CLASS,param_class_len)) {
        } else {
          num_params_found+=1;
        }
      }
    }
    free(str2);

    params=(char**)malloc(num_params_found*sizeof(char*));
    if (params==NULL) {
      lisLogSimple("%s Failed to malloc() inside lisCreateRecordConfig(%s)\n",LIS_LIB_LOG_NAME,rec_field_str);
      free(rec_cfg);
      free(script_filename);
      free(script_parameters);
      return NULL;
    }

    str2=strdup(script_parameters);
    if (str2==NULL) {
      lisLogSimple("%s Failed to strdup() inside lisCreateRecordConfig(%s)\n",LIS_LIB_LOG_NAME,rec_field_str);
      free(rec_cfg);
      free(script_filename);
      free(script_parameters);
      free(params);
      return NULL;
    }

    i=0;
    rest=str2;
    /*
    param=strsep(&str2," ");
    while (param!=NULL) {
      if (strlen(param)) {
        if (!strncmp(param,LIS_SCRIPT_PARAM_ID,param_id_len)) {
          rec_cfg->stateId=param+param_id_len;
        } else if (!strncmp(param,LIS_SCRIPT_PARAM_CLASS,param_class_len)) {
          rec_cfg->className=param+param_class_len;
        } else {
          params[i]=param;
          i+=1;
        }
      }
      param=strsep(&str2," ");
    }
    */
    while ((param=strtok_r(rest, " ", &rest))) {
      if (strlen(param)) {
        if (!strncmp(param,LIS_SCRIPT_PARAM_ID,param_id_len)) {
          rec_cfg->stateId=param+param_id_len;
        } else if (!strncmp(param,LIS_SCRIPT_PARAM_CLASS,param_class_len)) {
          rec_cfg->className=param+param_class_len;
        } else {
          params[i]=param;
          i+=1;
        }
      }
    }
    /* Remember: do _not_ free(str2)! since param points to its sub-strings! */

    rec_cfg->scriptParams=params;
    rec_cfg->paramsLen=i;
    rec_cfg->origParams=script_parameters;

    if (lisIsDetachScript(rec_field_str)) {
      rec_cfg->stateId=NULL;
    }
  }

  return rec_cfg;
}

void lisDeleteRecordConfig(struct lisRecordConfig *rec_config) {
  if (rec_config->scriptParams) free(rec_config->scriptParams);
  if (rec_config->origParams) free (rec_config->origParams);
  /* Warning: do _not_ free shared string rec_config->scriptFilename */
  free(rec_config);
}

struct lisIOIntrList * lisFetchIoIntr(char *iointr_id,int *new_iointr_created) {
  NOW_STR;
  struct lisIOIntrList * iointr_list=lisIOInterrupts;
  IOSCANPVT *ioscan=NULL;

  if (new_iointr_created) *new_iointr_created=FALSE;

  while (iointr_list!=NULL) {
    if (!strcmp(iointr_id,iointr_list->ioIntrId)) {
      lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Re-using I/O scan id=%s\n",now_str,LIS_LIB_LOG_NAME,iointr_id);
      break;
    }
    iointr_list=iointr_list->next;
  }

  if (iointr_list==NULL) {
    iointr_list=malloc(sizeof(struct lisIOIntrList));
    ioscan=malloc(sizeof(IOSCANPVT));
    if ((iointr_list)&&(ioscan)) {
      if (new_iointr_created) *new_iointr_created=TRUE;
      iointr_list->ioIntrId=iointr_id;
      iointr_list->ioScan=ioscan;
      iointr_list->records=NULL;
      iointr_list->next=lisIOInterrupts;
      lisIOInterrupts=iointr_list;
      lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Creating I/O scan id=%s\n",now_str,LIS_LIB_LOG_NAME,iointr_id); 
    } else {
      lisLogSimple("%s Failed to malloc() inside lisFetchIoIntr(%s)\n",LIS_LIB_LOG_NAME,iointr_id);
      if (iointr_list) {
        free(iointr_list);
        iointr_list=NULL;
      }
      if (ioscan) {
        free(ioscan);
        ioscan=NULL;
      }
    }
  }

  return iointr_list;
}

struct lisIOIntrList * lisGetIoIntr(const char *iointr_id) {
  struct lisIOIntrList * iointr_list=lisIOInterrupts;

  while (iointr_list!=NULL) {
    if (!strcmp(iointr_id,iointr_list->ioIntrId)) {
      break;
    }
    iointr_list=iointr_list->next;
  }

  return iointr_list;
}

int lisIoIntrRegisterRec(struct lisRecordState *rec_state,struct lisIOIntrList *iointr) {
  NOW_STR;
  struct lisRecordList *rec_iter;

  rec_iter=iointr->records;
  while (rec_iter!=NULL) {
    if (rec_iter->recordState==rec_state) {
      break;
    }
    rec_iter=rec_iter->next;
  }
  if (rec_iter==NULL) {
    rec_iter=malloc(sizeof(struct lisRecordList));
    if (rec_iter==NULL) {
      lisLogSimple("%s Failed to malloc() inside lisIoIntrRegisterRec(%s,%s)\n",LIS_LIB_LOG_NAME,rec_state->recordName,iointr->ioIntrId);
      return FALSE;
	}
    rec_iter->recordState=rec_state;
    rec_iter->next=iointr->records;
    iointr->records=rec_iter;

    rec_state->recordRuntime->ioIntr=iointr;

  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Record %s already registered in I/O scan id=%s list\n",now_str,LIS_LIB_LOG_NAME,rec_state->recordName,iointr->ioIntrId); 
  }

  return TRUE;
}

struct lisIOIntrList * lisIoIntrUnregisterRec(struct lisRecordState *rec_state) {
  NOW_STR;
  struct lisRecordList *rec_curr,*rec_prev=NULL;
  struct lisIOIntrList *iointr=NULL;

  if (rec_state->recordRuntime->ioIntr) {
    iointr=rec_state->recordRuntime->ioIntr;
    rec_state->recordRuntime->ioIntr=NULL;

    rec_curr=iointr->records;
    while (rec_curr!=NULL) {
      if (rec_curr->recordState==rec_state) {
        if (rec_prev==NULL) {
          iointr->records=rec_curr->next;
        } else {
          rec_prev->next=rec_curr->next;
        }
        break;

      } else {
        rec_prev=rec_curr;
        rec_curr=rec_curr->next;
      }
	}
    if (rec_curr!=NULL) {
      free(rec_curr);
    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Record %s registered but not found in its I/O scan id=%s list\n",now_str,LIS_LIB_LOG_NAME,rec_state->recordName,iointr->ioIntrId); 
    }
  }

  if (iointr==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Record %s not registered in any I/O scan\n",now_str,LIS_LIB_LOG_NAME,rec_state->recordName);
  }

  return iointr;
}

/* Review: do I need a safety around this data structure...? */
int lisBindIoIntr(const char *iointr_id,IOSCANPVT *ioscan) {
  NOW_STR;
  int res=FALSE;
  char *id_copy;
  struct lisIOIntrList * iointr_list=lisIOInterrupts;

  while (iointr_list!=NULL) {
    if (!strcmp(iointr_id,iointr_list->ioIntrId)) {
      break;
    }
    iointr_list=iointr_list->next;
  }
  if (iointr_list==NULL) {
    iointr_list=malloc(sizeof(struct lisIOIntrList));
    id_copy=strdup(iointr_id);
    if ((iointr_list)&&(id_copy)) {
      iointr_list->ioIntrId=id_copy;
      iointr_list->ioScan=ioscan;
      iointr_list->records=NULL;
      iointr_list->next=lisIOInterrupts;
      lisIOInterrupts=iointr_list;
      lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Binding I/O scan id=%s to IOSCANPVT %p\n",now_str,LIS_LIB_LOG_NAME,iointr_id,ioscan);
      res=TRUE;
    } else {
      lisLogSimple("%s Failed to malloc() inside lisBindIoIntr(%s,%p)\n",LIS_LIB_LOG_NAME,iointr_id,ioscan);
      if (id_copy) {
        free(id_copy);
        id_copy=NULL;
	  }
      if (iointr_list) {
        free(iointr_list);
        iointr_list=NULL;
	  }
    }
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unable to bind IOSCANPVT %p: id %s already exists!\n",now_str,LIS_LIB_LOG_NAME,ioscan,iointr_id);
  }
  
  return res;
}

struct lisPutLinkList * lisGetPutLinks() {
  return lisPutLinks;
}

void lisAddToPutLinks(dbAddr *dbaddr,const char *new_value) {
  struct lisPutLinkList *put_link;

  put_link=malloc(sizeof(struct lisPutLinkList));
  if (put_link==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisAddToPutLinks(%s.%s,%s)\n",LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,new_value);
  } else {
    memcpy(&put_link->dbaddr,dbaddr,sizeof(dbAddr));
    put_link->new_value=strdup(new_value);
    put_link->next=lisPutLinks;
    lisPutLinks=put_link;
  }
}

void lisFreePutLinks() {
  struct lisPutLinkList *put_link=lisPutLinks,*free_link;

  while (put_link) {
    free_link=put_link;
    put_link=put_link->next;
    free(free_link->new_value);
    free(free_link);
  }

  lisPutLinks=NULL;
}

struct lisAtExitList * lisGetAtExit() {
  return lisAtExits;
}

struct lisAtExitList * lisCreateAtExit(/*struct lisRecordState *rec_state*/) {
  struct lisAtExitList *at_exit;

  at_exit=malloc(sizeof(struct lisAtExitList));
  if (at_exit==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisCreateAtExit()\n",LIS_LIB_LOG_NAME);
  } else {
    at_exit->next=lisAtExits;
    lisAtExits=at_exit;
  }

  return at_exit;
}
