#include "lis_globals.h"
#include "lis_string_manip.h"

#include <stdlib.h>
#include <string.h>

int   lisDebugLevel = 1;
int   lisMinStackSize = 0;
char *lisBaseDirectory = NULL;
int   lisCastCharsAsString = FALSE;

epicsMutexId lisRecordsAddDelLock;

int lisIsIocRunning = FALSE;

epicsMutexId lisPutLinkLock;
CALLBACK lisPutLinkCallback;

void lisSetDebugLevel(int debug_level) {
  lisDebugLevel=debug_level;
}

void lisSetMinStackSize(int stack_size) {
  lisMinStackSize=stack_size;
}

void lisSetBaseDirectory(char *base_dir) {
  lisBaseDirectory=strdup(base_dir);
}

void lisSetCastCharsAsString(int charsasstring) {
  lisCastCharsAsString=charsasstring;
}
