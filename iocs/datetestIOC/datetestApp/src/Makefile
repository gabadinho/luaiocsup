TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = datetest
# datetest.dbd will be created and installed
DBD += datetest.dbd

# datetest.dbd will be made up from these files:
datetest_DBD += base.dbd

# Include dbd files from all support applications:
datetest_DBD += luaiocsup.dbd

# Add all the support libraries needed by this IOC
datetest_LIBS += luaiocsup

# datetest_registerRecordDeviceDriver.cpp derives from datetest.dbd
datetest_SRCS += datetest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
datetest_SRCS_DEFAULT += datetestMain.cpp
datetest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#datetest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
datetest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

