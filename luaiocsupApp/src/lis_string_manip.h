#ifndef INC_lis_string_manip_H
#define INC_lis_string_manip_H

#include <stdlib.h>
#include <string.h>

#define LIS_DIR_SEPARATOR "/"

char * lisStrToUpper(char *rec_type);

char * lisFetchScriptFilename(char *rec_lua_field);
char * lisFetchScriptParams(char *rec_lua_field);
int    lisIsDetachScript(char *rec_lua_field);

char * lisJoinDirectoryFilename(char *directory,char *filename);

char * lisTrimSpaces(char *str);

static inline unsigned char lisStringHash(const char *str) {
  int i=0;
  while (*str) i+=*str++;
  return (unsigned char)i;
}

#define LIS_STRFORMAT_LEN 1025
#define lisFormatString(buffer,format,...)               \
(*buffer)=(char*)malloc(LIS_STRFORMAT_LEN*sizeof(char)); \
sprintf(*buffer,format,__VA_ARGS__);                     \

#define lisAllocString(buffer)                           \
(*buffer)=(char*)malloc(LIS_STRFORMAT_LEN*sizeof(char)); \
(*buffer)[0]=0;                                          \

#define lisAppendString(buffer,string)                                      \
if (strlen(buffer)+strlen(string)<LIS_STRFORMAT_LEN) strcat(buffer,string); \

#define lisDiscardString(buffer) \
free(buffer);                    \

#ifdef vxWorks
#include <version.h>
/* For vxWorks */
#ifndef _WRS_VXWORKS_MAJOR
/* For vxWorks < ver.6 */
#define strdup(s) ({ char* __r=(char*)malloc(strlen(s)+1); __r ? strcpy(__r, s) : NULL; })
#endif

#endif 

#endif /* INC_lis_string_manip_H */
