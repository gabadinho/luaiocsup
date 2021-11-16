/**
  * \file lis_luasub_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS luasub record.
  * \author Jose C.G. Gabadinho
  *
  * Derived record from BaseRecord, to implement luasub specific details.
  * The base class factory should be use instead of constructors.
  */

#ifndef INC_lis_luasub_record_H
#define INC_lis_luasub_record_H

#include "lis_base_record.h"

#include "lis_luasubrec.h"
#include "lis_epics.h"



/** Class wrapper for a 'luaiocsup' luasub record.
  */
class LuaSubRecord: public BaseRecord {
public:
    /** Constructor for a 'luaiocrec' luasub record.
      * Parses INP/OUT field link value, creates Lua states, etc.
      *  
      * \param[in] precord Pointer to EPICS record structure
      * \param[in] rec_name Record name
      * \param[in] force_load true to force reloading of file, even if master state has already loaded it for some other record
      */
    LuaSubRecord(struct dbCommon *precord, std::string rec_name, bool force_load): BaseRecord(precord, LIS_LUASUB_REC_TYPE, rec_name, LIS_INP_FIELD_NAME, GetLinkFieldValue(precord), force_load) {}

private:
    /** Class method to retrieve the proper link field value from a luasub record.
      *  
      * \param[in] precord Pointer to EPICS record structure
      *
      * \return String value of luasub record's INP field
      */
    static std::string GetLinkFieldValue(struct dbCommon *precord) {
        struct luasubRecord *lsrec = reinterpret_cast<struct luasubRecord *>(precord);
        return std::string(lsrec->inp.value.instio.string);
    }
};

#endif /* INC_lis_luasub_record_H */
