#include "lis_globals.h"

//#include <stdlib.h>
#include <string.h>

int   lisDebugLevel = 1;
int   lisMinStackSize = 0;
char *lisBaseDirectory = NULL;
int   lisCastCharsAsString = FALSE;

epicsMutexId lisRecordsAddDelLock; /* MOVE THIS TO BaseRecord CLASS!!!*/

int lisIsIocRunning = FALSE;

epicsMutexId lisPutLinkLock;
CALLBACK lisPutLinkCallback;

void lisSetDebugLevel(int debug_level) {
    lisDebugLevel = debug_level;
}

void lisSetMinStackSize(int stack_size) {
    lisMinStackSize = stack_size;
}

void lisSetBaseDirectory(char *base_dir) {
    lisBaseDirectory = strdup(base_dir);
}

void lisSetCastCharsAsString(int charsasstring) {
    lisCastCharsAsString = charsasstring;
}
