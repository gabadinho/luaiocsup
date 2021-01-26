TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = dateformat
# dateformat.dbd will be created and installed
DBD += dateformat.dbd

# dateformat.dbd will be made up from these files:
dateformat_DBD += base.dbd

# Include dbd files from all support applications:
dateformat_DBD += luaiocsup.dbd

# Add all the support libraries needed by this IOC
dateformat_LIBS += luaiocsup

# dateformat_registerRecordDeviceDriver.cpp derives from dateformat.dbd
dateformat_SRCS += dateformat_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
dateformat_SRCS_DEFAULT += dateformatMain.cpp
dateformat_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#dateformat_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
dateformat_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

