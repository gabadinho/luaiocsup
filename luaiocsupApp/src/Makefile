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

# specify all source files to be compiled and added to the library
luaiocsup_SRCS += lis_globals.c luaiocsup.c lis_common_support.c lis_string_manip.c lis_lua.c lis_data_structs.c lis_data_cast.c lis_luasubrec.c

luaiocsup_LIBS += $(EPICS_BASE_IOC_LIBS)

USR_INCLUDES += -I/usr/local/include
USR_SYS_LIBS += lua
USR_CFLAGS += -D USE_TYPED_RSET

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
