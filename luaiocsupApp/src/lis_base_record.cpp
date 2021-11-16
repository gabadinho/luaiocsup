/**
  * \file lis_base_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS record.
  * \author Jose C.G. Gabadinho
  */

#include "lis_base_record.h"

#include <cstring>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <locale>

#include "lis_log.h"

#include "lis_ai_record.h"
#include "lis_stringout_record.h"
#include "lis_luasub_record.h"

#include <dbBase.h>
#include <epicsMutex.h>



std::map<std::string, std::unique_ptr<std::vector<std::unique_ptr<BaseRecord>>>> BaseRecord::LuaRecords;
epicsMutexId BaseRecord::LuaRecordsLock = epicsMutexCreate();

std::string BaseRecord::BaseDirectory;
int BaseRecord::DebugLevel;
int BaseRecord::MinStackSize;



BaseRecord * BaseRecord::CreateRecord(struct dbCommon *precord, bool force_file_load) {
    char *rec_type = precord->rdes->name;
    std::string rec_name(precord->name);
    std::unique_ptr<BaseRecord> new_rec;
    std::string rec_type_key(rec_type);
    BaseRecord *new_rec_ptr=nullptr;
    std::vector<std::unique_ptr<BaseRecord>>::iterator inserted_rec;

    epicsMutexLock(LuaRecordsLock);

    // catch bad_alloc exceptions below!!!
    if (!std::strcmp(rec_type, LIS_AI_REC_TYPE)) {
        new_rec = std::make_unique<AiRecord>(precord, rec_name, false);
    } else if (!std::strcmp(rec_type, LIS_STRINGOUT_REC_TYPE)) {
        new_rec = std::make_unique<StringoutRecord>(precord, rec_name, false);
    } else if (!std::strcmp(rec_type, LIS_LUASUB_REC_TYPE)) {
        new_rec = std::make_unique<LuaSubRecord>(precord, rec_name, false);
    }

    if (new_rec) {

        new_rec_ptr = new_rec.get();
        auto rec_type_records_it = LuaRecords.find(rec_type_key);
        if (rec_type_records_it == LuaRecords.end()) {
            auto rec_type_records = std::make_unique<std::vector<std::unique_ptr<BaseRecord>>>();
            inserted_rec = rec_type_records->insert(rec_type_records->end(), std::move(new_rec));
            LuaRecords[rec_type_key] = std::move(rec_type_records);

        } else {
            inserted_rec = rec_type_records_it->second->insert(rec_type_records_it->second->end(), std::move(new_rec));
        }

    }

    epicsMutexUnlock(LuaRecordsLock);

    return new_rec_ptr;
}

long BaseRecord::DeleteRecord(struct dbCommon *precord) {
    char *rec_type = precord->rdes->name;
    std::string rec_type_key(rec_type);
    std::string rec_name(precord->name);
    

    epicsMutexLock(LuaRecordsLock);

    auto rec_type_records_it = LuaRecords.find(rec_type_key);
    if (rec_type_records_it == LuaRecords.end()) {
        // ERROR!!!
    } else {
        rec_type_records_it->second->size();

        auto to_remove = std::remove_if(
            rec_type_records_it->second->begin(),
            rec_type_records_it->second->end(),
            [rec_name](const std::unique_ptr<BaseRecord>& rec_ptr){return (rec_ptr->recordType.second == rec_name);}
        );
        rec_type_records_it->second->erase(to_remove);
    }


    epicsMutexUnlock(LuaRecordsLock);
    return 0; // GET CONSTANT FOR THIS!!!
}

void BaseRecord::ReportRecords(int interest, std::string rec_type) {
    for (char& c: rec_type) {
        c = ::tolower(c);
    }

    epicsMutexLock(LuaRecordsLock);

    auto rec_vec_it = LuaRecords.find(rec_type);
    if (rec_vec_it != LuaRecords.end()) {
        for (size_t i=0; i<rec_vec_it->second->size(); ++i) {
                
            lisLogSimple("  %s.%s \"%s\"\n",
                rec_vec_it->second->at(i)->recordType.second.c_str(),
                rec_vec_it->second->at(i)->recordConfig.first.c_str(),
                rec_vec_it->second->at(i)->recordConfig.second.c_str());

            if (interest) {
            }

        }
    }

    epicsMutexUnlock(LuaRecordsLock);
}

BaseRecord::BaseRecord(struct dbCommon *precord, std::string rec_type, std::string rec_name, std::string link_field, std::string link_value, bool force_load) {
    this->pRecord = precord;
    this->recordType.first  = rec_type;
    this->recordType.second = rec_name;

    this->recordConfig.first  = link_field;
    this->recordConfig.second = link_value;

    std::string filename, master_id, table_name;
    ParseLinkField(link_value, filename, master_id, table_name);

    this->syncProcRuntime = std::make_unique<LuaRuntime>(master_id, BaseDirectory, filename, force_load, MinStackSize);
    this->asyncCallbackRuntime = std::make_unique<LuaRuntime>(master_id, MinStackSize);

    // INITIALIZE MASTER HERE!!!

    this->asyncCallbackRuntime->unlock();
    this->syncProcRuntime->unlockWithMaster();
}

long BaseRecord::Process(luaCodeType allowed_types, luaCodeType *code_type) {
    DECL_NOW_STR;
    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s BaseRecord::Process\n", now_str, LIS_LIB_LOG_NAME);
    return 0;
}

long BaseRecord::GetIoIntInfo(int cmd, IOSCANPVT *ppvt) {
    DECL_NOW_STR;
    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s BaseRecord::GetIoIntInfo\n", now_str, LIS_LIB_LOG_NAME);
    return -1;
}


long BaseRecord::LegacyInit() {
    DECL_NOW_STR;
    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s BaseRecord::LegacyInit\n", now_str, LIS_LIB_LOG_NAME);
    return 0;
}

bool BaseRecord::ParseLinkField(std::string link_value, std::string& filename, std::string& state_id, std::string& table_name) {
    size_t pos;
    pos = link_value.find(' ');
    if (pos != std::string::npos) {
        filename = link_value.substr(0, pos);
    } else {
        filename = link_value;
    }

    state_id = filename;

    return false;
}
