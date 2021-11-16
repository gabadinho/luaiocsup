/**
  * \file lis_globals.c
  * \brief Global variables definitions: configuration settings from iocsh command.
  * \author Jose C.G. Gabadinho
  */

#include "lis_globals.h"

#include <stdlib.h>
#include <string.h>



int   lisDebugLevel = 1;
int   lisMinStackSize = 0;
char *lisBaseDirectory = NULL;
int   lisCastCharsAsString = 0;

int lisIsIocRunning = 0;



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

void lisSetIocRunning() {
    lisIsIocRunning = 1;
}
