/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPluginPlcUtils.c
*
*  Created on: Mar 21, 2020
*      Author: anderssandstrom
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN
#define ECMC_EXAMPLE_PLUGIN_VERSION 2

#ifdef __cplusplus
extern "C" {
#endif  // ifdef __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecmcPluginDefs.h"
#include "ecmcPlcUtilsWrap.h"
#include "ecmcPluginClient.h"
#include "ecmcPvDefs.h"

// only allow to load once
#define ECMC_PLUGIN_ALLOW_MULTI_LOAD 0

// Error codes
#define ECMC_PLUGIN_ERROR_ALREADY_LOADED 1

static int    lastEcmcError = 0;
static char*  lastConfStr   = NULL;
static int    loaded = 0;

extern struct ecmcPluginData pluginDataDef;

/** Optional. 
 *  Will be called once after successfull load into ecmc.
 *  Return value other than 0 will be considered error.
 *  configStr can be used for configuration parameters.
 **/
int plcUtilsConstruct(char *configStr)
{
  //This module is only allowed to load once  
  if(loaded && !ECMC_PLUGIN_ALLOW_MULTI_LOAD) {
    printf("%s/%s:%d: Error: Module already loaded (0x%x).\n",__FILE__, __FUNCTION__,
           __LINE__,ECMC_PLUGIN_ERROR_ALREADY_LOADED);
    return ECMC_PLUGIN_ERROR_ALREADY_LOADED;
  }
  // create plcUtils object and register data callback
  lastConfStr = strdup(configStr);
  parseConfigStr(lastConfStr);

  // Add refs to generic funcs in runtime since objects
  pluginDataDef.funcs[0].funcGenericObj = getPvRegObj();  
  loaded = 1;
  return initPvs();
}

/** Optional function.
 *  Will be called once at unload.
 **/
void plcUtilsDestruct(void)
{
  cleanup();
  if(lastConfStr){
    free(lastConfStr);
  }
}

/** Optional function.
 *  Will be called each realtime cycle if definded
 *  ecmcError: Error code of ecmc. Makes it posible for 
 *  this plugin to react on ecmc erro-I/epics/base-7.0.3.1/include/os/Linuxrs
 *  Return value other than 0 will be considered to be an error code in ecmc.
 **/
int plcUtilsRealtime(int ecmcError)
{ 
  lastEcmcError = ecmcError;
  return 0;
}

/** Link to data source here since all sources should be availabe at this stage
 *  (for example ecmc PLC variables are defined only at enter of realtime)
 **/
int plcUtilsEnterRT(){
  return 0;
}

/** Optional function.
 *  Will be called once just before leaving realtime mode
 *  Return value other than 0 will be considered error.
 **/
int plcUtilsExitRT(void){
  return 0;
}

// Register data for plugin so ecmc know what to use
struct ecmcPluginData pluginDataDef = {
  // Allways use ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
  // Name 
  .name = "ecmcPlugin_PlcUtils",
  // Description
  .desc = "Plc Utils plugin for use with ecmc.",
  // Option description
  .optionDesc = ECMC_PV_OPTION_MAX_PV_COUNT"",
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // Optional construct func, called once at load. NULL if not definded.
  .constructFnc = plcUtilsConstruct,
  // Optional destruct func, called once at unload. NULL if not definded.
  .destructFnc = plcUtilsDestruct,
  // Optional func that will be called each rt cycle. NULL if not definded.
  .realtimeFnc = plcUtilsRealtime,
  // Optional func that will be called once just before enter realtime mode
  .realtimeEnterFnc = plcUtilsEnterRT,
  // Optional func that will be called once just before exit realtime mode
  .realtimeExitFnc = plcUtilsExitRT,
  // PLC funcs
  .funcs[0] =
      { /*----pv_reg_async----*/
        .funcName = ECMC_PV_PLC_CMD_PV_REG_ASYN,
        .funcDesc = "handle = " ECMC_PV_PLC_CMD_PV_REG_ASYN "(<pv name>, <provider name pva/ca>) : register new pv.",
        .funcArg0 = NULL,
        .funcArg1 = NULL,
        .funcArg2 = NULL,
        .funcArg3 = NULL,
        .funcArg4 = NULL,
        .funcArg5 = NULL,
        .funcArg6 = NULL,
        .funcArg7 = NULL,
        .funcArg8 = NULL,
        .funcArg9 = NULL,
        .funcArg10 = NULL,        
        .funcGenericObj = NULL,  //will be assigned here during plugin construct (cannot initiate with non-const)
      },
  .funcs[1]  = {0}, // last element set all to zero..
  .consts[0] = {0}, // last element set all to zero..
};

ecmc_plugin_register(pluginDataDef);

# ifdef __cplusplus
}
# endif  // ifdef __cplusplus
