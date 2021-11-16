/**
  * \file lis_common_support.cpp
  * \brief Functions that bridge standard-C EPICS device-support and Lua IOC Support C++ classes.
  * \author Jose C.G. Gabadinho
  */

#include "lis_common_support.h"

#include <cstring>
#include <stdlib.h>

#include "lis_log.h"
#include "lis_epics.h"
#include "lis_defs.h"
#include "lis_base_record.h"

#include <dbCommon.h>
#include <dbBase.h>
#include <dbAccessDefs.h>



void lisDevSupInit(int phase) {
    DECL_NOW_STR;

    if (phase == 0) {
        if (lisBaseDirectory == NULL) {
            BaseRecord::SetConfiguration("", lisMinStackSize, lisDebugLevel);            
        } else {
            BaseRecord::SetConfiguration(std::string(lisBaseDirectory), lisMinStackSize, lisDebugLevel);
        }
    } else {
        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Performing device support phase-1 bindings\n", now_str, LIS_LIB_LOG_NAME);
        // DO STUFF HERE!!!
    }
}

long lisReport(int interest, char *rec_report_type) {
    BaseRecord::ReportRecords(interest, std::string(rec_report_type));
    return LIS_DEVSUP_SUCCESS;
}

long lisAddRecord(struct dbCommon *precord) {
    int res = LIS_DEVSUP_SUCCESS;
    DECL_NOW_STR;
    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Adding <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);

    precord->dpvt = BaseRecord::CreateRecord(precord, lisIsIocRunning); // watch out for exceptions!

    return res;
}

long lisDeleteRecord(struct dbCommon *precord) {
    DECL_NOW_STR;
    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Deleting <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
    return BaseRecord::DeleteRecord(precord);
}

long lisProcess(struct dbCommon *precord, int allowed_type, int *type_found) {
    long res = LIS_DEVSUP_SUCCESS;
    DECL_NOW_STR;

    if (precord->dpvt) {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Processing <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        BaseRecord *lis_rec=static_cast<BaseRecord*>(precord->dpvt);
        res = lis_rec->Process(static_cast<luaCodeType>(allowed_type), nullptr);
    } else {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Unable to process misconfigured <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        res = S_db_notInit;
    }

    return res;
}

long lisSpecialLinConv(struct dbCommon *precord, int after) {
    DECL_NOW_STR;
    int res = LIS_DEVSUP_SUCCESS;

    if (precord->dpvt) {
        BaseRecord *lis_rec=static_cast<BaseRecord*>(precord->dpvt);
        res = lis_rec->SpecialLinConv(after);
    } else {
        res = S_db_notInit;
    }

    return res;
}

long lisGetIoIntInfo(int cmd, struct dbCommon *precord, IOSCANPVT *ppvt) {
    DECL_NOW_STR;
    int res = LIS_DEVSUP_SUCCESS;

    if (precord->dpvt) {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Retrieving I/O Intr info from <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        BaseRecord *lis_rec=static_cast<BaseRecord*>(precord->dpvt);
        res = lis_rec->GetIoIntInfo(cmd, ppvt);
    } else {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Unable to retrieve I/O Intr info from misconfigured <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        res = S_db_notInit;
    }

    return res;
}

long lisLegacyInitRecord(struct dbCommon *precord) {
    DECL_NOW_STR;
    int res = LIS_DEVSUP_SUCCESS;

    if (precord->dpvt) {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Initializing <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        BaseRecord *lis_rec=static_cast<BaseRecord*>(precord->dpvt);
        res = lis_rec->LegacyInit();
    } else {
        lisLog(LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Unable to initialize misconfigured <%s> record '%s'\n", now_str, LIS_LIB_LOG_NAME, precord->rdes->name, precord->name);
        res = S_db_notInit;
    }

    return res;
}

