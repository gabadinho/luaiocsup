https://dillinger.io/
https://stackedit.io/app#

# Overview
EPICS device- and record-support for the Lua language.

### What can it be used for
- Programming device support in Lua instead of C
  - Supported records: ai, ao, bi, bo, mbbi, mbbo, stringin, stringout, waveform
- Programming subroutine records in Lua instead of C
  - New record: luasub

#### Use-cases
- Replacing difficult-to-mantain database record sets
- Improving flexibility of seq/calc records
- Replacing sequencer code: removing compilation phase, and no IOC-restart required for code update

### What it's not
- It is not another interface layer to an EPICS IOC
- It does not change the behavior of an EPICS IOC

# Usage

### Initialization
Add the following command in your IOC startup script file:
```
lisConfigure (<script directory>, <log level>, <stack size>, <treat char waveform as string>)
```
Arguments:
1. Script directory
   - Absolute directory path where the _.lua_ files are located
2. Log-level; possible values are:
   - 0 = None
   - 1 = Standard
   - 2 = Verbose
3. Stack size for Lua states
   - Default if 0
4. If 1, char waveform types will be treated as strings in the Lua code
   - Not yet implemented!

Example:
```
lisConfigure ("$(TOP)/luascripts", 2, 0, 0)
```

### Record configuration
Remember sure to specify the device type:
```
field(DTYP,"lua")
```

Specify the .lua file in the INP/OUT field:
```
field(OUT,"@strout_date.lua")
```

Passing parameters to the Lua support:
There are 2 dedicated parameters for the Lua support:
- @id=\<state id>
  - Allows you to specify the state ID of where the _.lua_ file will be loaded and the code executed (default is the filename)
- @table=\<methods table>
  - Allows you to specify a global table, where the support methods are to be found as fields

Passing parameters to the .lua script file:
- Everything else is passed to the global _arg_ table

Examples:
```
field(OUT,"@strout_date.lua @id=mystate")
```
```
field(OUT,"@strout_date.lua @table=myrec arg1 arg2")
```

### Lua script implementation

#### Device support
Lua IOC Support forwards device-support callbacks to the record's Lua state, expecting to find the following methods in the _.lua_ file:
```
init_record(<rec.table>)
special_linconv(<rec.table>)
add_record(<rec.table>)
del_record(<rec.table>)
get_ioint_info(<rec.table>)
```

The processing callback is however different for each record type:
```
read_ai(<rec.table>)
write_ao(<rec.table>)
read_bi(<rec.table>)
write_bo(<rec.table>)
read_mbbi(<rec.table>)
write_mbbo(<rec.table>)
read_stringin(<rec.table>)
write_stringout(<rec.table>)
read_longin(<rec.table>)
write_longout(<rec.table>)
read_wf(<rec.table>)
```

The return of the above Lua methods is of course passed to the device-support.

##### Extended-device support
The library also implements extended-device support (as noticed by the _add_record_ and _del_record_ callbacks), therefore remember that you're able to modify the _INP_ and _OUT_ fields in a running IOC.

#### Record support
In the _luasub_ record no methods are hardwired, you specify the method names in the _ICOD_ (record initialization) and in the _PCOD_ (record processing) fields. Inline Lua code, instead of a method name, is also valid, as long as the _ACTP_ field is _FUNORCHK_ (default) or _CHUNK_.

Output fields are forwared to their respective links according to the _DRVO_ field:
- _NEVER_: output values are never forwarded to output links
- _ON_CHANGE_: output values are forwarded if changed from the Lua code
- _ALWAYS_: output values are always forwarded to output links
- _ON_VALID_: output values are forwarded if respective _VLD?_ field is explicitly set to _VALID_ from the Lua code

#### Using table fields instead of global methods
If the _@table_ parameter is specified in the _INP_/_OUT_ then the library expects the above methods as fields in the given table. Example:
```
myrec.init_record(<rec.table>)
```

#### Record table API
```
<record name> <rec.table>.record_name()
<curr/prev PACT> <rec.table>.pact(¿<new PACT>?)
<curr/prev NORD> <rec.table>.nord(¿<new NORD>?)
<field value> <rec.table>.retrieve_field(<field name>)
<rec.table>.update_field(<field name>,<field value>)
<rec.table>.scan_once()
<rec.table>.process()
<rec.table>.scan_lock()
<rec.table>.scan_unlock()
```

#### Meta-methods are available for read/write field access:
```
<rec.table>.<field name> = <new value>
<curr.value> = <rec.table>.<field name>
```

### Module API

#### Logging
Logging API, using _errlogSevPrintf_:
```
luaiocsup.ioclog(<level>,<message>)
luaiocsup.ioclog_minor(<message>)
luaiocsup.ioclog_major(<message>)
luaiocsup.ioclog_fatal(<message>)
luaiocsup.ioclog_info(<message>)
```

#### Scanning
Lua records suport _I/O Intr_ scanning. The _IOSCANPVT_ data structure is however internal and not transferable outside of the Lua IOC Support library.
```
<new vs reused id> luaiocsup.scanio_init(<io_strid>)
<id found> luaiocsup.scanio_request(<io_strid>,...)
```

You can post soft-events:
```
<boolean> luaiocsup.post_event(<event number>)
```

#### Other
Retrieving non-Lua records:
```
<rec.table> luaiocsup.find_record(<rec.name>)
```

#### IOC-related
You can check if the IOC is indeed running and register callbacks for when the IOC exits:
```
<boolean> luaiocsup.ioc_running()
luaiocsup.at_exit(<lua function>)
```

### Threading
The Lua IOC Support library as been sucessfully tested with threads outside of the EPICS IOC world, more specificaly with the _llthreads2_ library. If these worker-threads want to however trigger a record-processing via an I/O (or soft) event they must have access to the _LIS_ API, therefore the worker-thread as to call the following method in order to bind the required methods:
```
<boolean> luaiocsup.bind_luaiocsup()
```

