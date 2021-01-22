#!../../bin/linux-x86_64/datetest

#- You may have to change datetest to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/datetest.dbd"
datetest_registerRecordDeviceDriver pdbbase

## Configure Lua IOC Support
lisConfigure("${TOP}/luascripts",2,20)

## Load record instances
dbLoadRecords("db/stringout.db","RECORD=LUA:DATE,DTYP=lua,OUT='@timenow.lua'")

cd "${TOP}/iocBoot/${IOC}"
iocInit

