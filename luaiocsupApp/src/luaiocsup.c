#include "lis_globals.h"
#include "lis_c_common_support.h"
#include "lis_log.h"
#include "lis_defs.h"
#include "lis_mutex.h"

#include <registryFunction.h>
#include <epicsExport.h>

#include <iocsh.h>
#include <devSup.h>
#include <epicsStdio.h>
#include <initHooks.h>

#include <aiRecord.h>
#include <aoRecord.h>
#include <biRecord.h>
#include <boRecord.h>
#include <mbbiRecord.h>
#include <mbboRecord.h>
#include <stringinRecord.h>
#include <stringoutRecord.h>
#include <longinRecord.h>
#include <longoutRecord.h>
#include <waveformRecord.h>

#define RECSUPDEF(REC_RECORD, REC_NAME, DB_LINK_FIELD, PROC_METH_NAME) \
static long lisAddRecord ## REC_NAME (struct dbCommon *precord) { \
    return lisAddRecord(precord, DB_LINK_FIELD, PROC_METH_NAME); \
} \
static long lisDelRecord ## REC_NAME (struct dbCommon *precord) { \
    return lisDeleteRecord(precord); \
} \
static struct dsxt lisDsxt ## REC_NAME = { \
    lisAddRecord ## REC_NAME, lisDelRecord ## REC_NAME \
}; \
static long lisReport ## REC_NAME (int interest) { \
    return lisReport(interest, # REC_NAME); \
} \
static long lisInitLua ## REC_NAME (int phase) { \
    NOW_STR; \
    if(phase==0) { \
        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Initializing %s support\n", now_str, LIS_LIB_LOG_NAME, #REC_NAME); \
        devExtend(&lisDsxt ## REC_NAME); \
    } \
    return LIS_EPICS_SUCCESS; \
} \
static long lisInitRecord ## REC_NAME (struct dbCommon *precord) { \
    return(lisLegacyInitRecord(precord)); \
} \
static long lisProcess ## REC_NAME (struct REC_RECORD ## Record *precord) { \
    return lisDevSupProcess((struct dbCommon *)precord); \
}

#define EXPORT_DSET(REC_DSET, REC_NAME) \
REC_DSET ## dset lis ## REC_NAME = { \
    { 5, /* space for 5 functions */ \
      lisReport ## REC_NAME, \
      lisInitLua ## REC_NAME, \
      lisInitRecord ## REC_NAME, \
      lisGetIoIntInfo }, \
    lisProcess ## REC_NAME, \
}; \
epicsExportAddress(dset, lis ## REC_NAME);

#define EXPORT_DSET_LINCONV(REC_DSET, REC_RECORD, REC_NAME) \
static long lisSpecialLinConv ## REC_NAME (struct REC_RECORD ## Record *precord, int after) { \
    return lisSpecialLinConv((struct dbCommon *)precord, after); \
} \
REC_DSET ## dset lis ## REC_NAME = { \
    { 6, /* space for 6 functions */ \
      lisReport ## REC_NAME, \
      lisInitLua ## REC_NAME, \
      lisInitRecord ## REC_NAME, \
      lisGetIoIntInfo }, \
    lisProcess ## REC_NAME, \
    lisSpecialLinConv ## REC_NAME \
}; \
epicsExportAddress(dset, lis ## REC_NAME);

long lisDevSupProcess(struct dbCommon *precord) {
    return lisProcess(precord, LIS_ALLOW_FUNCTION_ONLY, NULL);
}

RECSUPDEF(ai, AI, LIS_INP_FIELD_NAME, LIS_AI_PROCESS_METHOD)
EXPORT_DSET_LINCONV(ai, ai, AI)

RECSUPDEF(ao, AO, LIS_OUT_FIELD_NAME, LIS_AO_PROCESS_METHOD)
EXPORT_DSET_LINCONV(ao, ao, AO)

RECSUPDEF(bi, BI, LIS_INP_FIELD_NAME, LIS_BI_PROCESS_METHOD)
EXPORT_DSET(bi, BI)

RECSUPDEF(bo, BO, LIS_OUT_FIELD_NAME, LIS_BO_PROCESS_METHOD)
EXPORT_DSET(bo, BO)

RECSUPDEF(mbbi, MBBI, LIS_INP_FIELD_NAME, LIS_MBBI_PROCESS_METHOD)
EXPORT_DSET(mbbi, MBBI)

RECSUPDEF(mbbo, MBBO, LIS_OUT_FIELD_NAME, LIS_MBBO_PROCESS_METHOD)
EXPORT_DSET(mbbo, MBBO)

RECSUPDEF(stringin, STRINGIN, LIS_INP_FIELD_NAME, LIS_STRINGIN_PROCESS_METHOD)
EXPORT_DSET(stringin, STRINGIN)

RECSUPDEF(stringout, STRINGOUT, LIS_OUT_FIELD_NAME, LIS_STRINGOUT_PROCESS_METHOD)
EXPORT_DSET(stringout, STRINGOUT)

RECSUPDEF(longin, LONGIN, LIS_INP_FIELD_NAME, LIS_LONGIN_PROCESS_METHOD)
EXPORT_DSET(longin, LONGIN)

RECSUPDEF(longout, LONGOUT, LIS_OUT_FIELD_NAME, LIS_LONGOUT_PROCESS_METHOD)
EXPORT_DSET(longout, LONGOUT)

RECSUPDEF(waveform, WAVEFORM, LIS_INP_FIELD_NAME, LIS_WAVEFORM_PROCESS_METHOD)
EXPORT_DSET(wf, WAVEFORM)

static const iocshArg lisConfigureArg0 = { "Directory", iocshArgString };
static const iocshArg lisConfigureArg1 = { "Debug level", iocshArgInt };
static const iocshArg lisConfigureArg2 = { "Stack size", iocshArgInt };
static const iocshArg lisConfigureArg3 = { "Allow Str2WfChar", iocshArgInt };

static const iocshArg *const lisConfigureArgs[4] = { &lisConfigureArg0, &lisConfigureArg1, &lisConfigureArg2, &lisConfigureArg3 };
static const iocshFuncDef lisConfigureDef = { "lisConfigure", 4, lisConfigureArgs };

void lisConfigure(char *scripts_dir, int log_level, int stack_size, int charsasstring) {
    epicsStdoutPrintf("Lua IOC Support scripts dir: %s\n", scripts_dir);
    epicsStdoutPrintf("Lua IOC Support log level  : %d\n", log_level);
    epicsStdoutPrintf("Lua IOC Support stack size : %d\n", stack_size);
    epicsStdoutPrintf("Lua IOC Support chr-as-str : %d\n", charsasstring);
    lisSetBaseDirectory(scripts_dir);
    lisSetDebugLevel(log_level);
    lisSetMinStackSize(stack_size);
    lisSetCastCharsAsString(charsasstring);
}

static void lisIocshConfigure(const iocshArgBuf *args) {
    lisConfigure(args[0].sval, args[1].ival, args[2].ival, args[3].ival);
}

void lisInitHook(initHookState state) {
    NOW_STR;
    //lisSupInitHook(state);
    /*if (state==initHookAtIocBuild) {
        lisDevSupHookAtIocBuild();
    } else*/ if (state==initHookAfterIocRunning) {
        //lisDevSupHookAfterIocRunning();

        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s IOC is now running\n", now_str, LIS_LIB_LOG_NAME);
        lisIsIocRunning = TRUE;
    }
}

static void lisConfigureRegistrar(void) {
    lisRecordsAddDelLock=lisMutexCreate();
    initHookRegister(lisInitHook);
    iocshRegister(&lisConfigureDef, lisIocshConfigure);
}
epicsExportRegistrar(lisConfigureRegistrar);
