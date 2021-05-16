#include "lis_base_record.h"
#include "lis_mutex.h"
#include "lis_ai_record.h"

#include <cstring>

#include <dbBase.h>

epicsMutexId BaseRecord::recordsAddDelLock = lisMutexCreate();

std::map<std::string, std::vector<BaseRecord&>&> BaseRecord::allRecords;

BaseRecord * BaseRecord::CreateRecord(struct dbCommon *precord, std::string db_link_field, std::string proc_meth_name) {
    char *rec_type = precord->rdes->name;

    if (!std::strcmp(rec_type, "ai")) { // GET CONSTANT FOR THIS!!!
        return new AiRecord(precord, false);
    }

    return nullptr;
}

//BaseRecord::BaseRecord(struct dbCommon *precord, std::string db_link_field, std::string init_meth_name, std::string proc_meth_name, bool force_load) {
//}

BaseRecord::BaseRecord(struct dbCommon *precord, bool force_load) {
}
