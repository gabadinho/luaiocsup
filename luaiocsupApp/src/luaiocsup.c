/**
  * \file luaiocsup.c
  * \brief Device-support declarations for 'luaiocsup' records.
  * \author Jose C.G. Gabadinho
  *
  * Device-support functions and structures (does not include the 'luasub' record, which is
  * a record-support implementation).
  * These are: ai, ao, bi, bi, mbbi, mbbo, stringin, stringout, longin, longout, waveform.
  * 
  * Also: iocsh command definition and registration.
  */
 
#include "lis_globals.h"
#include "lis_c_common_support.h"
#include "lis_log.h"
#include "lis_epics.h"
#include "lis_defs.h"

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



/** Processes a record (down to the Lua script).
  *
  * \param[in] precord Pointer to the luaiocsup record (of any of the supported types)
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisDevSupProcess(struct dbCommon *precord) {
    return lisProcess(precord, lctFUNCTION_NAME, NULL); // From lis_common_support
}



/** Macro that creates all required device-support functions, per record-type. */
#define RECSUPDEF(REC_RECORD, REC_NAME) \
static long lisAddRecord ## REC_NAME (struct dbCommon *precord) { \
    return lisAddRecord(precord); \
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
    DECL_NOW_STR; \
    if (phase == 0) { \
        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Initializing %s support\n", now_str, LIS_LIB_LOG_NAME, #REC_NAME); \
        devExtend(&lisDsxt ## REC_NAME); \
    } \
    return LIS_DEVSUP_SUCCESS; \
} \
static long lisInitRecord ## REC_NAME (struct dbCommon *precord) { \
    return(lisLegacyInitRecord(precord)); \
} \
static long lisProcess ## REC_NAME (struct REC_RECORD ## Record *precord) { \
    return lisDevSupProcess((struct dbCommon *)precord); \
}



/** Macro that creates the linear-converstion device-support function, 
  * and exports the device-support record structure.
  */
#define EXPORT_DSET_LINCONV(REC_DSET, REC_RECORD, REC_NAME) \
static long lisSpecialLinConv ## REC_NAME (struct REC_RECORD ## Record *precord, int after) { \
    return lisSpecialLinConv((struct dbCommon *)precord, after); \
} \
REC_DSET ## dset lis ## REC_NAME = { \
    { 6, \
      lisReport ## REC_NAME, \
      lisInitLua ## REC_NAME, \
      lisInitRecord ## REC_NAME, \
      lisGetIoIntInfo }, \
    lisProcess ## REC_NAME, \
    lisSpecialLinConv ## REC_NAME \
}; \
epicsExportAddress(dset, lis ## REC_NAME);

/** Macro that exports the device-support record structure. */
#define EXPORT_DSET(REC_DSET, REC_NAME) \
REC_DSET ## dset lis ## REC_NAME = { \
    { 5, \
      lisReport ## REC_NAME, \
      lisInitLua ## REC_NAME, \
      lisInitRecord ## REC_NAME, \
      lisGetIoIntInfo }, \
    lisProcess ## REC_NAME, \
}; \
epicsExportAddress(dset, lis ## REC_NAME);



/** Expand above macros for all luaiocsup supported record types. */
RECSUPDEF(ai, AI)
EXPORT_DSET_LINCONV(ai, ai, AI)

RECSUPDEF(ao, AO)
EXPORT_DSET_LINCONV(ao, ao, AO)

RECSUPDEF(bi, BI)
EXPORT_DSET(bi, BI)

RECSUPDEF(bo, BO)
EXPORT_DSET(bo, BO)

RECSUPDEF(mbbi, MBBI)
EXPORT_DSET(mbbi, MBBI)

RECSUPDEF(mbbo, MBBO)
EXPORT_DSET(mbbo, MBBO)

RECSUPDEF(stringin, STRINGIN)
EXPORT_DSET(stringin, STRINGIN)

RECSUPDEF(stringout, STRINGOUT)
EXPORT_DSET(stringout, STRINGOUT)

RECSUPDEF(longin, LONGIN)
EXPORT_DSET(longin, LONGIN)

RECSUPDEF(longout, LONGOUT)
EXPORT_DSET(longout, LONGOUT)

RECSUPDEF(waveform, WAVEFORM)
EXPORT_DSET(wf, WAVEFORM)



/** Code for iocsh registration. */
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
    DECL_NOW_STR;
    if (state == initHookAfterIocRunning) {
        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s IOC is now running\n", now_str, LIS_LIB_LOG_NAME);
        lisSetIocRunning();
    }
}

static void lisConfigureRegistrar(void) {
    initHookRegister(lisInitHook);
    iocshRegister(&lisConfigureDef, lisIocshConfigure);
}
epicsExportRegistrar(lisConfigureRegistrar);
