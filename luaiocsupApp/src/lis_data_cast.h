#ifndef INC_lis_data_cast_H
#define INC_lis_data_cast_H

#include "lis_data_structs.h"

#include <stddef.h>

#include <lua.h>

#include <dbFldTypes.h>
#include <dbAddr.h>

int lisRetrieveFieldToStackDirect(lua_State *L,dbAddr *dbaddr,long *from_idx,long *upto_idx);

void lisUpdateLinkFieldCallback(CALLBACK *rec_callback);

long lisUpdateFieldFromIntegerDbPut(lua_State *L,dbAddr *dbaddr);
long lisUpdateFieldFromNumberDbPut(lua_State *L,dbAddr *dbaddr);
long lisUpdateFieldFromStringDbPut(lua_State *L,dbAddr *dbaddr);

long lisUpdateFieldFromTableDbPut(lua_State *L,dbAddr *dbaddr,long *max_elements);

int lisRetrieveFieldToStackDbGet(lua_State *L,dbAddr *dbaddr,long *max_elements);

#endif /* INC_lis_data_cast_H */
