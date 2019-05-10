#ifndef INC_lis_epics_H
#define INC_lis_epics_H

#include <epicsMutex.h>

#define LIS_AI_REC_NAME        "AI"
#define LIS_AO_REC__NAME       "AO"
#define LIS_BI_REC_NAME        "BI"
#define LIS_BO_REC_NAME        "BO"
#define LIS_MBBI_REC_NAME      "MBBI"
#define LIS_MBBO_REC_NAME      "MBBO"
#define LIS_STRINGIN_REC_NAME  "STRINGIN"
#define LIS_STRINGOUT_REC_NAME "STRINGOUT"
#define LIS_LONGIN_REC_NAME    "LONGIN"
#define LIS_LONGOUT_REC_NAME   "LONGOUT"
#define LIS_WAVEFORM_REC_NAME  "WAVEFORM"

#define LIS_LUASUB_REC_NAME    "LUASUB"

#define LIS_INP_FIELD_NAME  "INP"
#define LIS_OUT_FIELD_NAME  "OUT"

#define LIS_RECFLD_SEPARATOR "."

static inline epicsMutexId lisMutexCreate ()                   { return epicsMutexCreate(); }
static inline void         lisMutexDestroy(epicsMutexId mutex) { epicsMutexDestroy(mutex);  }

static inline epicsMutexLockStatus lisMutexLock   (epicsMutexId mutex) { return epicsMutexLock(mutex); }
static inline void                 lisMutexUnlock (epicsMutexId mutex) { epicsMutexUnlock(mutex);      }

static inline void lisMutexLock2  (epicsMutexId mutex1,epicsMutexId mutex2) { epicsMutexLock(mutex1); epicsMutexLock(mutex2);     }
static inline void lisMutexUnlock2(epicsMutexId mutex1,epicsMutexId mutex2) { epicsMutexUnlock(mutex2); epicsMutexUnlock(mutex1); }

#endif /* INC_lis_epics_H */
