#include "lis_data_cast.h"

#include "lis_globals.h"
#include "lis_log.h"
#include "lis_lua.h"

#include <stdlib.h>
#include <string.h>

#include <epicsTypes.h>
#include <epicsTime.h>

static inline long lisDbPutDBR_STRING(dbAddr *dbaddr,const char *val,long no_elements) {
  NOW_STR;
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Updating %s.%s to DBF_STRING '%s' ... (n_el=%ld)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val,no_elements);
  return dbPut(dbaddr,DBR_STRING,val,no_elements);
}
#define UPDATE_DBR_TYPE(DBRTYPE,MODIF,VALTYPE) \
static inline long lisDbPut ## DBRTYPE(dbAddr *dbaddr,VALTYPE *val,long no_elements) { \
  NOW_STR; \
  lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Updating %s.%s to " #DBRTYPE " "MODIF" ... (n_el=%ld)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,*val,no_elements); \
  return dbPut(dbaddr,DBRTYPE,val,no_elements); \
}
UPDATE_DBR_TYPE(DBR_CHAR,"%c",epicsInt8)
UPDATE_DBR_TYPE(DBR_UCHAR,"%u",epicsUInt8)
UPDATE_DBR_TYPE(DBR_SHORT,"%d",epicsInt16)
UPDATE_DBR_TYPE(DBR_USHORT,"%u",epicsUInt16)
UPDATE_DBR_TYPE(DBR_LONG,"%d",epicsInt32)
UPDATE_DBR_TYPE(DBR_ULONG,"%u",epicsUInt32)
UPDATE_DBR_TYPE(DBR_FLOAT,"%f",epicsFloat32)
UPDATE_DBR_TYPE(DBR_DOUBLE,"%f",epicsFloat64)

void lisUpdateLinkFieldCallback(CALLBACK *rec_callback) {
  NOW_STR;
  dbAddr *dbaddr;
  struct lisPutLinkList *put_link;
  long status;

  lisMutexLock(lisPutLinkLock);

  put_link=lisGetPutLinks();
  while (put_link) {
    dbaddr=&put_link->dbaddr;
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Updating %s.%s to DBF_LINK '%s'\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,put_link->new_value);
    status=dbPutField(dbaddr,DBR_STRING,put_link->new_value,1);
    if (status) {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating %s.%s to DBF_LINK '%s' (status=%ld)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,put_link->new_value,status);
	}
    put_link=put_link->next;
  }

  lisFreePutLinks();

  lisMutexUnlock(lisPutLinkLock);
}

long lisUpdateFieldFromIntegerDbPut(lua_State *L,dbAddr *dbaddr) {
  NOW_STR;
  epicsInt32 val_int32;
  epicsUInt32 val_uint32;
  epicsInt16 val_int16;
  epicsUInt16 val_uint16;
  epicsInt8 val_int8;
  epicsUInt8 val_uint8;
  epicsFloat64 val_float64;
  epicsFloat32 val_float32;
  long status=S_db_errArg;
  long updated=0;

  val_int32=lua_tointeger(L,-1);
  lua_pop(L,1);

  switch (dbaddr->field_type) {
    case DBF_CHAR:
      val_int8=val_int32;
      status=lisDbPutDBR_CHAR(dbaddr,&val_int8,1);
      break;

    case DBF_UCHAR:
      val_uint8=val_int32;
      status=lisDbPutDBR_UCHAR(dbaddr,&val_uint8,1);
      break;

    case DBF_SHORT:
      val_int16=val_int32;
      status=lisDbPutDBR_SHORT(dbaddr,&val_int16,1);
      break;

    case DBF_USHORT:
      val_uint16=val_int32;
      status=lisDbPutDBR_USHORT(dbaddr,&val_uint16,1);
      break;

    case DBF_LONG:
      status=lisDbPutDBR_LONG(dbaddr,&val_int32,1);
      break;

    case DBF_ULONG:
      val_uint32=val_int32;
      status=lisDbPutDBR_ULONG(dbaddr,&val_uint32,1);
      break;

    case DBF_FLOAT:
      val_float32=val_int32;
      status=lisDbPutDBR_FLOAT(dbaddr,&val_float32,1);
      break;

    case DBF_DOUBLE:
      val_float64=val_int32;
      updated=lisDbPutDBR_DOUBLE(dbaddr,&val_float64,1);
      break;

    case DBF_ENUM:
      val_uint16=val_int32;
      status=lisDbPutDBR_USHORT(dbaddr,&val_uint16,1);
      break;

    case DBF_MENU:
      val_uint16=val_int32;
      status=lisDbPutDBR_USHORT(dbaddr,&val_uint16,1);
      break;

    default:
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s to %d (unsupported type %d from Lua integer)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val_int32,dbaddr->field_type);
      break;
  }

  if (status==0) updated=1;
  return updated;
}

long lisUpdateFieldFromNumberDbPut(lua_State *L,dbAddr *dbaddr) {
  NOW_STR;
  epicsFloat64 val_number;
  epicsFloat32 val_float;
  long status=S_db_errArg;
  long updated=0;

  val_number=lua_tonumber(L,-1);
  lua_pop(L,1);

  switch (dbaddr->field_type) {
    case DBF_FLOAT:
      val_float=val_number;
      status=lisDbPutDBR_FLOAT(dbaddr,&val_float,1);
      break;

    case DBF_DOUBLE:
      status=lisDbPutDBR_DOUBLE(dbaddr,&val_number,1);
      break;

    default:
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s to %f (unsupported type %d from Lua number)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val_number,dbaddr->field_type);
      break;
  }

  if (status==0) updated=1;
  return updated;
}

/* IMPLEMENT CODE ABOUT UPDATING CHAR WAVEFORM WITH A STRING */
long lisUpdateFieldFromStringDbPut(lua_State *L,dbAddr *dbaddr) {
  NOW_STR;
  const char *val_string;
  long status=S_db_errArg;
  long updated=0;

  val_string=lua_tostring(L,-1);
  lua_pop(L,1);

  switch (dbaddr->field_type) {
    case DBF_STRING:
      status=lisDbPutDBR_STRING(dbaddr,val_string,1);
      break;

    case DBF_CHAR:
      if (lisCastCharsAsString) {
        /* RE-CHECK THIS!!! */
        status=lisDbPutDBR_CHAR(dbaddr,(epicsInt8 *)val_string,strlen(val_string)+1);
      } else {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s to '%s' (unsupported type %d from Lua string)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val_string,dbaddr->field_type);
      }
      break;

    case DBF_INLINK:
    case DBF_OUTLINK:
    case DBF_FWDLINK:
      lisMutexLock(lisPutLinkLock);
      lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Scheduling callback for DBF_LINK update %s.%s to '%s'\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val_string);
      lisAddToPutLinks(dbaddr,val_string);
      callbackRequest(&lisPutLinkCallback);
      lisMutexUnlock(lisPutLinkLock);
      status=0;
      break;

    default:
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s to '%s' (unsupported type %d from Lua string)\n",now_str,LIS_LIB_LOG_NAME,dbaddr->precord->name,dbaddr->pfldDes->name,val_string,dbaddr->field_type);
      break;
  }

  if (status==0) updated=1;
  return updated;
}

/* Updated elements is capped to the length of the field, even if given table is bigger */
/* Beware: return in the middle of the function! */
long lisUpdateFieldFromTableDbPut(lua_State *L,dbAddr *dbaddr,long *max_elements) {
  NOW_STR;
  long status=-1,updated=0;
  char *rec_name=dbaddr->precord->name;
  char *field_name=dbaddr->pfldDes->name;
  long idx,i;
  long table_len,no_elements=dbaddr->no_elements;
  short field_size=dbaddr->field_size;
  epicsFloat64 *val_number=NULL;
  epicsInt32 *val_int32=NULL;
  char *val_str=NULL;
  const char *str_ptr;

  if (no_elements<=1) {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s (table given to a single-value field)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name);
    lua_pop(L,1); /* Pop table */
    return S_db_errArg; /* Beware: return in the middle of the function! */
  }

  table_len=lua_rawlen(L,-1);
  if (table_len>no_elements) {
    lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s  Capping update field %s.%s at field length %ld (given table has length of %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,no_elements,table_len);
    table_len=no_elements;
  }
  if ((max_elements!=NULL) && (*max_elements<table_len)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s  Capping update field %s.%s at given max.length %ld (table has length of %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,*max_elements,table_len);
    table_len=*max_elements;
  }

  for (idx=LIS_LUATABLE_INDEX_START; idx<=table_len; idx++) {
    lua_rawgeti(L,-1,idx);

    if (lua_isinteger(L,-1)) {
      if (val_str!=NULL) {
        free(val_str);
        val_str=NULL;
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s with table of strings (found integer at Cindex %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,idx-1);
        lua_pop(L,1); /* Pop value */
        break;
      }

      if (val_number!=NULL) {
        val_number[idx-1]=lua_tointeger(L,-1);
	  } else {
        if (val_int32==NULL) {
          val_int32=malloc(table_len*sizeof(epicsInt32));
          if (val_int32==NULL) {
            lisLogSimple("%s Failed to malloc() inside lisUpdateFieldFromTableDbPut\n",LIS_LIB_LOG_NAME);
            lua_pop(L,1); /* Pop value */
            break;
          }
        }
        val_int32[idx-1]=lua_tointeger(L,-1);
	  }

    } else if (lua_isnumber(L,-1)) {
      if (val_str!=NULL) {
        free(val_str);
        val_str=NULL;
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s with table of strings (found number at Cindex %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,idx-1);
        lua_pop(L,1); /* Pop value */
        break;
      }

      if (val_number!=NULL) {
        val_number[idx-1]=lua_tonumber(L,-1);
	  } else {
        val_number=malloc(table_len*sizeof(epicsFloat64));
        if (val_number==NULL) {
          lisLogSimple("%s Failed to malloc() inside lisUpdateFieldFromTableDbPut\n",LIS_LIB_LOG_NAME);
          if (val_int32!=NULL) {
            free(val_int32);
            val_int32=NULL;
          }
          lua_pop(L,1); /* Pop value */
          break;
        }
        if (val_int32!=NULL) {
          lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s  Upgrading table from integer to number at Cindex %ld\n",now_str,LIS_LIB_LOG_NAME,idx-1);
          for (i=LIS_LUATABLE_INDEX_START; i<idx; i++) {
            val_number[i-1]=val_int32[i-1];
          }
          free(val_int32);
          val_int32=NULL;
        }

        val_number[idx-1]=lua_tonumber(L,-1);
      }

    } else if (lua_isstring(L,-1)) {
      if ((val_number!=NULL)||(val_int32!=NULL)) {
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s with table of numbers (found string at Cindex %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,idx-1);
        if (val_int32!=NULL) {
          free(val_int32);
          val_int32=NULL;
        }
        if (val_number!=NULL) {
          free(val_number);
          val_number=NULL;
        }
        lua_pop(L,1); /* Pop value */
        break;
	  }

      if (val_str==NULL) {
        val_str=malloc(table_len*field_size*sizeof(char));
        if (val_str==NULL) {
          lisLogSimple("%s Failed to malloc() inside lisUpdateFieldFromTableDbPut\n",LIS_LIB_LOG_NAME);
          if (val_int32!=NULL) {
            free(val_int32);
            val_int32=NULL;
          }
          if (val_number!=NULL) {
            free(val_number);
            val_number=NULL;
          }
          lua_pop(L,1); /* Pop value */
          break;
        }
	  }

	  str_ptr=lua_tostring(L,-1);
	  strncpy(val_str+(idx-1)*field_size,str_ptr,field_size-1);

    } else {
      lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error updating field %s.%s (unsupported value type; stopping at Cindex %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,idx);
      if (val_int32!=NULL) {
        free(val_int32);
        val_int32=NULL;
      }
      if (val_number!=NULL) {
        free(val_number);
        val_number=NULL;
      }
      if (val_str!=NULL) {
        free(val_str);
        val_str=NULL;
      }

      lua_pop(L,1); /* Pop value */
      break;
    }

    lua_pop(L,1); /* Pop value */
  }

  if (val_number!=NULL) {
    status=lisDbPutDBR_DOUBLE(dbaddr,val_number,idx-1);
    free(val_number);
    updated=idx-1;
  } else if (val_int32!=NULL) {
    status=lisDbPutDBR_LONG(dbaddr,val_int32,idx-1);
    free(val_int32);
    updated=idx-1;
  } else if (val_str!=NULL) {
    status=lisDbPutDBR_STRING(dbaddr,val_str,idx-1);
    free(val_str);
    updated=idx-1;
  }
  if (status!=0) updated=0;

  lua_pop(L,1); /* Pop Table */
  return updated;
}

/*
 * <table>[luaiocsup.DBLINK.FIELDTYPE] = field_type
 * <table>[luaiocsup.DBLINK.LINKTYPE] = field_link->type
 * <table>[luaiocsup.DBLINK.LINK] = pv_lnk->pvname OR value.constantStr
 * <table>[luaiocsup.DBLINK.LINKMASK] = pv_lnk->pvlMask OR nil
 */
static inline int lisDbGetDBR_LINK(dbAddr *dbaddr,lua_State *L,short field_type,char *rec_name,char *field_name) {
  NOW_STR;
  struct link *field_link;
  struct pv_link *pv_lnk;
  struct instio *inst_io;
  char *const_str,*lnkname="";
  int pushed=S_db_errArg;

  field_link=(struct link *)dbaddr->pfield;
  if (field_link->type==CONSTANT) {
    const_str=field_link->value.constantStr;
    lua_createtable(L,0,3);

    lua_pushstring(L,"FIELDTYPE");
    lua_pushnumber(L,field_type);
    lua_settable(L,-3);

    lua_pushstring(L,"LINKTYPE");
    lua_pushnumber(L,field_link->type);
    lua_settable(L,-3);

    lua_pushstring(L,"LINK");
    if (const_str) {
      lnkname=const_str;
    }
    lua_pushstring(L,lnkname);
    lua_settable(L,-3);

    pushed=1;
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Retrieving %s.%s as '%s' (field type:%d link type:%d)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,lnkname,field_type,field_link->type);
  } else if ((field_link->type==PV_LINK) || (field_link->type==DB_LINK) || (field_link->type==DB_LINK)) {
    pv_lnk=&field_link->value.pv_link;
    lua_createtable(L,0,4);

    lua_pushstring(L,LIS_DBLINK_TABLE_FIELDTYPE);
    lua_pushnumber(L,field_type);
    lua_settable(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINKTYPE);
    lua_pushnumber(L,field_link->type);
    lua_settable(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINK);
    if (pv_lnk->pvname) {
      lnkname=pv_lnk->pvname;
    }
    lua_pushstring(L,lnkname);
    lua_settable(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINKMASK);
    lua_pushinteger(L,pv_lnk->pvlMask);
    lua_settable(L,-3);

    pushed=1;
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Retrieving %s.%s as '%s' (field type:%d link type:%d mask:%d)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,lnkname,field_type,field_link->type,pv_lnk->pvlMask);
  } else if (field_link->type==INST_IO) {
    inst_io=&field_link->value.instio;
    lua_createtable(L,0,3);

    lua_pushstring(L,LIS_DBLINK_TABLE_FIELDTYPE);
    lua_pushnumber(L,field_type);
    lua_settable(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINKTYPE);
    lua_pushnumber(L,field_link->type);
    lua_settable(L,-3);

    lua_pushstring(L,LIS_DBLINK_TABLE_LINK);
    if (inst_io->string) {
      lnkname=inst_io->string;
    }
    lua_pushstring(L,lnkname);
    lua_settable(L,-3);

    pushed=1;
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Retrieving %s.%s as '%s' (field type:%d link type:%d)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,lnkname,field_type,field_link->type);
  } else {
    lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error retrieving field %s.%s (unsupported link type %d)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,field_link->type);
    lua_pushnil(L);
    pushed=1;
  }
  
  return pushed;
}

static inline int lisDbGetDBR_STRING(dbAddr *dbaddr,lua_State *L,long *no_elements,char *rec_name,char *field_name) {
  NOW_STR;
  long status,options=DBR_STATUS|DBR_TIME;
  long idx;
  void *dbget_buffer=NULL;
  char *val_ptr,*str_ptr;
  struct dbr_status *rec_status;
  struct dbr_time *rec_timestamp;
  double epoch_sec;
  int pushed=S_db_errArg;

  dbget_buffer=malloc(sizeof(struct dbr_status) + sizeof(struct dbr_time) + *no_elements*dbaddr->field_size* sizeof(char));
  if (dbget_buffer==NULL) {
    lisLogSimple("%s Failed to malloc() inside lisDbGetDBR_STRING\n",LIS_LIB_LOG_NAME);
    return S_db_noMemory;
  }
  val_ptr=dbget_buffer+sizeof(struct dbr_status) + sizeof(struct dbr_time);

  status=dbGet(dbaddr,DBR_STRING,dbget_buffer,&options,no_elements,NULL);
  if (status==0) {
    rec_status=dbget_buffer;
    rec_timestamp=dbget_buffer+sizeof(struct dbr_status);
    epoch_sec=rec_timestamp->time.secPastEpoch+POSIX_TIME_AT_EPICS_EPOCH+rec_timestamp->time.nsec*(10e-10);
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Retrieving %s.%s as '%s' ... (n_el:%ld data:%s field:%s)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,val_ptr,*no_elements,"char[]","DBF_STRING"); \

    if (*no_elements>1) {
      lua_createtable(L,*no_elements,0);
      for (idx=LIS_LUATABLE_INDEX_START; idx<=*no_elements; idx++) {
        str_ptr=val_ptr+(idx-1)*dbaddr->field_size;
        lua_pushstring(L,str_ptr);
        lua_rawseti(L,-2,idx);
      }
      lua_pushinteger(L,rec_status->status);
      lua_pushinteger(L,rec_status->severity);
      lua_pushnumber(L,epoch_sec);
      pushed=4;
    } else if (*no_elements==1) {
      lua_pushstring(L,val_ptr);
      lua_pushinteger(L,rec_status->status);
      lua_pushinteger(L,rec_status->severity);
      lua_pushnumber(L,epoch_sec);
      pushed=4;
    } else {
      lua_pushnil(L);
      pushed=1;
    }
  }

  free(dbget_buffer);
  return pushed;
}

#define RETRIEVE_DBR_TYPE(DBRTYPE,MODIF,VALTYPE,PUSHMETHOD) \
static inline long lisDbGet ## DBRTYPE(dbAddr *dbaddr,lua_State *L,long *no_elements,char *dbf,char *rec_name,char *field_name) { \
  NOW_STR; \
  long status,options=DBR_STATUS|DBR_TIME; \
  long idx; \
  void *dbget_buffer=NULL; \
  VALTYPE *val_ptr; \
  struct dbr_status *rec_status; \
  struct dbr_time *rec_timestamp; \
  double epoch_sec; \
  int pushed=S_db_errArg; \
  dbget_buffer=malloc(sizeof(struct dbr_status) + sizeof(struct dbr_time) + *no_elements*sizeof(VALTYPE)); \
  if (dbget_buffer==NULL) { \
    lisLogSimple("%s Failed to malloc() inside lisDbGet*\n",LIS_LIB_LOG_NAME); \
    return S_db_noMemory; \
  } \
  val_ptr=dbget_buffer+sizeof(struct dbr_status) + sizeof(struct dbr_time); \
  status=dbGet(dbaddr,DBRTYPE,dbget_buffer,&options,no_elements,NULL); \
  if (status==0) { \
    rec_status=dbget_buffer; \
    rec_timestamp=dbget_buffer+sizeof(struct dbr_status); \
    epoch_sec=rec_timestamp->time.secPastEpoch+POSIX_TIME_AT_EPICS_EPOCH+rec_timestamp->time.nsec*(10e-10); \
    lisLog(LIS_LOGLVL_VERBOSE,errlogInfo,now_str,"%s %s  Retrieving %s.%s as "MODIF" ... (n_el:%ld data:%s field:%s)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,*val_ptr,*no_elements,#VALTYPE,dbf); \
    if (*no_elements>1) { \
      lua_createtable(L,*no_elements,0); \
      for (idx=LIS_LUATABLE_INDEX_START; idx<=*no_elements; idx++) { \
        lua_pushnumber(L,val_ptr[idx-1]); \
        lua_rawseti(L,-2,idx); \
      } \
      lua_pushinteger(L,rec_status->status); \
      lua_pushinteger(L,rec_status->severity); \
      lua_pushnumber(L,epoch_sec); \
      pushed=4; \
    } else if (*no_elements==1) { \
      lua_push ## PUSHMETHOD(L,*val_ptr); \
      lua_pushinteger(L,rec_status->status); \
      lua_pushinteger(L,rec_status->severity); \
      lua_pushnumber(L,epoch_sec); \
      pushed=4; \
    } else { \
      lua_pushnil(L); \
      pushed=1; \
    } \
  } \
  free(dbget_buffer); \
  return pushed; \
}
RETRIEVE_DBR_TYPE(DBR_CHAR,"%c",epicsInt8,integer)
RETRIEVE_DBR_TYPE(DBR_UCHAR,"%u",epicsUInt8,integer)
RETRIEVE_DBR_TYPE(DBR_SHORT,"%d",epicsInt16,integer)
RETRIEVE_DBR_TYPE(DBR_USHORT,"%u",epicsUInt16,integer)
RETRIEVE_DBR_TYPE(DBR_LONG,"%d",epicsInt32,integer)
RETRIEVE_DBR_TYPE(DBR_ULONG,"%u",epicsUInt32,integer)
RETRIEVE_DBR_TYPE(DBR_FLOAT,"%f",epicsFloat32,number)
RETRIEVE_DBR_TYPE(DBR_DOUBLE,"%f",epicsFloat64,number)

/* TODO: PUSHING A STRING FROM A WAVEFORM OF CHARs!!! */
int lisRetrieveFieldToStackDbGet(lua_State *L,dbAddr *dbaddr,long *max_elements) {
  NOW_STR;
  char *rec_name=dbaddr->precord->name;
  char *field_name=dbaddr->pfldDes->name;
  long no_elements=dbaddr->no_elements;
  int pushed=0;

  if ((max_elements!=NULL) && (*max_elements<no_elements)) {
    lisLog(LIS_LOGLVL_STANDARD,errlogInfo,now_str,"%s %s  Capping retrieve field %s.%s at given max.length %ld (field has length of %ld)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,*max_elements,no_elements);
    no_elements=*max_elements;
  }
  if (no_elements>0) {
    switch (dbaddr->field_type) {
      case DBF_CHAR:
        /* CHECK lisAllowString2DbfChar !!! */
        pushed=lisDbGetDBR_CHAR(dbaddr,L,&no_elements,"DBF_CHAR",rec_name,field_name);
        break;

      case DBF_UCHAR:
        pushed=lisDbGetDBR_UCHAR(dbaddr,L,&no_elements,"DBF_UCHAR",rec_name,field_name);
        break;

      case DBF_SHORT:
        pushed=lisDbGetDBR_SHORT(dbaddr,L,&no_elements,"DBF_SHORT",rec_name,field_name);
        break;

      case DBF_USHORT:
        pushed=lisDbGetDBR_USHORT(dbaddr,L,&no_elements,"DBF_USHORT",rec_name,field_name);
        break;

      case DBF_LONG:
        pushed=lisDbGetDBR_LONG(dbaddr,L,&no_elements,"DBF_LONG",rec_name,field_name);
        break;

      case DBF_ULONG:
        pushed=lisDbGetDBR_ULONG(dbaddr,L,&no_elements,"DBF_ULONG",rec_name,field_name);
        break;

      case DBF_FLOAT:
        pushed=lisDbGetDBR_FLOAT(dbaddr,L,&no_elements,"DBF_FLOAT",rec_name,field_name);
        break;

      case DBF_DOUBLE:
        pushed=lisDbGetDBR_DOUBLE(dbaddr,L,&no_elements,"DBF_DOUBLE",rec_name,field_name);
        break;

      case DBF_ENUM:
        pushed=lisDbGetDBR_USHORT(dbaddr,L,&no_elements,"DBF_ENUM",rec_name,field_name);
        break;

      case DBF_MENU:
        pushed=lisDbGetDBR_USHORT(dbaddr,L,&no_elements,"DBF_MENU",rec_name,field_name);
        break;

      case DBF_STRING:
        pushed=lisDbGetDBR_STRING(dbaddr,L,&no_elements,rec_name,field_name);
        break;

      case DBF_INLINK:
      case DBF_OUTLINK:
      case DBF_FWDLINK:
        pushed=lisDbGetDBR_LINK(dbaddr,L,dbaddr->field_type,rec_name,field_name);
        break;

      default:
        lisLog(LIS_LOGLVL_STANDARD,errlogMajor,now_str,"%s %s  Error retrieving field %s.%s (unsupported type %d)\n",now_str,LIS_LIB_LOG_NAME,rec_name,field_name,dbaddr->field_type);
        break;
    }
  }
  if (pushed==0) {
    lua_pushnil(L);
    pushed=1;
  }

  return pushed;
}
