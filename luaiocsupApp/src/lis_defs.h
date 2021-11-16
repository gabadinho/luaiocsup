/**
  * \file lis_defs.h
  * \brief Definitions (mostly macro directives) for Lua IOC Support features.
  * \author Jose C.G. Gabadinho
  */

#ifndef INC_lis_defs_H
#define INC_lis_defs_H

/**
 * \enum luaCodeType 
 * \brief The possible types of Lua string-encoded code types to execute: function name, code chunk, or both.
 */
enum luaCodeType {
    lctUNKNOWN = -1,
    lctANY,
    lctFUNCTION_NAME,
    lctCODE_CHUNK
};

/** Record names. */
#define LIS_AI_REC_NAME        "AI"
#define LIS_AO_REC_NAME        "AO"
#define LIS_BI_REC_NAME        "BI"
#define LIS_BO_REC_NAME        "BO"
#define LIS_MBBI_REC_NAME      "MBBI"
#define LIS_MBBO_REC_NAME      "MBBO"
#define LIS_STRINGIN_REC_NAME  "STRINGIN"
#define LIS_STRINGOUT_REC_NAME "STRINGOUT"
#define LIS_LONGIN_REC_NAME    "LONGIN"
#define LIS_LONGOUT_REC_NAME   "LONGOUT"
#define LIS_WAVEFORM_REC_NAME  "WAVEFORM"
#define LIS_LUASUB_REC_NAME    "LUASUB"

/** Record type names. */
#define LIS_AI_REC_TYPE        "ai"
#define LIS_AO_REC_TYPE        "ao"
#define LIS_BI_REC_TYPE        "bi"
#define LIS_BO_REC_TYPE        "bo"
#define LIS_MBBI_REC_TYPE      "mbbi"
#define LIS_MBBO_REC_TYPE      "mbbo"
#define LIS_STRINGIN_REC_TYPE  "stringin"
#define LIS_STRINGOUT_REC_TYPE "stringout"
#define LIS_LONGIN_REC_TYPE    "longin"
#define LIS_LONGOUT_REC_TYPE   "longout"
#define LIS_WAVEFORM_REC_TYPE  "waveform"
#define LIS_LUASUB_REC_TYPE    "luasub"

#endif /* INC_lis_defs_H */
