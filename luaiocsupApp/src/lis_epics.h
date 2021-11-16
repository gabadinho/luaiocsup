/**
  * \file lis_epics.h
  * \brief Definitions (mostly macro directives) for EPICS features.
  * \author Jose C.G. Gabadinho
  */

#ifndef INC_lis_epics_H
#define INC_lis_epics_H

/** Success return-value for device- and record-support methods.
  * Exception: process() method for analog-input and -output records.
  */
#define LIS_DEVSUP_SUCCESS 0

#define LIS_INP_FIELD_NAME  "INP"
#define LIS_OUT_FIELD_NAME  "OUT"

#endif /* INC_lis_epics_H */
