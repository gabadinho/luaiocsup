#include "lis_string_manip.h"

#include "lis_globals.h"
#include "lis_log.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Warning: returns a strdup() */
char * lisStrToUpper(char *rec_type) {
  char *c,*s=NULL;
  if (rec_type!=NULL) {
    s=strdup(rec_type);
    if (s!=NULL) {
      c=s;
      while (*c) {
        *c=toupper(*c);
        c++;
      }
    } else {
      lisLogSimple("%s Failed to strdup() inside lisStrToUpper(%s)\n",LIS_LIB_LOG_NAME,rec_type);
    }
  }
  return s;
}

static char * lisBypassChar(char *str,char c) {
  if (str!=NULL) {
    while ((*str)&&(*str==c)) {
      str=str+1;
    } 
  }
  return str;
}

static char * lisBypassSpace(char *str) {
  if (str!=NULL) {
    while ((*str)&&(isspace((int)(*str)))) {
      str=str+1;
    } 
  }
  return str;
}

static char * lisBypassAt(char *str) {
  str=lisBypassSpace(str);
  str=lisBypassChar(str,'@');
  str=lisBypassSpace(str);
  return str;
}

static char * lisBypassAmpersand(char *str) {
  str=lisBypassSpace(str);
  str=lisBypassChar(str,'&');
  str=lisBypassSpace(str);
  return str;
}

/* Warning: returns a strdup() */
char * lisFetchScriptFilename(char *rec_lua_field) {
  char *filename=NULL;
  int i=0;

  if ((rec_lua_field!=NULL)&&(strlen(rec_lua_field)>0)) {
    rec_lua_field=lisBypassAt(rec_lua_field);
    if (strlen(rec_lua_field)) {
      filename=strdup(rec_lua_field);
      if (filename==NULL) {
        lisLogSimple("%s Failed to strdup() inside lisFetchScriptFilename(%s)\n",LIS_LIB_LOG_NAME,rec_lua_field);
      } else {
        for (i=0;i<strlen(rec_lua_field);i++) {
          if ((filename[i]==' ')||(filename[i]=='&')) { /* use isspace() instead */
            filename[i]=0;
            break;
          }
        }
      }
    }
  }

  return filename;
}

/* Warning: returns a strdup() */
char * lisFetchScriptParams(char *rec_lua_field) {
  char *params=NULL;
  int i=0;

  if ((rec_lua_field!=NULL)&&(strlen(rec_lua_field)>0)) {
    rec_lua_field=lisBypassAt(rec_lua_field);
    if (strlen(rec_lua_field)) {
      for (i=0;i<strlen(rec_lua_field);i++) {
        if ((rec_lua_field[i]==' ')||(rec_lua_field[i]=='&')) { /* use isspace() instead */
          params=lisBypassAmpersand(rec_lua_field+i);
          break;
        }
      }
    }
  }
  if (params!=NULL) {
    params=strdup(params);
    if (params==NULL) {
      lisLogSimple("%s Failed to strdup() inside lisFetchScriptParams(%s)\n",LIS_LIB_LOG_NAME,rec_lua_field);
    }
  }

  return params;
}

int lisIsDetachScript(char *rec_lua_field) {
  int detach=0,i=0,space_found=0;

  if ((rec_lua_field!=NULL)&&(strlen(rec_lua_field)>0)) {
    rec_lua_field=lisBypassAt(rec_lua_field);
    if (strlen(rec_lua_field)) {
      for (i=0;i<strlen(rec_lua_field);i++) {
        if (rec_lua_field[i]=='&') {
          detach=1;
          break;
        } else if (rec_lua_field[i]==' ') {
          space_found=1;
        } else {
          if (space_found) {
            break;
          }
        }
      }
    }
  }

  return detach;
}

/* Warning: returns a malloc() */
char * lisJoinDirectoryFilename(char *directory,char *filename) {
  char *buffer;

  buffer=(char*)malloc((strlen(directory)+strlen(filename)+2)*sizeof(char));
  if (buffer==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisJoinDirectoryFilename(%s,%s)\n",LIS_LIB_LOG_NAME,directory,filename);
  } else {
    strcpy(buffer,directory);
    strcat(buffer,LIS_DIR_SEPARATOR);
    strcat(buffer,filename);
  }

  return buffer;
}

/* Warning: returns a strdup() */
char * lisTrimSpaces(char *str) {
  char *s=NULL,*end;
  if (str!=NULL) {
    while ((*str) && (isspace((int)(*str)))) str++;
    s=strdup(str);
    if (s!=NULL) {
      end=s+strlen(s);
      while ((end!=s) && isspace((int)(*(end-1)))) end--;
      *end='\0';
    } else {
      lisLogSimple("%s Failed to strdup() inside lisTrimSpaces(%s)\n",LIS_LIB_LOG_NAME,str);
    }
  }
  return s;
}
