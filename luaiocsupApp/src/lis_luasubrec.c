/**
  * \file lis_luasubrec.c
  * \brief Record-support implementation for a luaiocsup EPICS luasub record.
  * \author Jose C.G. Gabadinho
  *
  * Generic, with multiple inputs and outputs, luaiocsub record.
  */

#define GEN_SIZE_OFFSET
#include "lis_luasubrec.h"
#undef  GEN_SIZE_OFFSET

#include "lis_log.h"
#include "lis_common_support.h"
#include "lis_defs.h"
#include "lis_epics.h"

#include <dbCommon.h>
#include <recSup.h>
#include <devSup.h>
#include <dbAddr.h>
#include <epicsExport.h>



/** Record-support function prototypes. */
static long luasubInitRecord(struct dbCommon *, int);
static long luasubProcess(struct dbCommon *);
static long luasubSpecial(DBADDR *, int);
static long luasubCvtDbAddr(DBADDR *);
static long luasubGetArrayInfo(DBADDR *, long *, long *);
static long luasubPutArrayInfo(DBADDR *, long);
static long luasubGetPrecision(const DBADDR *, long *);

/** Declare and export record-support structure. */
struct typed_rset luasubRSET = {
    RSETNUMBER,
    NULL,
    NULL,
    luasubInitRecord,
    luasubProcess,
    luasubSpecial,
    NULL,
    luasubCvtDbAddr,
    luasubGetArrayInfo,
    luasubPutArrayInfo,
    NULL,
    luasubGetPrecision,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
epicsExportAddress(rset, luasubRSET);

/** Device-support function prototypes. */
static long luasubReport(int);
static long luasubInitDevSup(int);

/** Declare and export device-support structure. */
dset lisLUASUB = {
    4, /* Space for 4 functions */
    luasubReport,
    luasubInitDevSup,
    NULL,
    lisGetIoIntInfo
};
epicsExportAddress(dset, lisLUASUB);

/** Extented device-support function prototypes. */
static long luasubAddRecord(struct dbCommon *);
static long luasubDelRecord(struct dbCommon *);

/** Extended record-support structure. */
static struct dsxt lisDsxtLUASUB = {
    luasubAddRecord,
    luasubDelRecord
};



/** Declare input and output fields. */
#define NUM_IO_FIELDS 26

static const char *luasubInpValFields[NUM_IO_FIELDS] = {
    "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N",
    "O", "P", "Q", "R", "S", "T", "U",
    "V", "W", "X", "Y", "Z"
};

static const char *luasubOutValFields[NUM_IO_FIELDS] = {
    "VALA", "VALB", "VALC", "VALD", "VALE", "VALF", "VALG",
    "VALH", "VALI", "VALJ", "VALK", "VALL", "VALM", "VALN",
    "VALO", "VALP", "VALQ", "VALR", "VALS", "VALT", "VALU",
    "VALV", "VALW", "VALX", "VALY", "VALZ"
};



/** Called after adding the record and before the IOC starts running.
  *
  * Not called when INP/OUT fields are updated while IOC is running.
  *
  * Possible tasks (if not performed by the addRecord hook): validade INP/OUT
  * field, check for hardware presence, initialize device, reserve memory, etc.
  *
  * \param[in] precord Pointer to common record structure
  * \param[in] pass 0 for record-only initialization, 1 when other records are available
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubInitRecord(struct dbCommon *precord, int pass) {
    if (pass == 1) {
        return lisLegacyInitRecord(precord);
    }
    return LIS_DEVSUP_SUCCESS;
}

/** Called when the record is to be processed.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubProcess(struct dbCommon *precord) {  
    return lisProcess(precord, lctANY, NULL);
}

/** Called before and after a special(SPC_MOD) field is modified.
  *
  * \param[in] paddr Pointer to record field
  * \param[in] after 0 for before change, 1 for after
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubSpecial(DBADDR *paddr, int after) {
    return LIS_DEVSUP_SUCCESS;
}

/** Retrieve information about an array element.
  *
  * \param[in] paddr Pointer to record field
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubCvtDbAddr(DBADDR *paddr) {
    return LIS_DEVSUP_SUCCESS;
}

/** Returns the number of elements and offset of the first element for an array.
  *
  * \param[in] paddr Pointer to record field
  *
  * \param[out] no_elements Number of elements
  * \param[out] offset Offset for first array (for circular buffers)
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubGetArrayInfo(DBADDR *paddr, long *no_elements, long *offset) {
    return LIS_DEVSUP_SUCCESS;
}

/** Called when new values are put into an array.
  *
  * \param[in] paddr Pointer to record field
  * \param[in] n_new Number of new elements
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubPutArrayInfo(DBADDR *paddr, long n_new) {
    return LIS_DEVSUP_SUCCESS;
}

/** Returns the number of significant decimals places.
  *
  * \param[in] paddr Pointer to record field
  *
  * \param[out] precision The precision for ASCII convertion
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubGetPrecision(const DBADDR *paddr, long *precision) {
    return LIS_DEVSUP_SUCCESS;
}

/** Called when iocsh 'dbior' command is executed.
  *
  * Reports information for a device-support record type.
  *
  * \param[in] interest Set to 0 for 1-liner report, >0 for more detailed information
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubReport(int interest) {
    if (interest) {
        lisLogSimple("  Lua IOC Support scripts directory: %s\n", lisBaseDirectory);
    }
    return lisReport(interest, LIS_LUASUB_REC_NAME);
}

/** Initializes the luasub device support.
  *
  * \param[in] phase Current IOC initialization phase, according to EPICS implementation
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubInitDevSup(int phase) {
    DECL_NOW_STR;

    if (phase == 0) {
        lisLog(LIS_LOGLVL_VERBOSE, errlogInfo, now_str, "%s %s Initializing %s support\n", now_str, LIS_LIB_LOG_NAME, LIS_LUASUB_REC_NAME);
        devExtend(&lisDsxtLUASUB);
    }
    lisDevSupInit(phase);

    return LIS_DEVSUP_SUCCESS;
}

/** New record being added to the database; extended device-support implementation.
  *
  * Can be called during IOC initialization or also during runtime if a user updates
  * the INP field of a luasub 'luaiocsup' record.
  *
  * Possible tasks: validate INP/OUT field, check for hardware presence, initialize device,
  * reserve memory, etc.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubAddRecord(struct dbCommon *precord) {
    return lisAddRecord(precord);
}

/** Record is to be deleted; extended device-support implementation.
  * Called during IOC runtime if a user updates either the INP field of a
  * luasub 'luaiocsup' record.
  *
  * \param[in] precord Pointer to common record structure
  *
  * \return LIS_DEVSUP_SUCCESS for success, or an error: S_db_* S_dev_* etc.
  */
static long luasubDelRecord(struct dbCommon *precord) {
    return lisDeleteRecord(precord);
}
