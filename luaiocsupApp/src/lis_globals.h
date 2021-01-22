#ifndef INC_lis_globals_H
#define INC_lis_globals_H

#include <epicsMutex.h>
#include <callback.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define LIS_EPICS_SUCCESS 0

extern int   lisDebugLevel;
extern int   lisMinStackSize;
extern char *lisBaseDirectory;
extern int   lisCastCharsAsString; /* THIS FEATURE NEEDS REVIEWING/IMPLEMENTATION!!!!!! */

extern epicsMutexId lisRecordsAddDelLock;

extern int lisIsIocRunning;

extern epicsMutexId lisPutLinkLock;
extern CALLBACK lisPutLinkCallback;

void lisSetDebugLevel(int debug_level);
void lisSetMinStackSize(int stack_size);
void lisSetBaseDirectory(char *base_dir);
void lisSetCastCharsAsString(int charsasstring);

#endif /* INC_lis_globals_H */
