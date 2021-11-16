/**
  * \file lis_c_common_support.h
  * \brief Functions that bridge standard-C EPICS device-support and Lua IOC Support C++ classes.
  * \author Jose C.G. Gabadinho
  */

#ifndef INC_lis_c_common_support_H
#define INC_lis_c_common_support_H

#include <dbCommon.h>
#include <devSup.h>



#ifdef __cplusplus
extern "C" {
#endif

/** Initializes the Lua IOC Support.
  * To avoid being called by every single device-support implemented, this is called only
  * by the initialization hook of 'luasub' record support.
  *
  * \param[in] phase Current IOC initialization phase, according to EPICS implementation
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
void lisDevSupInit(int phase);

/** Called when iocsh 'dbior' command is executed.
  *
  * Reports information for a device-support record type.
  *
  * \param[in] interest Set to 0 for 1-liner report, >0 for more detailed information
  * \param[in] rec_report_type Which 'luaiocsup' record type to report
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisReport(int interest, char *rec_report_type);

/** New record being added to the database; extended device-support implementation.
  *
  * Can be called during IOC initialization or also during runtime if a user updates
  * either the INP or OUT field of a 'luaiocsup' record.
  *
  * Possible tasks: validate INP/OUT field, check for hardware presence, initialize device,
  * reserve memory, etc.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisAddRecord(struct dbCommon *precord);

/** Record is to be deleted; extended device-support implementation.
  * Called during IOC runtime if a user updates either the INP or OUT field of a
  * 'luaiocsup' record.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisDeleteRecord(struct dbCommon *precord);

/** Called when the record is to be processed.
  *
  * \param[in] precord Pointer to common record structure
  * \param[in] allowed_type Allowed type of Lua code to execute (function name, code chunk, both)
  *
  * \param[out] type_found Type of executed code (function name or code chunk)
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  *         Exceptions: in records that use the linear convertion, ai and ao, if this process
  *                     function returns 0 then VAL is converted according to LINR field; to
  *                     prevent this, return value should be 2
  */
long lisProcess(struct dbCommon *precord, int allowed_type, int *type_found);

/** Called for records that support the linear convertion hook, ai and ao, when one of LINR,
  * EGUF, EGUL, ROFF is modified.
  *
  * \param[in] precord Pointer to common record structure
  * \param[in] after Set to 0 before field is updated, set to 1 after field is updated
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisSpecialLinConv(struct dbCommon *precord, int after);

/** Returns which IOSCANPVT (interrupt source) is associated with the record
  * when its scan rate is set to I/O Intr.
  *
  * \param[in] cmd Set to 0 when SCAN is set to I/O Intr, set to 1 when SCAN is
  *                set to any other value
  * \param[in] precord Pointer to common record structure
  *
  * \param[out] ppvt Updated IOSCANPVT structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisGetIoIntInfo(int cmd, struct dbCommon *precord, IOSCANPVT *ppvt);

/** Called after adding the record and before the IOC starts running.
  *
  * Not called when INP/OUT fields are updated while IOC is running.
  *
  * Possible tasks (if not performed by the addRecord hook): validade INP/OUT
  * field, check for hardware presence, initialize device, reserve memory, etc.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
long lisLegacyInitRecord(struct dbCommon *precord);

#ifdef __cplusplus
}
#endif

#endif /* INC_lis_c_common_support_H */
