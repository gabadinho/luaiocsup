#define GEN_SIZE_OFFSET
#include "lis_luasubrec.h"
#undef  GEN_SIZE_OFFSET

#include "lis_log.h"
#include "lis_common_support.h"
#include "lis_epics.h"

#include <dbCommon.h>
#include <recSup.h>
#include <devSup.h>
#include <dbAddr.h>
#include <epicsExport.h>

/*
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
*/

//typedef long (*GENFUNCPTR)(struct luasubRecord *);

static long luasubInitRecord(luasubRecord *, int);
static long luasubProcess(luasubRecord *);
static long luasubSpecial(DBADDR *, int);
static long luasubCvtDbAddr(DBADDR *);
static long luasubGetArrayInfo(DBADDR *, long *, long *);
static long luasubPutArrayInfo(DBADDR *, long);
static long luasubGetPrecision(DBADDR *, long *);
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
epicsExportAddress(rset, luasubRSET);

static long luasubReport(int);
static long luasubInitDevSup(int);
dset lisLUASUB = {
    { 4, /* Space for 4 functions */
      luasubReport,
      luasubInitDevSup,
      NULL,
      lisGetIoIntInfo
    }
};
epicsExportAddress(dset, lisLUASUB);

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
/*
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
*/

/* Beware: return in the middle of the function! */
/*
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
      return FALSE; // Beware: return in the middle of the function!
    }

    *pne=*pno;
    if (povl) {
      if (num) {
        *povl=calloc(*pno,flen);
        if (povl==NULL) {
          lisLogSimple("%s Failed to calloc() inside luasubInitValFields[povl]\n",LIS_LIB_LOG_NAME);
          return FALSE; // Beware: return in the middle of the function!
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
*/

/* Beware: return in the middle of the function! */
/*
static long luasubFetchInputValues(luasubRecord *prec) {
  long status,n_request;
  int i;

  for (i=0; i<NUM_IO_FIELDS; i++) {
    n_request=(&prec->noa)[i];
    status=dbGetLink(&(&prec->inpa)[i],(&prec->fta)[i],(&prec->a)[i],0,&n_request);
    if (n_request>0) (&prec->nea)[i]=n_request;
    if (status) return status; // Beware: return in the middle of the function!
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
*/

static long luasubInitRecord(luasubRecord *prec, int pass) {
    NOW_STR;
    long res=LIS_EPICS_SUCCESS;
  /*
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
  */

    return res;
}

static long luasubProcess(luasubRecord *prec) {
    long res=LIS_EPICS_SUCCESS;
  
    return res;
}

/* Beware: return in the middle of the function! */
static long luasubSpecial(DBADDR *paddr, int after) {
    NOW_STR;
    return LIS_EPICS_SUCCESS;
}

static long luasubCvtDbAddr(DBADDR *paddr) {
    NOW_STR;
    return LIS_EPICS_SUCCESS;
}

static long luasubGetArrayInfo(DBADDR *paddr, long *no_elements, long *offset) {
    NOW_STR;
    return LIS_EPICS_SUCCESS;
}

static long luasubPutArrayInfo(DBADDR *paddr, long n_new) {
    NOW_STR;
    return LIS_EPICS_SUCCESS;
}

static long luasubGetPrecision(DBADDR *paddr, long *precision) {
    return LIS_EPICS_SUCCESS;
}

static long luasubReport(int interest) {
    lisLogSimple("  Lua IOC Support scripts directory: %s\n", lisBaseDirectory);
    return lisReport(interest, LIS_LUASUB_REC_NAME);
}

static long luasubInitDevSup(int phase) {
    NOW_STR;
    return LIS_EPICS_SUCCESS;
}

/* Beware: return in the middle of the function! */
static long luasubAddRecord(struct dbCommon *precord) {
    NOW_STR;
    return 0;
}

static long luasubDelRecord(struct dbCommon *precord) {
    return 0;
}
