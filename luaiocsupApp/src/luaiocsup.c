#include <stdlib.h>

#include "lis_globals.h"
#include "lis_common_support.h"
#include "lis_log.h"
#include "lis_data_structs.h"
#include "lis_lua.h"
#include "lis_epics.h"

#include <registryFunction.h>
#include <epicsExport.h>

#include <iocsh.h>
#include <devSup.h>
#include <epicsStdio.h>
#include <initHooks.h>

#define RECSUPDEF(REC,DB_LINK_FIELD,PROC_METH_NAME,LINCONV) \
static long lisAddRecord ## REC (struct dbCommon *precord) { \
  return lisAddRecord(precord,DB_LINK_FIELD,PROC_METH_NAME); \
} \
static long lisDelRecord ## REC (struct dbCommon *precord) { \
  return lisDeleteRecord(precord); \
} \
static struct dsxt lisDsxt ## REC = { \
    lisAddRecord ## REC, lisDelRecord ## REC \
}; \
static long lisReport ## REC (int interest) { \
  return lisReport(interest,# REC); \
} \
static long lisInitLua ## REC (int phase) { \
  NOW_STR; \
  if(phase==0) { \
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s Initializing %s support\n",now_str,LIS_LIB_LOG_NAME,#REC); \
    devExtend(&lisDsxt ## REC); \
  } \
  return LIS_EPICS_SUCCESS; \
} \
static long lisInitRecord ## REC (struct dbCommon *precord) { \
  return(lisLegacyInitRecord(precord)); \
} \
struct { \
  long num; \
  DEVSUPFUN report; \
  DEVSUPFUN init; \
  DEVSUPFUN init_record; \
  DEVSUPFUN get_ioint_info; \
  DEVSUPFUN proc_fun; \
  DEVSUPFUN special_linconv; \
} lis ## REC = { \
  6, /* space for 6 functions */ \
  lisReport ## REC, \
  lisInitLua ## REC, \
  lisInitRecord ## REC, \
  lisGetIoIntInfo, \
  lisDevSupProcess, \
  LINCONV \
}; \
epicsExportAddress(dset,lis ## REC);

long lisDevSupProcess(struct dbCommon *precord) {
  return lisProcess(precord,LIS_ALLOW_FUNCTION_ONLY,NULL);
}

RECSUPDEF(AI,LIS_INP_FIELD_NAME,LIS_AI_PROCESS_METHOD,lisSpecialLinConv)
RECSUPDEF(AO,LIS_OUT_FIELD_NAME,LIS_AO_PROCESS_METHOD,lisSpecialLinConv)

RECSUPDEF(BI,LIS_INP_FIELD_NAME,LIS_BI_PROCESS_METHOD,NULL)
RECSUPDEF(BO,LIS_OUT_FIELD_NAME,LIS_BO_PROCESS_METHOD,NULL)

RECSUPDEF(MBBI,LIS_INP_FIELD_NAME,LIS_MBBI_PROCESS_METHOD,NULL)
RECSUPDEF(MBBO,LIS_OUT_FIELD_NAME,LIS_MBBO_PROCESS_METHOD,NULL)

RECSUPDEF(STRINGIN,LIS_INP_FIELD_NAME,LIS_STRINGIN_PROCESS_METHOD,NULL)
RECSUPDEF(STRINGOUT,LIS_OUT_FIELD_NAME,LIS_STRINGOUT_PROCESS_METHOD,NULL)

RECSUPDEF(LONGIN,LIS_INP_FIELD_NAME,LIS_LONGIN_PROCESS_METHOD,NULL)
RECSUPDEF(LONGOUT,LIS_OUT_FIELD_NAME,LIS_LONGOUT_PROCESS_METHOD,NULL)

RECSUPDEF(WAVEFORM,LIS_INP_FIELD_NAME,LIS_WAVEFORM_PROCESS_METHOD,NULL)

static const iocshArg lisConfigureArg0 = {"Directory",iocshArgString};
static const iocshArg lisConfigureArg1 = {"Debug level",iocshArgInt};
static const iocshArg lisConfigureArg2 = {"Stack size",iocshArgInt};
static const iocshArg lisConfigureArg3 = {"Allow Str2WfChar",iocshArgInt};

static const iocshArg *const lisConfigureArgs[4] = {&lisConfigureArg0,&lisConfigureArg1,&lisConfigureArg2,&lisConfigureArg3};
static const iocshFuncDef lisConfigureDef = {"lisConfigure",4,lisConfigureArgs};

void lisConfigure(char *scripts_dir,int log_level,int stack_size,int charsasstring) {
  epicsStdoutPrintf("Lua IOC Support scripts dir: %s\n",scripts_dir);
  epicsStdoutPrintf("Lua IOC Support log level  : %d\n",log_level);
  epicsStdoutPrintf("Lua IOC Support stack size : %d\n",stack_size);
  epicsStdoutPrintf("Lua IOC Support chr-as-str : %d\n",charsasstring);
  lisSetBaseDirectory(scripts_dir);
  lisSetDebugLevel(log_level);
  lisSetMinStackSize(stack_size);
  lisSetCastCharsAsString(charsasstring);
}

static void lisIocshConfigure(const iocshArgBuf *args) {
  lisConfigure(args[0].sval,args[1].ival,args[2].ival,args[3].ival);
}

void lisInitHook(initHookState state) {
  NOW_STR;
  if (state==initHookAfterIocRunning) {
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s IOC is now running\n",now_str,LIS_LIB_LOG_NAME);
    lisIsIocRunning=TRUE;
  }
}

static void lisConfigureRegistrar(void) {
  lisRecordsAddDelLock=lisMutexCreate();
  initHookRegister(lisInitHook);
  iocshRegister(&lisConfigureDef,lisIocshConfigure);
}
epicsExportRegistrar(lisConfigureRegistrar);
