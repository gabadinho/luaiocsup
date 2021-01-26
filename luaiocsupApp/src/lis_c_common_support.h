#ifndef INC_lis_c_common_support_H
#define INC_lis_c_common_support_H

#include <dbCommon.h>
#include <dbScan.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void lisDevSupPhase1Init();

long lisReport(int interest, char *rec_report_type);

long lisAddRecord(struct dbCommon *precord, char *db_link_field, char *proc_meth_name);
int  lisDeleteRecord(struct dbCommon *precord);

long lisGenerateRecord(struct dbCommon *precord, char *db_link_field, char *init_meth_name, char *proc_meth_name, int force_load);

long lisProcess(struct dbCommon *precord, int allowed_types, int *code_type);

long lisSpecialLinConv(struct dbCommon *precord, int after);

long lisGetIoIntInfo(int cmd, struct dbCommon *precord, IOSCANPVT *ppvt);

long lisLegacyInitRecord(struct dbCommon *precord);

#ifdef __cplusplus
}
#endif

#endif /* INC_lis_c_common_support_H */
