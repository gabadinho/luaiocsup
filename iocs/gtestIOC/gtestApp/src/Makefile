TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = gtest
# gtest.dbd will be created and installed
DBD += gtest.dbd

# gtest.dbd will be made up from these files:
gtest_DBD += base.dbd

# Include dbd files from all support applications:
gtest_DBD += gtestreg.dbd
gtest_DBD += luaiocsup.dbd

# Add all the support libraries needed by this IOC
gtest_LIBS += luaiocsup

# gtest_registerRecordDeviceDriver.cpp derives from gtest.dbd
gtest_SRCS += gtest_registerRecordDeviceDriver.cpp

gtest_SRCS += gtestRegistrar.cpp

# Build the main IOC entry point on workstation OSs.
gtest_SRCS_DEFAULT += gtestMain.cpp
gtest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#gtest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
gtest_LIBS += $(EPICS_BASE_IOC_LIBS)

# Note: googletest was compiled by passing -DCMAKE_POSITION_INDEPENDENT_CODE=ON to cmake
#       to prevent 'recompile with -fPIC' error when creating libluaiocsup.so
#       and installed to /usr/local
USR_SYS_LIBS += gtest pthread
USR_INCLUDES += -I../../../../../luaiocsupApp/src/

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

