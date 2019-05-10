#ifndef INC_lis_common_support_H
#define INC_lis_common_support_H

#include "lis_globals.h"
#include "lis_data_structs.h"

void lisDevSupPhase1Init();

long lisReport(int interest,char *rec_report_type);

long lisAddRecord(struct dbCommon *precord,char *db_link_field,char *proc_meth_name);
int  lisDeleteRecord(struct dbCommon *precord);

long lisGenerateRecord(struct dbCommon *precord,char *db_link_field,char *init_meth_name,char *proc_meth_name,int force_load);

long lisProcess(struct dbCommon *precord,int allowed_types,int *code_type);

long lisSpecialLinConv(struct dbCommon *precord,int after);

long lisGetIoIntInfo(int cmd,struct dbCommon *precord,IOSCANPVT *ppvt);

long lisLegacyInitRecord(struct dbCommon *precord);

#endif /* INC_lis_common_support_H */
