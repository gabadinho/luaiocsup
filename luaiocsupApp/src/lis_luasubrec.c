#define GEN_SIZE_OFFSET
#include "lis_luasubrec.h"
#undef  GEN_SIZE_OFFSET

#include "lis_globals.h"
#include "lis_log.h"
#include "lis_string_manip.h"
#include "lis_common_support.h"
#include "lis_epics.h"
#include "lis_lua.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dbEvent.h>
#include <dbAccess.h>
#include <recGbl.h>
#include <recSup.h>
#include <devSup.h>
#include <epicsExport.h>

typedef long (*GENFUNCPTR)(struct luasubRecord *);

static long luasubInitRecord(luasubRecord *,int);
static long luasubProcess(luasubRecord *);
static long luasubSpecial(DBADDR *,int);
static long luasubCvtDbAddr(DBADDR *);
static long luasubGetArrayInfo(DBADDR *,long *,long *);
static long luasubPutArrayInfo(DBADDR *,long);
static long luasubGetPrecision(DBADDR *,long *);
rset luasubRSET = {
  RSETNUMBER,
  NULL,
  NULL,
  luasubInitRecord,
  luasubProcess,
  luasubSpecial,
  NULL,
  luasubCvtDbAddr,
  luasubGetArrayInfo,
  luasubPutArrayInfo,
  NULL,
  luasubGetPrecision,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};
epicsExportAddress(rset,luasubRSET);

static long luasubReport(int);
static long luasubInitDevSup(int);
dset lisLUASUB = {
  4, /* Space for 4 functions */
  luasubReport,
  luasubInitDevSup,
  NULL,
  lisGetIoIntInfo
};
epicsExportAddress(dset,lisLUASUB);

static long luasubAddRecord(struct dbCommon *);
static long luasubDelRecord(struct dbCommon *);
static struct dsxt lisDsxtLUASUB = {
  luasubAddRecord,
  luasubDelRecord
};

#define NUM_IO_FIELDS 26

static const char *luasubInpValFields[] = {
  "A", "B", "C", "D", "E", "F", "G",
  "H", "I", "J", "K", "L", "M", "N",
  "O", "P", "Q", "R", "S", "T", "U",
  "V", "W", "X", "Y", "Z"
};

static const char *luasubOutValFields[] = {
  "VALA", "VALB", "VALC", "VALD", "VALE", "VALF", "VALG",
  "VALH", "VALI", "VALJ", "VALK", "VALL", "VALM", "VALN",
  "VALO", "VALP", "VALQ", "VALR", "VALS", "VALT", "VALU",
  "VALV", "VALW", "VALX", "VALY", "VALZ"
};

/* Clear all output-valid flags */
static inline void luasubClearValidOutputFields(luasubRecord *prec) {
  memset(&(prec->vlda),0,NUM_IO_FIELDS*sizeof(epicsEnum16));
}

static void luasubInitOldFields(luasubRecord *prec) {
  int i;

  strcpy(prec->ocod,prec->pcod);
  prec->oval=prec->val;
  for (i=0; i<NUM_IO_FIELDS; i++) {
    epicsUInt32 nev=(&prec->neva)[i];
    (&prec->onva)[i]=nev;
    if (nev) memcpy((&prec->ovla)[i],(&prec->vala)[i],dbValueSize((&prec->ftva)[i])*nev);
  }
}

/* Beware: return in the middle of the function! */
static int luasubInitValFields(int num_fields,epicsEnum16 *pft,epicsUInt32 *pno,epicsUInt32 *pne,epicsUInt32 *pon,const char **fldnames,void **pval,void **povl) {
  int i;
  epicsUInt32 num,flen;

  for (i=0; i<num_fields; i++,pft++,pno++,pne++,pval++) {
    if (*pft>DBF_ENUM) *pft=DBF_CHAR;
    if (*pno==0) *pno=1;

    flen=dbValueSize(*pft);
    num=*pno*flen;
    *pval=calloc(*pno,flen);
    if (pval==NULL) {
      lisLogSimple("%s Failed to calloc() inside luasubInitValFields[pval]\n",LIS_LIB_LOG_NAME);
      return FALSE; /* Beware: return in the middle of the function! */
    }

    *pne=*pno;
    if (povl) {
      if (num) {
        *povl=calloc(*pno,flen);
        if (povl==NULL) {
          lisLogSimple("%s Failed to calloc() inside luasubInitValFields[povl]\n",LIS_LIB_LOG_NAME);
          return FALSE; /* Beware: return in the middle of the function! */
        }

	  }
      povl++;
      *pon++=*pne;
    }
  }

  return TRUE;
}

static int luasubInitInpOutFields(char *rec_name,int num_fields,struct link *pinp,epicsUInt32 *pno,epicsEnum16 *pft,void **pa,struct link *pout) {
  NOW_STR;
  int i,res=TRUE;

  for (i=0; i<num_fields; i++,pinp++,pout++,pno++,pft++,pa++) {
    switch (pinp->type) {
      case CONSTANT:
        if (*pno<2) recGblInitConstantLink(pinp,*pft,*pa);
        break;
      case PV_LINK:
      case CA_LINK:
      case DB_LINK:
        break;
      default:
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong input link %d type %d of record %s\n",now_str,LIS_LIB_LOG_NAME,i,pinp->type,rec_name);
        res=FALSE;
    }

    switch (pout->type) {
      case CONSTANT:
      case PV_LINK:
      case CA_LINK:
      case DB_LINK:
        break;
      default:
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Wrong output link %d type %d of record %s\n",now_str,LIS_LIB_LOG_NAME,i,pout->type,rec_name);
        res=FALSE;
    }
  }

  return res;
}

/* Beware: return in the middle of the function! */
static long luasubFetchInputValues(luasubRecord *prec) {
  long status,n_request;
  int i;

  for (i=0; i<NUM_IO_FIELDS; i++) {
    n_request=(&prec->noa)[i];
    status=dbGetLink(&(&prec->inpa)[i],(&prec->fta)[i],(&prec->a)[i],0,&n_request);
    if (n_request>0) (&prec->nea)[i]=n_request;
    if (status) return status; /* Beware: return in the middle of the function! */
  }

  return LIS_EPICS_SUCCESS;
}

static void luasubPushOutputValues(luasubRecord *prec,short output_updated[]) {
  NOW_STR;
  int i;

  switch (prec->drvo) {
    case luasubDRVO_NEVER:
      break;

    case luasubDRVO_ON_CHANGE:
      for (i=0; i<NUM_IO_FIELDS; i++) {
        void *povl=(&prec->ovla)[i];
        void *pval=(&prec->vala)[i];
        epicsUInt32 *ponv=&(&prec->onva)[i];
        epicsUInt32 *pnev=&(&prec->neva)[i];
        epicsUInt32 onv=*ponv;
        epicsUInt32 nev=*pnev;
        epicsUInt32 alen=dbValueSize((&prec->ftva)[i])*nev;
        if (nev!=onv || memcmp(povl,pval,alen)) {
          lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushing changed output-field VAL%c of %s record %s\n",now_str,LIS_LIB_LOG_NAME,65+i,LIS_LUASUB_REC_NAME,prec->name);
          dbPutLink(&(&prec->outa)[i],(&prec->ftva)[i],(&prec->vala)[i],(&prec->neva)[i]);
          output_updated[i]=1;
        }
      }
      break;

    case luasubDRVO_ALWAYS:
      for (i=0; i<NUM_IO_FIELDS; i++) {
        dbPutLink(&(&prec->outa)[i],(&prec->ftva)[i],(&prec->vala)[i],(&prec->neva)[i]);
      }
      break;

    case luasubDRVO_ON_VALID:
      for (i=0; i<NUM_IO_FIELDS; i++) {
        if ((&prec->vlda)[i]) {
          lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Pushing valid output-field VAL%c of %s record %s\n",now_str,LIS_LIB_LOG_NAME,65+i,LIS_LUASUB_REC_NAME,prec->name);
          dbPutLink(&(&prec->outa)[i],(&prec->ftva)[i],(&prec->vala)[i],(&prec->neva)[i]);
          output_updated[i]=1;
        }
	  }
      break;
  }
}

static void luasubDriveMonitors(luasubRecord *prec,short output_updated[]) {
  int i;
  unsigned short monitor_mask=recGblResetAlarms(prec)|DBE_VALUE|DBE_LOG;

  /* Post events for VAL field */
  if (prec->val!=prec->oval) {
    db_post_events(prec,&prec->val,monitor_mask);
    prec->oval=prec->val;
  }

  /* Post events for VAL? and NEV? fields */
  switch (prec->drvo) {
    case luasubDRVO_NEVER:
      break;

    case luasubDRVO_ON_CHANGE:
      for (i=0; i<NUM_IO_FIELDS; i++) {
		if (output_updated[i]) {
          void *povl=(&prec->ovla)[i];
          void *pval=(&prec->vala)[i];
          epicsUInt32 *ponv=&(&prec->onva)[i];
          epicsUInt32 *pnev=&(&prec->neva)[i];
          epicsUInt32 nev=*pnev;
          epicsUInt32 alen=dbValueSize((&prec->ftva)[i])*nev;

          memcpy(povl,pval,alen);
          db_post_events(prec,pval,monitor_mask);

          *ponv=nev;
          db_post_events(prec,pnev,monitor_mask);
        }
      }
      break;

    case luasubDRVO_ALWAYS:
      for (i=0; i<NUM_IO_FIELDS; i++) {
        db_post_events(prec,(&prec->vala)[i],monitor_mask);
        db_post_events(prec,&(&prec->neva)[i],monitor_mask);
      }
      break;

    case luasubDRVO_ON_VALID:
      for (i=0; i<NUM_IO_FIELDS; i++) {
		if (output_updated[i]) {
          void *povl=(&prec->ovla)[i];
          void *pval=(&prec->vala)[i];
          epicsUInt32 *ponv=&(&prec->onva)[i];
          epicsUInt32 *pnev=&(&prec->neva)[i];
          epicsUInt32 nev=*pnev;
          epicsUInt32 alen=dbValueSize((&prec->ftva)[i])*nev;

	      memcpy(povl,pval,alen);
	      db_post_events(prec,pval,monitor_mask);

		  *ponv=nev;
		  db_post_events(prec,pnev,monitor_mask);
	    }
      }
      break;
  }
}

static long luasubInitRecord(luasubRecord *prec,int pass) {
  NOW_STR;
  long res=LIS_EPICS_SUCCESS;
  struct lisRecordState *priv=prec->dpvt;

  if (pass==1) {
    if (priv==NULL) {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Call to init() on uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
      res=S_db_errArg;
    } else if ((prec->icod!=NULL)&&(strlen(prec->icod)>0)) {
        lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Initializing record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
        res=lisPrepLuaAndPCall(priv->recordRuntime->coroutStack,prec->name,priv->recordConfig->className,prec->icod,prec->actp);
        lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
    }
  }

  return res;
}

static long luasubProcess(luasubRecord *prec) {
  int code_type,pact=prec->pact;
  long res=LIS_EPICS_SUCCESS;
  struct lisRecordState *priv=prec->dpvt;
  short output_updated[NUM_IO_FIELDS];

  if (!pact) {
    prec->pact=TRUE;
    res=luasubFetchInputValues(prec);
    prec->pact=FALSE;
  }

  if ((res==LIS_EPICS_SUCCESS)&&(prec->pcod[0]!=0)) {
    res=lisProcess((struct dbCommon *)prec,prec->actp,&code_type);
    if (res==LIS_EPICS_SUCCESS) {
      prec->udf=FALSE;
    }
    prec->val=res;
    if (priv) { /* Update is-yielded field */
      prec->yldd=priv->recordRuntime->isYielded;
    }
    if (code_type) prec->pctp=code_type;
  }

  if (!pact && prec->pact) return LIS_EPICS_SUCCESS;
  prec->pact=TRUE;

  memset(output_updated,0,sizeof output_updated);
  if (res==LIS_EPICS_SUCCESS) {
    luasubPushOutputValues(prec,output_updated);
  }
  recGblGetTimeStamp(prec);
  luasubDriveMonitors(prec,output_updated);
  luasubClearValidOutputFields(prec);
  recGblFwdLink(prec);
  prec->pact=FALSE;

  return res;
}

/* Beware: return in the middle of the function! */
static long luasubSpecial(DBADDR *paddr,int after) {
  NOW_STR;
  luasubRecord *prec=(luasubRecord *)paddr->precord;
  int field_index=dbGetFieldIndex(paddr);
  struct lisRecordState *priv=prec->dpvt;

  if (priv==NULL) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Call to special() on uninitialized record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }

  if (field_index==luasubRecordPCOD) {
    if (!after) {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Changing process routine of record %s (old: %s)\n",now_str,LIS_LIB_LOG_NAME,prec->name,prec->pcod);
      strcpy(prec->ocod,prec->pcod);
    } else {
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Changing process routine of record %s (new: %s)\n",now_str,LIS_LIB_LOG_NAME,prec->name,prec->pcod);
	}

    lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

	if ((after)&&(prec->yldd)) {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Record %s is yielded, re-creating execution stack!\n",now_str,LIS_LIB_LOG_NAME,prec->name);
      lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->stackReference);
      priv->recordRuntime->coroutStack=lisCreateStack(priv->lisState->masterState,&priv->recordRuntime->stackReference);
      priv->recordRuntime->isYielded=FALSE;
      prec->yldd=FALSE;
    } else if ((after)&&(lua_gettop(priv->recordRuntime->coroutStack)>0)) {
  	  lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Record %s execution stack is not empty, re-creating it!\n",now_str,LIS_LIB_LOG_NAME,prec->name);
      lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->stackReference);
      priv->recordRuntime->coroutStack=lisCreateStack(priv->lisState->masterState,&priv->recordRuntime->stackReference);
	}

    if (after) {
      prec->udf=TRUE;
      prec->pctp=luasubPCTP_UNKNOWN;
	}

    lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

  } else if (field_index==luasubRecordRST) {
    if (after) {
      lisMutexLock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);

      if (prec->yldd) {
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Yielded record %s, re-creating execution stack\n",now_str,LIS_LIB_LOG_NAME,prec->name);
        lisDisposeStack(priv->lisState->masterState,priv->recordRuntime->stackReference);
        priv->recordRuntime->coroutStack=lisCreateStack(priv->lisState->masterState,&priv->recordRuntime->stackReference);
        priv->recordRuntime->isYielded=FALSE;
        prec->yldd=FALSE;
      } else if (lua_gettop(priv->recordRuntime->coroutStack)>0) {
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Flushing execution stack for record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
  	    lua_settop(priv->recordRuntime->coroutStack,0);
	  } else {
        lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s Nothing to reset for record %s\n",now_str,LIS_LIB_LOG_NAME,prec->name);
	  }
      prec->rst=0;

      lisMutexUnlock2(priv->lisState->stateLock,priv->recordRuntime->stackLock);
	}

  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s Unsupported special field %d of record %s\n",now_str,LIS_LIB_LOG_NAME,field_index,prec->name);
  }

  return LIS_EPICS_SUCCESS;
}

static long luasubCvtDbAddr(DBADDR *paddr) {
  NOW_STR;
  luasubRecord *prec=(luasubRecord *)paddr->precord;
  int field_index=dbGetFieldIndex(paddr);

  if (field_index>=luasubRecordA && field_index<=luasubRecordZ) {
    int offset=field_index-luasubRecordA;

    paddr->pfield     =(&prec->a  )[offset];
    paddr->no_elements=(&prec->noa)[offset];
    paddr->field_type =(&prec->fta)[offset];

    paddr->dbr_field_type=paddr->field_type;
    paddr->field_size    =dbValueSize(paddr->field_type);

  } else if (field_index>=luasubRecordVALA && field_index<=luasubRecordVALZ) {
    int offset=field_index-luasubRecordVALA;

    paddr->pfield     =(&prec->vala)[offset];
    paddr->no_elements=(&prec->nova)[offset];
    paddr->field_type =(&prec->ftva)[offset];

    paddr->dbr_field_type=paddr->field_type;
    paddr->field_size    =dbValueSize(paddr->field_type);

  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Calling cvt_dbaddr for unsupported field %s.%s\n",now_str,LIS_LIB_LOG_NAME,prec->name,paddr->pfldDes->name);
  }

  return LIS_EPICS_SUCCESS;
}

static long luasubGetArrayInfo(DBADDR *paddr,long *no_elements,long *offset) {
  NOW_STR;
  luasubRecord *prec=(luasubRecord *)paddr->precord;
  int field_index=dbGetFieldIndex(paddr);

  if (field_index>=luasubRecordA && field_index<=luasubRecordZ) {
    *no_elements=(&prec->nea)[field_index-luasubRecordA];
    *offset=0;
  } else if (field_index>=luasubRecordVALA && field_index<=luasubRecordVALZ) {
    *no_elements=(&prec->neva)[field_index-luasubRecordVALA];
    *offset=0;
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Calling get_array_info for unsupported field %s.%s\n",now_str,LIS_LIB_LOG_NAME,prec->name,paddr->pfldDes->name);
  }

  return LIS_EPICS_SUCCESS;
}

static long luasubPutArrayInfo(DBADDR *paddr, long n_new) {
  NOW_STR;
  luasubRecord *prec=(luasubRecord *)paddr->precord;
  int field_index=dbGetFieldIndex(paddr);

  if (field_index>=luasubRecordA && field_index<=luasubRecordZ) {
    (&prec->nea)[field_index-luasubRecordA]=n_new;
  } else if (field_index>=luasubRecordVALA && field_index<=luasubRecordVALZ) {
    (&prec->neva)[field_index-luasubRecordVALA]=n_new;
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMinor,now_str,"%s %s Calling put_array_info for unsupported field %s.%s\n",now_str,LIS_LIB_LOG_NAME,prec->name,paddr->pfldDes->name);
  }

  return LIS_EPICS_SUCCESS;
}

static long luasubGetPrecision(DBADDR *paddr,long *precision) {
  luasubRecord *prec=(luasubRecord *)paddr->precord;
  *precision=prec->prec;
  recGblGetPrec(paddr,precision);
  return LIS_EPICS_SUCCESS;
}

static long luasubReport(int interest) {
  lisLogSimple("  Lua IOC Support scripts directory: %s\n",lisBaseDirectory);
  return lisReport(interest,LIS_LUASUB_REC_NAME);
}

static long luasubInitDevSup(int phase) {
  NOW_STR;
  if(phase==0) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Initializing %s support\n",now_str,LIS_LIB_LOG_NAME,LIS_LUASUB_REC_NAME);
    devExtend(&lisDsxtLUASUB);
  } else {
    lisDevSupPhase1Init();
  }
  return LIS_EPICS_SUCCESS;
}

/* Beware: return in the middle of the function! */
static long luasubAddRecord(struct dbCommon *precord) {
  NOW_STR;
  luasubRecord *prec=(luasubRecord *)precord;

  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Adding record %s\n",now_str,LIS_LIB_LOG_NAME,precord->name);

  if (luasubInitValFields(NUM_IO_FIELDS,&prec->fta,&prec->noa,&prec->nea,NULL,luasubInpValFields,&prec->a,NULL)==FALSE) {
    return S_db_noMemory;
  }
  if (luasubInitValFields(NUM_IO_FIELDS,&prec->ftva,&prec->nova,&prec->neva,&prec->onva,luasubOutValFields,&prec->vala,&prec->ovla)==FALSE) {
    return S_db_noMemory;
  }  
  if (luasubInitInpOutFields(precord->name,NUM_IO_FIELDS,&prec->inpa,&prec->noa,&prec->fta,&prec->a,&prec->outa)==FALSE) {
    return S_db_badField;
  }
  luasubInitOldFields(prec);
  luasubClearValidOutputFields(prec);

  return lisGenerateRecord((struct dbCommon *)prec,LIS_INP_FIELD_NAME,NULL,prec->pcod,lisIsIocRunning);
}

static long luasubDelRecord(struct dbCommon *precord) {
  return lisDeleteRecord(precord);
}


