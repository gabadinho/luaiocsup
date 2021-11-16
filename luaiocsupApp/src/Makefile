TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += luaiocsup

# install luaiocsup.dbd into <top>/dbd
DBD += luaiocsup.dbd
DBD += luasubRecord.dbd

# Build included Lua source
USR_CFLAGS += -DLUA_COMPAT_5_3
SRC_DIRS += $(TOP)/luaiocsupApp/src/lua-5.4.3/src
#INC += lua.h lualib.h lauxlib.h luaconf.h
luaiocsup_SRCS += lapi.c lauxlib.c lbaselib.c lcode.c lcorolib.c lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lutf8lib.c lvm.c lzio.c

# specify all source files to be compiled and added to the library
INC += lis_lua_state.h lis_base_record.h
luaiocsup_SRCS += luaiocsup.c lis_globals.c lis_common_support.cpp
luaiocsup_SRCS += lis_lua_state.cpp lis_base_record.cpp
luaiocsup_SRCS += lis_ai_record.cpp lis_luasubrec.c

luaiocsup_LIBS += $(EPICS_BASE_IOC_LIBS)

# Or: use system's default lua libraries instead
#USR_INCLUDES += -I/home/gabadinho/SWDev/lua-5.4.3/src
#USR_LDFLAGS += -L/home/gabadinho/SWDev/lua-5.4.3/src
#USR_LDLIBS += lua
##USR_SYS_LIBS += lua

USR_CFLAGS += -D USE_TYPED_RSET
USR_CPPFLAGS += -D USE_TYPED_RSET
 
#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
