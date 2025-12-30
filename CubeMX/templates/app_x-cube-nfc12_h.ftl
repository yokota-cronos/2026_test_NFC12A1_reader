[#ftl]
[#assign moduleName = "x-cube-nfcx"]
[#if ModuleName??]
	[#assign moduleName = ModuleName]
[/#if]
/**
  ******************************************************************************
  * File Name          :  ${name?replace(".","_")?lower_case}.h
  * Description        : This file provides code for the configuration
  *                      of the ${name} instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) ${year} STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_${moduleName?upper_case?replace("-","_")}_H
#define APP_${moduleName?upper_case?replace("-","_")}_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nfc_conf.h"
/* Exported Functions --------------------------------------------------------*/

void _Error_Handler(char *file, int line);
void ${fctName}(void);
void ${fctProcessName}(void);
void BSP_NFC0XCOMM_IRQ_Callback(void);

#ifdef __cplusplus
}
#endif
#endif /* __INIT_H */

