/**
  * \file lis_globals.h
  * \brief Global variables definitions: configuration settings from iocsh command.
  * \author Jose C.G. Gabadinho
  */

#ifndef INC_lis_globals_H
#define INC_lis_globals_H

#include <epicsMutex.h>
#include <callback.h>



/** Global variable for the debug level. */
extern int lisDebugLevel;

/** Global variable for the co-routine minimum stack size. */
extern int lisMinStackSize;

/** Global variable for the directory of where to file the Lua scripts. */
extern char *lisBaseDirectory;

/** Global variable for using waveform of chars as strings. */
extern int lisCastCharsAsString; /* Note: this feature needs reviewing/implementation!!! */

/** Global variable to indicate that the IOC is in running mode. */
extern int lisIsIocRunning;



/** Sets the global debug level (used by lisLog()).
  *
  * \param[in] debug_level Global debug level
  */
void lisSetDebugLevel(int debug_level);

/** Sets the minimum stack size (when creating co-routine stacks with lua_newthread()).
  *
  * \param[in] stack_size Minimum stack size, in elements
  */
void lisSetMinStackSize(int stack_size);

/** Sets the directory where the lua scripts, referenced by the records, are located.
  *
  * \param[in] base_dir Lua scripts directory
  */
void lisSetBaseDirectory(char *base_dir);

/** Enabled the mode that a waveform of chars can be seens as a string.
  *
  * \param[in] base_dir Lua scripts directory
  */
void lisSetCastCharsAsString(int charsasstring);

/** Indicates the Lua IOC Support internal code that the IOC is fully configured and 
  * is now in running mode (this is used to force the reloading of scripts when updating
  * a record INP/OUT field, handled by extended device support).
  */
void lisSetIocRunning();

#endif /* INC_lis_globals_H */
