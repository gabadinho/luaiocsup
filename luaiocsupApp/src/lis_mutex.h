#ifndef INC_lis_epics_H
#define INC_lis_epics_H

#include <epicsMutex.h>

static inline epicsMutexId lisMutexCreate ()                   { return epicsMutexCreate(); }
static inline void         lisMutexDestroy(epicsMutexId mutex) { epicsMutexDestroy(mutex);  }

static inline epicsMutexLockStatus lisMutexLock   (epicsMutexId mutex) { return epicsMutexLock(mutex); }
static inline void                 lisMutexUnlock (epicsMutexId mutex) { epicsMutexUnlock(mutex);      }

static inline void lisMutexLock2  (epicsMutexId mutex1,epicsMutexId mutex2) { epicsMutexLock(mutex1); epicsMutexLock(mutex2);     }
static inline void lisMutexUnlock2(epicsMutexId mutex1,epicsMutexId mutex2) { epicsMutexUnlock(mutex2); epicsMutexUnlock(mutex1); }

#endif /* INC_lis_epics_H */
