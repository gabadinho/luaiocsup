#ifndef INC_lis_log_H
#define INC_lis_log_H

#include "lis_globals.h"

#include <lua.h>

#include <epicsStdio.h>
#include <errlog.h>
#include <epicsTime.h>

#define LIS_LOGLVL_NONE     0
#define LIS_LOGLVL_STANDARD 1
#define LIS_LOGLVL_VERBOSE  2

#define LIS_TIMESTAMP_LEN 24
#define NOW_STR char now_str[LIS_TIMESTAMP_LEN]

#define LIS_LIB_LOG_NAME    "[luaiocsup]"
#define LIS_SCRIPT_LOG_NAME "[luascript]"

static inline void lisGenLogTime(char *buffer,int buffer_size) {
  epicsTimeStamp now;
  epicsTimeGetCurrent(&now);
  epicsTimeToStrftime(buffer,buffer_size,"%Y/%m/%d %H:%M:%S.%02f",&now);
}

#define lisLog(level,severity,timestamp,format,...)                \
if (lisDebugLevel>=level) {                                        \
  if (timestamp!=NULL) lisGenLogTime(timestamp,sizeof(timestamp)); \
  errlogSevPrintf(severity,format,__VA_ARGS__);                    \
}

#define lisLogSimple(...)       \
epicsStdoutPrintf(__VA_ARGS__);

#endif /* INC_lis_log_H */
