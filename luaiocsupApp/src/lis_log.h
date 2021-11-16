/**
  * \file lis_log.h
  * \brief Macros for logging to EPICS errlog.
  * \author Jose C.G. Gabadinho
  */

#ifndef INC_lis_log_H
#define INC_lis_log_H

#include "lis_globals.h"

#include <epicsStdio.h>
#include <errlog.h>
#include <epicsTime.h>



/** Levels of logs verbosity. */
#define LIS_LOGLVL_NONE     0
#define LIS_LOGLVL_STANDARD 1
#define LIS_LOGLVL_VERBOSE  2



/** Buffer for date+time log prefix. */
#define LIS_TIMESTAMP_LEN 24
#define DECL_NOW_STR char now_str[LIS_TIMESTAMP_LEN];



/** Log source prefixes. */
#define LIS_LIB_LOG_NAME    "[luaiocsup]"
#define LIS_SCRIPT_LOG_NAME "[luascript]"



/** Generates a date+time string to be used as a log line prefix.
  *
  * \param[out] buffer Where to write the date+time string
  * \param[in] buffer_size Size of buffer
  */
static inline void lisGenLogTime(char *buffer, int buffer_size) {
    epicsTimeStamp now;
    epicsTimeGetCurrent(&now);
    epicsTimeToStrftime(buffer, buffer_size, "%Y/%m/%d %H:%M:%S.%02f", &now);
}



/** Prints a log message to errlog. Generates a timestamp script if a char buffer is provided.
  *
  * \param[in] level Verbosity level of the message (if global log level is less than message is discarded)
  * \param[in] severity Severity, passed to errlogSevPrintf
  * \param[in] timestamp Buffer for the timestamp, either NULL or now_str
  * \param[in] format Log message, including formatting directives for remaining arguments
  * \param[in] ... Remaining message arguments
  */
#define lisLog(level, severity, timestamp, format, ...)               \
if (lisDebugLevel >= level) {                                         \
    if (timestamp!=NULL) lisGenLogTime(timestamp, sizeof(timestamp)); \
    errlogSevPrintf(severity, format, __VA_ARGS__);                   \
}

/** Simple log message, without date, source, level. */
#define lisLogSimple(...) \
epicsStdoutPrintf(__VA_ARGS__);

#endif /* INC_lis_log_H */
