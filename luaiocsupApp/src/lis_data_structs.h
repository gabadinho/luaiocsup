#ifndef INC_lis_data_structs_H
#define INC_lis_data_structs_H

#include "lis_string_manip.h"
#include "lis_epics.h"

#include <lua.h>

#include <dbBase.h>
#include <dbCommon.h>
#include <dbFldTypes.h>
#include <callback.h>
#include <dbScan.h>
#include <dbAddr.h>
#include <dbAccessDefs.h>
#include <special.h>

struct lisLuaState {
  char *stateId;

  lua_State *masterState;
  epicsMutexId stateLock;

  struct lisFilenameList {
    char *filename;
    struct lisFilenameList *next;
  } *loadedFiles;
};

struct lisLuaStateList {
  struct lisLuaState *lisState;
  struct lisLuaStateList *next;
};

struct lisRecordConfig {
  char *dbLinkFieldName;
  char *configString;

  char *scriptFilename;
  char *className;
  char *stateId;

  char **scriptParams;
  int  paramsLen;
  char *origParams;
};

struct lisRecordRuntime {
  char *luaCode2Run; /* Can be function-name code-chunk */

  lua_State *coroutStack;
  int stackReference;
  epicsMutexId stackLock;

  lua_State *callbackStack; /* Beware: no specific mutex for the callback stack! */
  int cbStackReference;

  int isYielded;
  CALLBACK asyncCallback;

  struct lisIOIntrList *ioIntr;

  int markedForDeletion;
};

struct lisRecordState {
  char *recordName;
  char *recordTypeName;

  struct lisLuaState      *lisState;
  struct lisRecordConfig  *recordConfig;
  struct lisRecordRuntime *recordRuntime;

  struct dbCommon *pRec;
};

struct lisRecordList {
  struct lisRecordState *recordState;
  struct lisRecordList *next;
};

struct lisIOIntrList {
  char *ioIntrId;
  IOSCANPVT *ioScan;
  struct lisRecordList *records;
  struct lisIOIntrList *next;
};

struct lisPutLinkList {
  dbAddr dbaddr;
  char *new_value;
  struct lisPutLinkList *next;
};

struct lisAtExitList {
  lua_State *atExitStack;
  int atExitStackReference;
  struct lisAtExitList *next;
};

#define LIS_GETFLD_LEN 129
static inline dbAddr * lisGetField(const char *rec_name,const char *field_name,dbAddr *db_addr) {
  char buffer[LIS_GETFLD_LEN];
  strcpy(buffer,rec_name);
  strcat(buffer,LIS_RECFLD_SEPARATOR);
  strcat(buffer,field_name);
  if (dbNameToAddr(buffer,db_addr)==0) return db_addr;
  return NULL;
}

int                      lisIsFileLoaded(struct lisFilenameList *loaded_files,char *filename);
struct lisFilenameList * lisAddToLoadedFiles(struct lisFilenameList *loaded_files,char *filename,int *was_new_file);

struct lisLuaState *     lisFetchLuaState(char *state_id,char *record_name,char *record_type,int *new_state_created);
void                     lisDeleteLuaState(struct lisLuaState *lua_st);
struct lisLuaStateList * lisGetLuaStates();

struct lisRecordRuntime * lisCreateRecordRuntime(char *code2run);
void                      lisDeleteRecordRuntime(struct lisRecordRuntime *rec_rt);

struct lisRecordList * lisAddRecordToSupport(struct lisRecordState *rec_state);
struct lisRecordList * lisGetSupportRecords();

struct lisRecordState * lisCreateRecordState(struct dbCommon *prec,char *rec_name,char *rec_type_name);
struct lisLuaState *    lisDeleteRecordState(struct lisRecordState *rec_state);

struct lisRecordConfig * lisCreateRecordConfig(char *db_link_field,char *rec_field_str);
void                     lisDeleteRecordConfig(struct lisRecordConfig *rec_config);

struct lisIOIntrList * lisFetchIoIntr(char *iointr_id,int *new_iointr_created);
struct lisIOIntrList * lisGetIoIntr(const char *iointr_id);
int                    lisIoIntrRegisterRec(struct lisRecordState *rec_state,struct lisIOIntrList *iointr);
struct lisIOIntrList * lisIoIntrUnregisterRec(struct lisRecordState *rec_state);

int lisBindIoIntr(const char *iointr_id,IOSCANPVT *ioscan);

struct lisPutLinkList * lisGetPutLinks();
void                    lisAddToPutLinks(dbAddr *dbaddr,const char *new_value);
void                    lisFreePutLinks();

struct lisAtExitList * lisGetAtExit();
struct lisAtExitList * lisCreateAtExit();

#endif /* INC_lis_data_structs_H */
