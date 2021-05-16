#ifndef INC_lis_base_record_H
#define INC_lis_base_record_H

#include <string>
#include <map>
#include <vector>
#include <exception>

#include <dbCommon.h>
#include <epicsMutex.h>
#include <devSup.h>

class BaseRecord {
public:
    static BaseRecord * CreateRecord(struct dbCommon *precord, std::string db_link_field, std::string proc_meth_name);

/*
    virtual long Process(int allowed_types, int *code_type);

    virtual long SpecialLinConv(int after) { throw std::logic_error("Not implemented!"); }

    long GetIoIntInfo(int cmd, IOSCANPVT *ppvt);
*/

protected:
    //BaseRecord(struct dbCommon *precord, std::string db_link_field, std::string init_meth_name, std::string proc_meth_name, bool force_load);

    BaseRecord(struct dbCommon *precord, bool force_load);

    std::string recordName, recordType;

    struct dbCommon *pRecord;

    static epicsMutexId recordsAddDelLock;
    static std::map<std::string, std::vector<BaseRecord&>&> allRecords;
};

#endif /* INC_lis_base_record_H */
