[#ftl]
[#assign moduleName = "x-cube-nfc12"]
[#if ModuleName??]
    [#assign moduleName = ModuleName]
[/#if]
/**
  ******************************************************************************
  * File Name          :  app_${moduleName?lower_case}.c
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
  
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rfal_platform.h"
#include "nfc_conf.h"
#include "app_${moduleName?lower_case}.h"
#include "st25r500_irq.h"
#include "logger.h"

[#assign useNFC12POLLINGTAGDETECT = false]



[#if RTEdatas??]
[#list RTEdatas as define]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT")]
[#assign useNFC12POLLINGTAGDETECT = true]
[/#if]


[/#list]
[/#if]

[#if useNFC12POLLINGTAGDETECT]
[@common.optinclude name=mxTmpFolder + "/nfc12a1_PollingTagDetect_demo_gv.tmp"/]
[/#if]



void ${fctName}(void)
{
 /* USER CODE BEGIN SV */ 

 /* USER CODE END SV */


 [#if useNFC12POLLINGTAGDETECT]
 /* USER CODE BEGIN NFC12_Library_Init_PreTreatment */
 
 /* USER CODE END NFC12_Library_Init_PreTreatment */

 /* Initialize the peripherals and the NFC12 components */
 [@common.optinclude name=mxTmpFolder + "/nfc12a1_PollingTagDetect_demo_init.tmp"/]
 /* USER CODE BEGIN SV */ 

 /* USER CODE END SV */
 
 /* USER CODE BEGIN NFC12_Library_Init_PostTreatment */
 
 /* USER CODE END NFC12_Library_Init_PostTreatment */
 [/#if]
  

}
/*
 * LM background task
 */
void ${fctProcessName}(void)
{
  [#if useNFC12POLLINGTAGDETECT]
  /* USER CODE BEGIN NFC12_Library_Process */
  
  /* USER CODE END NFC12_Library_Process */
  /* Run Demo Application */
  demoCycle(); 
  [/#if]
}

[#if useNFC12POLLINGTAGDETECT]
[@common.optinclude name=mxTmpFolder + "/nfc12a1_PollingTagDetect_demo.tmp"/]
[/#if]



#ifdef __cplusplus
}
#endif

