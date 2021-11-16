/**
  * \file lis_ai_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS ai record.
  * \author Jose C.G. Gabadinho
  *
  * Derived record from BaseRecord, to implement analog-input specific details.
  * The base class factory should be use instead of constructors.
  */

#ifndef INC_lis_ai_record_H
#define INC_lis_ai_record_H

#include "lis_base_record.h"

#include "lis_epics.h"

#include <aiRecord.h>



/** Class wrapper for a 'luaiocsup' ai record.
  */
class AiRecord: public BaseRecord {
public:
    /** Constructor for a 'luaiocrec' ai record.
      * Parses INP/OUT field link value, creates Lua states, etc.
      *  
      * \param[in] precord Pointer to EPICS record structure
      * \param[in] rec_name Record name
      * \param[in] force_load true to force reloading of file, even if master state has already loaded it for some other record
      */
    AiRecord(struct dbCommon *precord, std::string rec_name, bool force_load): BaseRecord(precord, LIS_AI_REC_TYPE, rec_name, LIS_INP_FIELD_NAME, GetLinkFieldValue(precord), force_load) {}

    /** Called for records that support the linear convertion hook, ai and ao, when one of LINR,
      * EGUF, EGUL, ROFF is modified.
      *
      * \param[in] after Set to 0 before field is updated, set to 1 after field is updated
      *
      * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
      */
    long SpecialLinConv(int after);

private:
    /** Class method to retrieve the proper link field value from an ai record.
      *  
      * \param[in] precord Pointer to EPICS record structure
      *
      * \return String value of ai record's INP field
      */
    static std::string GetLinkFieldValue(struct dbCommon *precord) {
        struct aiRecord *airec = reinterpret_cast<struct aiRecord *>(precord);
        return std::string(airec->inp.value.instio.string);
    }
};

#endif /* INC_lis_ai_record_H */
