#ifndef INC_lis_ai_record_H
#define INC_lis_ai_record_H

#include "lis_base_record.h"

class AiRecord: public BaseRecord {
public:

    virtual long SpecialLinConv(int after);

    AiRecord(struct dbCommon *precord, bool force_load):BaseRecord(precord,force_load) {}

};

#endif /* INC_lis_ai_record_H */
