/**
  * \file lis_ai_record.h
  * \brief O.O. abstraction for a luaiocsup EPICS ai record.
  * \author Jose C.G. Gabadinho
  */

#include "lis_ai_record.h"

#include "lis_epics.h"



long AiRecord::SpecialLinConv(int after) {
    return LIS_DEVSUP_SUCCESS;
}
