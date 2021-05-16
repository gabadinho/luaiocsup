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
#DBD += luasubRecord.dbd

# specify all source files to be compiled and added to the library
INC += lis_lua.h
#luaiocsup_SRCS += lis_globals.c luaiocsup.c lis_common_support.c lis_string_manip.c lis_lua.c lis_data_structs.c lis_data_cast.c lis_luasubrec.c
luaiocsup_SRCS += lis_globals.c luaiocsup.c lis_common_support.cpp lis_luasubrec.c
luaiocsup_SRCS += lis_base_record.cpp lis_ai_record.cpp

luaiocsup_LIBS += $(EPICS_BASE_IOC_LIBS)

# Build included lua language
SRC_DIRS += $(TOP)/luaiocsupApp/src/lua-5.4.3/src
INC += lua.h lualib.h lauxlib.h luaconf.h
luaiocsup_SRCS += lapi.c lauxlib.c lbaselib.c lcode.c lcorolib.c lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lutf8lib.c lvm.c lzio.c

# Or: use system's default lua libraries instead
#USR_INCLUDES += -I/usr/local/include
#USR_SYS_LIBS += lua

USR_CFLAGS += -D USE_TYPED_RSET
USR_CPPFLAGS += -D USE_TYPED_RSET
 
#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
