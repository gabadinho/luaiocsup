#include "lis_common_support.h"

#include "lis_log.h"
#include "lis_mutex.h"

void lisDevSupPhase1Init() {
    NOW_STR;

    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Performing device support phase-1 bindings\n", now_str, LIS_LIB_LOG_NAME);

}

long lisReport(int interest, char *rec_report_type) {
    lisMutexLock(lisRecordsAddDelLock);
    lisMutexUnlock(lisRecordsAddDelLock);

    return LIS_EPICS_SUCCESS;
}

//long lisAddRecord(struct dbCommon *precord, char *db_link_field, char *proc_meth_name) {
long lisAddRecord(struct dbCommon *precord) {
    int res=LIS_EPICS_SUCCESS;
    NOW_STR;

    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Adding record %s\n", now_str, LIS_LIB_LOG_NAME, precord->name);

    lisMutexLock(lisRecordsAddDelLock);
    //res=lisGenerateRecord(precord,db_link_field,LIS_ADD_REC_FUN,proc_meth_name,lisIsIocRunning);
    lisMutexUnlock(lisRecordsAddDelLock);

    return res;
}

int lisDeleteRecord(struct dbCommon *precord) {
    int res=LIS_EPICS_SUCCESS;
    NOW_STR;

    lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Deleting record %s\n", now_str, LIS_LIB_LOG_NAME, precord->name);

    return res;
}

/* Beware: return in the middle of the function! */
long lisGenerateRecord(struct dbCommon *precord, char *db_link_field, char *init_meth_name, char *proc_meth_name, int force_load) {
    NOW_STR;

    return LIS_EPICS_SUCCESS;
}

/* Beware: return in the middle of the function! */
long lisProcess(struct dbCommon *precord, int allowed_types, int *code_type) {
    NOW_STR;
    long res=LIS_EPICS_SUCCESS;
    char *name;
  
    name = precord->name;
    lisLog( LIS_LOGLVL_STANDARD, errlogInfo, now_str, "%s %s Processing record %s\n", now_str, LIS_LIB_LOG_NAME, name);

    return res;
}

/* int after: 0=before_field_update 1=after_field_update */
/* Called when one of LINR,EGUF,EGUL,ROFF is modified */
long lisSpecialLinConv(struct dbCommon *precord, int after) {
    NOW_STR;
    int res=LIS_EPICS_SUCCESS;

    return res;
}

long lisGetIoIntInfo(int cmd, struct dbCommon *precord, IOSCANPVT *ppvt) {
    NOW_STR;
    int res=LIS_EPICS_SUCCESS;

    return res;
}

long lisLegacyInitRecord(struct dbCommon *precord) {
    NOW_STR;
    int res=LIS_EPICS_SUCCESS;

    return res;
}
