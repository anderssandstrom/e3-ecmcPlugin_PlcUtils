/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPlcUtilsWrap.h
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*
\*************************************************************************/
#ifndef ECMC_PLC_UTILS_WRAP_H_
#define ECMC_PVA_ECMC_PLC_UTILS_WRAP_H_WRAP_H_

#include "ecmcPvDefs.h"

# ifdef __cplusplus
extern "C" {
# endif  // ifdef __cplusplus
  int    parseConfigStr(char *configStr);
# ifdef __cplusplus
}
# endif  // ifdef __cplusplus

#endif  /* ECMC_PLC_UTILS_WRAP_H_ */
