/**
  * \file lis_stringout_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS stringout record.
  * \author Jose C.G. Gabadinho
  *
  * Derived record from BaseRecord, to implement string-output specific details.
  * The base class factory should be use instead of constructors.
  */

#ifndef INC_lis_stringout_record_H
#define INC_lis_stringout_record_H

#include "lis_base_record.h"

#include "lis_epics.h"

#include <stringoutRecord.h>



/** Class wrapper for a 'luaiocsup' stringout record.
  */
class StringoutRecord: public BaseRecord {
public:
    /** Constructor for a 'luaiocrec' ai record.
      * Parses INP/OUT field link value, creates Lua states, etc.
      *  
      * \param[in] precord Pointer to EPICS record structure
      * \param[in] rec_name Record name
      * \param[in] force_load true to force reloading of file, even if master state has already loaded it for some other record
      */
    StringoutRecord(struct dbCommon *precord, std::string rec_name, bool force_load): BaseRecord(precord, LIS_STRINGOUT_REC_TYPE, rec_name, LIS_OUT_FIELD_NAME, GetLinkFieldValue(precord), force_load) {}

private:
    /** Class method to retrieve the proper link field value from a stringout record.
      *  
      * \param[in] precord Pointer to EPICS record structure
      *
      * \return String value of stringout record's OUT field
      */
    static std::string GetLinkFieldValue(struct dbCommon *precord) {
        struct stringoutRecord *sorec = reinterpret_cast<struct stringoutRecord *>(precord);
        return std::string(sorec->out.value.instio.string);
    }
};

#endif /* INC_lis_stringout_record_H */
