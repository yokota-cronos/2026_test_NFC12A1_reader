[#ftl]
/**
  ******************************************************************************
  * @file           : nfc_conf.h
  * @brief          : This file contains definitions for the NFC12 components bus interfaces
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
[#assign IpInstance = ""]
[#assign IpName = ""]
[#assign ExtiLine = ""]
[#assign IrqNumber = ""]
[#assign NFC0XCOMMInstance = ""]
[#assign LED_A_PORT = ""]
[#assign LED_A_PIN = ""]
[#assign LED_B_PORT = ""]
[#assign LED_B_PIN = ""]
[#assign LED_F_PORT = ""]
[#assign LED_F_PIN = ""]
[#assign LED_V_PORT = ""]
[#assign LED_V_PIN = ""]
[#assign LED_FIELD_PORT = ""]
[#assign LED_FIELD_PIN = ""]
[#assign NFC_CS_PORT = ""]
[#assign NFC_CS_PIN = ""]
[#assign NFC_RST_PORT = ""]
[#assign NFC_RST_PIN = ""]
[#assign NFC_INT_PORT = ""]
[#assign NFC_INT_PIN = ""]
[#assign NFC_INT_EXTI_LINE = ""]
[#assign NFC_INT_EXTI_IRQn = ""]
[#assign TESTED_VAR = true]
[#assign RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN_TEST = "0x10"]

[#if BspIpDatas??]
  [#list BspIpDatas as SWIP] 
	[#if SWIP.variables??]
		[#list SWIP.variables as variables]
			[#if variables.name?contains("IpInstance")]
				[#assign IpInstance = variables.value]
			[/#if]
			[#if variables.name?contains("IpName")]
				[#assign IpName = variables.value]
			[/#if]
			[#if variables.value?contains("NFC12A1 BUS IO driver")]
				[#assign NFC0XCOMMInstance = IpInstance]
			[/#if]
			[#if variables.name?contains("GPIO_INT_NUM")]
				[#assign IrqNumber = variables.value]
			[/#if]
			[#if variables.name?contains("EXTI_LINE_NUMBER")]
				[#assign ExtiLine = variables.value]
			[/#if]		
			[#if variables.value?contains("LED_A")]
				[#assign LED_A_PORT = IpName]
				[#assign LED_A_PIN = IpInstance]				
			[/#if]
			[#if variables.value?contains("LED_B")]
				[#assign LED_B_PORT = IpName]
				[#assign LED_B_PIN = IpInstance]				
			[/#if]
			[#if variables.value?contains("LED_F")]
				[#assign LED_F_PORT = IpName]
				[#assign LED_F_PIN = IpInstance]				
			[/#if]	
			[#if variables.value?contains("LED_V")]
				[#assign LED_V_PORT = IpName]
				[#assign LED_V_PIN = IpInstance]				
			[/#if]
			[#if variables.value?contains("LED_FIELD")]
				[#assign LED_FIELD_PORT = IpName]
				[#assign LED_FIELD_PIN = IpInstance]				
			[/#if]	
			[#if variables.value?contains("NFC CS PIN")]
				[#assign NFC_CS_PORT = IpName]
				[#assign NFC_CS_PIN = IpInstance]				
			[/#if]
		    [#if variables.value?contains("NFC RST PIN")]
				[#assign NFC_RST_PORT = IpName]
				[#assign NFC_RST_PIN = IpInstance]				
			[/#if]
			[#if variables.value?contains("NFC INT PIN")]
				[#assign NFC_INT_PORT = IpName]
				[#assign NFC_INT_PIN = IpInstance]		
                [#assign NFC_INT_EXTI_LINE = ExtiLine]
				[#assign NFC_INT_EXTI_IRQn = IrqNumber]					
			[/#if]			
		[/#list]
	[/#if]
  [/#list]
[/#if]

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NFC0XA1_CONF_H__
#define __NFC0XA1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif
 
[#assign useNFC12POLLINGTAGDETECT = false]
[#assign useNFC12POLLINGTAGDETECTNDEF = false]

[#assign useSPI = false]
[#assign useCustomNFC12TAG = false]
[#assign useBOARDNFC12 = false]
[#assign useNDEF                = false]

[#assign USE_RFAL_FEATURE_LISTEN_MODE            = false]
[#assign USE_RFAL_FEATURE_WAKEUP_MODE            = false]
[#assign USE_RFAL_FEATURE_LOWPOWER_MODE          = false]
[#assign USE_RFAL_FEATURE_NFCA                   = false]
[#assign USE_RFAL_FEATURE_NFCB                   = false]
[#assign USE_RFAL_FEATURE_NFCF                   = false]
[#assign USE_RFAL_FEATURE_NFCV                   = false]
[#assign USE_RFAL_FEATURE_T1T                    = false]
[#assign USE_RFAL_FEATURE_T2T                    = false]
[#assign USE_RFAL_FEATURE_T4T                    = false]
[#assign USE_RFAL_FEATURE_ST25TB                 = false]
[#assign USE_RFAL_FEATURE_ST25xV                 = false]
[#assign USE_RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG  = false]
[#assign USE_RFAL_FEATURE_DPO                    = false]
[#assign USE_RFAL_FEATURE_ISO_DEP                = false]
[#assign USE_RFAL_FEATURE_ISO_DEP_POLL           = false]
[#assign USE_RFAL_FEATURE_ISO_DEP_LISTEN         = false]
[#assign USE_RFAL_FEATURE_NFC_DEP                = false]

[#assign USE_RFAL_DEFAULT_CONFIG                 = false]
[#assign USE_RFAL_CUSTOM_CONFIG                  = false]
[#assign RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN_TEST                  = "0x10"]

[#if RTEdatas??]
[#list RTEdatas as define]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT")]
[#assign useNFC12POLLINGTAGDETECT = true]
[/#if]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT_NDEF")]
[#assign useNFC12POLLINGTAGDETECTNDEF = true]
[#assign useNFC12POLLINGTAGDETECT = false]
[/#if]


[#if define?contains("BSP_NFC12TAG")]
[#assign useCustomNFC12TAG = true]
[/#if]

[#if define?contains("USE_SPI")]
[#assign useSPI = true]
[/#if]

[#if define?contains("USE_NFC12A1")]
[#assign useBOARDNFC12 = true]
[/#if]

[#if define?contains("LIB_NDEF_DEFAULT")]
[#assign useNDEF = true]
[/#if]

[#if define?contains("LIB_RFAL_DEFAULT")]
[#assign USE_RFAL_DEFAULT_CONFIG = true]
[/#if]

[#if define?contains("LIB_RFAL_CUSTOM")]
[#assign USE_RFAL_CUSTOM_CONFIG = true]
[/#if]

[#if define?contains("RFAL_FEATURE_LISTEN_MODE")]
[#assign USE_RFAL_FEATURE_LISTEN_MODE = true]
[/#if]

[#if define?contains("RFAL_FEATURE_WAKEUP_MODE")]
[#assign USE_RFAL_FEATURE_WAKEUP_MODE = true]
[/#if]

[#if define?contains("RFAL_FEATURE_LOWPOWER_MODE")]
[#assign USE_RFAL_FEATURE_LOWPOWER_MODE = true]
[/#if]

[#if define?contains("RFAL_FEATURE_NFCA")]
[#assign USE_RFAL_FEATURE_NFCA = true]
[/#if]

[#if define?contains("RFAL_FEATURE_NFCB")]
[#assign USE_RFAL_FEATURE_NFCB = true]
[/#if]

[#if define?contains("RFAL_FEATURE_NFCF")]
[#assign USE_RFAL_FEATURE_NFCF = true]
[/#if]

[#if define?contains("RFAL_FEATURE_NFCV")]
[#assign USE_RFAL_FEATURE_NFCV = true]
[/#if]

[#if define?contains("RFAL_FEATURE_T1T")]
[#assign USE_RFAL_FEATURE_T1T = true]
[/#if]

[#if define?contains("RFAL_FEATURE_T2T")]
[#assign USE_RFAL_FEATURE_T2T = true]
[/#if]

[#if define?contains("RFAL_FEATURE_T4T")]
[#assign USE_RFAL_FEATURE_T4T = true]
[/#if]

[#if define?contains("RFAL_FEATURE_ST25TB")]
[#assign USE_RFAL_FEATURE_ST25TB = true]
[/#if]

[#if define?contains("RFAL_FEATURE_ST25xV")]
[#assign USE_RFAL_FEATURE_ST25xV = true]
[/#if]

[#if define?contains("RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG")]
[#assign USE_RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG = true]
[/#if]

[#if define?contains("RFAL_FEATURE_DPO")]
[#assign USE_RFAL_FEATURE_DPO = true]
[/#if]

[#if define?contains("RFAL_FEATURE_ISO_DEP")]
[#assign USE_RFAL_FEATURE_ISO_DEP = true]
[/#if]

[#if define?contains("RFAL_FEATURE_ISO_DEP_POLL")]
[#assign USE_RFAL_FEATURE_ISO_DEP_POLL = true]
[/#if]

[#if define?contains("RFAL_FEATURE_ISO_DEP_LISTEN")]
[#assign USE_RFAL_FEATURE_ISO_DEP_LISTEN = true]
[/#if]

[#if define?contains("RFAL_FEATURE_NFC_DEP")]
[#assign USE_RFAL_FEATURE_NFC_DEP = true]

[/#if]

[/#list]
[/#if] 
[#compress]

[#list SWIPdatas as SWIP]
    [#if SWIP.defines??]
        [#list SWIP.defines as definition]
            [#assign value = definition.value]
            [#assign name = definition.name]
         
            [#if name == "RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN"]
                [#assign TEST_RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN = value]
            [/#if]
			
			[#if name == "RFAL_FEATURE_NFC_RF_BUF_LEN"]
                [#assign TEST_RFAL_FEATURE_NFC_RF_BUF_LEN = value]
            [/#if]
			
			[#if name == "RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN"]
                [#assign TEST_RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN= value]
            [/#if]
			
			[#if name == "RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN"]
                [#assign TEST_RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN= value]
            [/#if]
			
			[#if name == "RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN"]
                [#assign TEST_RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN= value]
            [/#if]
         
        [/#list]
    [/#if]
[/#list]

[/#compress]


#include "${FamilyName?lower_case}xx_hal.h"
#include "${BoardName}_bus.h"
#include "${BoardName}_errno.h"
#include "${FamilyName?lower_case}xx_hal_exti.h"
#include "RTE_Components.h"
[#if USE_RFAL_DEFAULT_CONFIG]
#include "rfal_defConfig.h"                   
[/#if] 

#define TEST               ${NFC_INT_EXTI_LINE}
#define TEST_LINE          ${NFC_INT_EXTI_IRQn}


[#if useNDEF]
#define TEST_USE_NDEF_TRUE                    
[/#if] 

[#if USE_RFAL_DEFAULT_CONFIG]
#define TEST_RFAL_DEFAULT                    
[/#if] 

[#if USE_RFAL_CUSTOM_CONFIG]
#define TEST_RFAL_CUSTOM                   
[/#if] 

[#if USE_RFAL_CUSTOM_CONFIG]

[#if USE_RFAL_FEATURE_LISTEN_MODE]
[#if ("${McuName}"?contains("STM32L053R8"))]
#define RFAL_FEATURE_LISTEN_MODE                    false       /*!< Enable/Disable RFAL support for Listen Mode                               */
[#else ]
#define RFAL_FEATURE_LISTEN_MODE                    true       /*!< Enable/Disable RFAL support for Listen Mode                               */
[/#if]
[#else]
#define RFAL_FEATURE_LISTEN_MODE                    false  /*!< Enable/Disable RFAL support for Listen Mode                               */
[/#if] 
[#if USE_RFAL_FEATURE_WAKEUP_MODE]
#define RFAL_FEATURE_WAKEUP_MODE                    true /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
[#else]
#define RFAL_FEATURE_WAKEUP_MODE                    false /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
[/#if] 
[#if USE_RFAL_FEATURE_LOWPOWER_MODE]
#define RFAL_FEATURE_LOWPOWER_MODE                  true  /*!< Enable/Disable RFAL support for the Low Power mode                        */
[#else]
#define RFAL_FEATURE_LOWPOWER_MODE                  false /*!< Enable/Disable RFAL support for the Low Power mode                        */
[/#if] 
[#if USE_RFAL_FEATURE_NFCA]
#define RFAL_FEATURE_NFCA                           true /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
[#else]
#define RFAL_FEATURE_NFCA                           false /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
[/#if] 
[#if USE_RFAL_FEATURE_NFCB]
#define RFAL_FEATURE_NFCB                           true  /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
[#else]
#define RFAL_FEATURE_NFCB                           false /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
[/#if] 
[#if USE_RFAL_FEATURE_NFCF]
#define RFAL_FEATURE_NFCF                           true /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
[#else]
#define RFAL_FEATURE_NFCF                           false /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
[/#if] 
[#if USE_RFAL_FEATURE_NFCV]
#define RFAL_FEATURE_NFCV                           true /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
[#else]
#define RFAL_FEATURE_NFCV                           false /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
[/#if] 
[#if USE_RFAL_FEATURE_T1T]
#define RFAL_FEATURE_T1T                            true /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
[#else]
#define RFAL_FEATURE_T1T                            false /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
[/#if] 
[#if USE_RFAL_FEATURE_T2T]
#define RFAL_FEATURE_T2T                            true /*!< Enable/Disable RFAL support for T2T                                       */
[#else]
#define RFAL_FEATURE_T2T                            false /*!< Enable/Disable RFAL support for T2T                                       */
[/#if] 
[#if USE_RFAL_FEATURE_T4T]
#define RFAL_FEATURE_T4T                            true /*!< Enable/Disable RFAL support for T4T                                       */
[#else]
#define RFAL_FEATURE_T4T                            false /*!< Enable/Disable RFAL support for T4T                                       */
[/#if] 
[#if USE_RFAL_FEATURE_ST25TB]
#define RFAL_FEATURE_ST25TB                         true /*!< Enable/Disable RFAL support for ST25TB                                    */
[#else]
#define RFAL_FEATURE_ST25TB                         false /*!< Enable/Disable RFAL support for ST25TB                                    */
[/#if] 
[#if USE_RFAL_FEATURE_ST25xV]
#define RFAL_FEATURE_ST25xV                         true  /*!< Enable/Disable RFAL support for  ST25TV/ST25DV                            */
[#else]
#define RFAL_FEATURE_ST25xV                         false  /*!< Enable/Disable RFAL support for  ST25TV/ST25DV                            */
[/#if] 
[#if USE_RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG] 
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG          true  /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
[#else]
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG          false  /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
[/#if] 
[#if USE_RFAL_FEATURE_DPO]
#define RFAL_FEATURE_DPO                            true  /*!< Enable/Disable RFAL Dynamic Power Output support                          */
[#else]
#define RFAL_FEATURE_DPO                            false  /*!< Enable/Disable RFAL Dynamic Power Output support                          */
[/#if] 
[#if USE_RFAL_FEATURE_ISO_DEP]
#define RFAL_FEATURE_ISO_DEP                        true /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
[#else]
#define RFAL_FEATURE_ISO_DEP                        false /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
[/#if] 
[#if USE_RFAL_FEATURE_ISO_DEP_POLL]
#define RFAL_FEATURE_ISO_DEP_POLL                   true /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
[#else]
#define RFAL_FEATURE_ISO_DEP_POLL                   false /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
[/#if] 
[#if USE_RFAL_FEATURE_ISO_DEP_LISTEN]
#define RFAL_FEATURE_ISO_DEP_LISTEN                 true /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
[#else]
#define RFAL_FEATURE_ISO_DEP_LISTEN                 false /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
[/#if] 
[#if USE_RFAL_FEATURE_NFC_DEP]
[#if ("${McuName}"?contains("STM32L053R8"))]
#define RFAL_FEATURE_NFC_DEP                        false       /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */
[#else ]
#define RFAL_FEATURE_NFC_DEP                        true       /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */
[/#if]
[#else]
#define RFAL_FEATURE_NFC_DEP                        false /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */
[/#if] 



[#if useNDEF] 
[#if !(USE_RFAL_FEATURE_T1T & USE_RFAL_FEATURE_T2T & USE_RFAL_FEATURE_T4T & USE_RFAL_FEATURE_NFCF & USE_RFAL_FEATURE_NFCV & USE_RFAL_FEATURE_ST25xV)]
#warning "To use NDEF, Select features: RFAL_FEATURE_T1T, RFAL_FEATURE_T2T, RFAL_FEATURE_T4T, RFAL_FEATURE_NFCF, RFAL_FEATURE_NFCV and RFAL_FEATURE_ST25xV"
[/#if] 
[/#if] 


[#if USE_RFAL_FEATURE_T1T | USE_RFAL_FEATURE_T2T] 
[#if !USE_RFAL_FEATURE_NFCA]
#warning "Please select feature : RFAL_FEATURE_NFCA"
[/#if] 
[/#if] 


[#if USE_RFAL_FEATURE_T4T] 
[#if !USE_RFAL_FEATURE_ISO_DEP]
#warning "Please select feature : RFAL_FEATURE_ISO_DEP"
[/#if] 
[/#if] 

[#if USE_RFAL_FEATURE_ST25TB] 
[#if !USE_RFAL_FEATURE_NFCB]
#warning "Please select feature : RFAL_FEATURE_NFCB"
[/#if] 
[/#if] 


[#if USE_RFAL_FEATURE_ST25xV] 
[#if !USE_RFAL_FEATURE_NFCV]
#warning "Please select feature : RFAL_FEATURE_NFCV"
[/#if] 
[/#if] 


[#if USE_RFAL_FEATURE_ISO_DEP] 
[#if !(USE_RFAL_FEATURE_NFCA & USE_RFAL_FEATURE_NFCB & (USE_RFAL_FEATURE_ISO_DEP_POLL | USE_RFAL_FEATURE_ISO_DEP_LISTEN ))]
#warning "Please select feature : RFAL_FEATURE_NFCA and RFAL_FEATURE_NFCB and (RFAL_FEATURE_ISO_DEP_POLL or RFAL_FEATURE_ISO_DEP_LISTEN"
[/#if] 
[/#if] 


[#if USE_RFAL_FEATURE_NFC_DEP] 
[#if !(USE_RFAL_FEATURE_NFCA & USE_RFAL_FEATURE_NFCF)]
#warning "Please select feature : RFAL_FEATURE_NFCA & RFAL_FEATURE_NFCF"
[/#if] 
[/#if] 


/*
******************************************************************************
* RFAL FEATURES CONFIGURATION
******************************************************************************
*/

#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN ${TEST_RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN}U  /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
#define RFAL_FEATURE_NFC_RF_BUF_LEN         ${TEST_RFAL_FEATURE_NFC_RF_BUF_LEN}U          /*!< RF buffer length used by RFAL NFC layer                                   */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN  ${TEST_RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN}U   /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN   ${TEST_RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN}U    /*!< ISO-DEP APDU max length. Please use multiples of I-Block max length       */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN    ${TEST_RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN}U     /*!< NFC-DEP PDU max length.                                                   */

/*
******************************************************************************
* RFAL CUSTOM SETTINGS
******************************************************************************
  Custom analog configs are used to cope with Automatic Antenna Tuning (AAT)
  that are optimized differently for each board.
*/
//#define RFAL_ANALOG_CONFIG_CUSTOM                         /*!< Use Custom Analog Configs when defined                                    */

[/#if] 


[#if TESTED_VAR]
#define TESTED_VAR_OK_BMP         
[/#if] 


#define LED_FIELD_Pin 1         /*!< Enable usage of led field pin on the platform      */
#define LED_FIELD_GPIO_Port 1   /*!< Enable usage of led field port on the platform     */
#define USE_LOGGER 1


[#if useNFC12POLLINGTAGDETECT | useNFC12POLLINGTAGDETECTNDEF | useCustomNFC12TAG | useBOARDNFC12]
#define NFC12A1_LED_A_PIN_CLK_ENABLE()     __HAL_RCC_${LED_A_PORT}_CLK_ENABLE()
#define NFC12A1_LED_B_PIN_CLK_ENABLE()     __HAL_RCC_${LED_B_PORT}_CLK_ENABLE()
#define NFC12A1_LED_F_PIN_CLK_ENABLE()     __HAL_RCC_${LED_F_PORT}_CLK_ENABLE()
#define NFC12A1_LED_V_PIN_CLK_ENABLE()     __HAL_RCC_${LED_V_PORT}_CLK_ENABLE()
#define NFC12A1_LED_FIELD_PIN_CLK_ENABLE()     __HAL_RCC_${LED_FIELD_PORT}_CLK_ENABLE()
#define NFC12A1_LED_A_PIN                  ${LED_A_PIN}
#define NFC12A1_LED_A_PIN_PORT             ${LED_A_PORT}
#define NFC12A1_LED_B_PIN                  ${LED_B_PIN}
#define NFC12A1_LED_B_PIN_PORT             ${LED_B_PORT}
#define NFC12A1_LED_F_PIN                  ${LED_F_PIN}
#define NFC12A1_LED_F_PIN_PORT             ${LED_F_PORT}
#define NFC12A1_LED_V_PIN                  ${LED_V_PIN}
#define NFC12A1_LED_V_PIN_PORT             ${LED_V_PORT}
#define NFC12A1_LED_FIELD_PIN              ${LED_FIELD_PIN}
#define NFC12A1_LED_FIELD_PIN_PORT         ${LED_FIELD_PORT}


#define NFC12A1_ALLLED_GPIO_CLK_ENABLE() {NFC12A1_LED_A_PIN_CLK_ENABLE();NFC12A1_LED_B_PIN_CLK_ENABLE();NFC12A1_LED_F_PIN_CLK_ENABLE();NFC12A1_LED_V_PIN_CLK_ENABLE();NFC12A1_LED_FIELD_PIN_CLK_ENABLE();}
#define NFC12A1_NFCTAG_INSTANCE         (0)
#define NFC12A1_NFCTAG_GPO_PRIORITY     (0)

#ifdef LED_FIELD_Pin
/* Case of ST25R500 */
#define PLATFORM_LED_FIELD_PIN       ${LED_FIELD_PIN}         /*!< GPIO pin used as field LED                        */
#endif /* LED_FIELD_Pin */

#ifdef LED_FIELD_GPIO_Port
/* Case of ST25R500 */
#define PLATFORM_LED_FIELD_PORT       ${LED_FIELD_PORT}    /*!< GPIO port used as field LED                       */
#endif  /* LED_FIELD_GPIO_Port */

/* Case of ST25R500 */
#define PLATFORM_LED_A_PIN           ${LED_A_PIN}         /*!< GPIO pin used for LED A    */
#define PLATFORM_LED_A_PORT          ${LED_A_PORT}    /*!< GPIO port used for LED A   */
#define PLATFORM_LED_B_PIN           ${LED_B_PIN}         /*!< GPIO pin used for LED B    */
#define PLATFORM_LED_B_PORT          ${LED_B_PORT}    /*!< GPIO port used for LED B   */
#define PLATFORM_LED_F_PIN           ${LED_F_PIN}         /*!< GPIO pin used for LED F    */
#define PLATFORM_LED_F_PORT          ${LED_F_PORT}    /*!< GPIO port used for LED F   */
#define PLATFORM_LED_V_PIN           ${LED_V_PIN}         /*!< GPIO pin used for LED V    */
#define PLATFORM_LED_V_PORT          ${LED_V_PORT}    /*!< GPIO port used for LED V   */

[/#if]


#define BUS_${NFC0XCOMMInstance}_NSS_GPIO_PIN           ${NFC_CS_PIN}
#define BUS_${NFC0XCOMMInstance}_NSS_GPIO_PORT          ${NFC_CS_PORT}
#define BUS_${NFC0XCOMMInstance}_IRQ_GPIO_PIN           ${NFC_INT_PIN}
#define BUS_${NFC0XCOMMInstance}_IRQ_GPIO_PORT          ${NFC_INT_PORT}
#define BUS_${NFC0XCOMMInstance}_RST_GPIO_PIN           ${NFC_RST_PIN}
#define BUS_${NFC0XCOMMInstance}_RST_GPIO_PORT          ${NFC_RST_PORT}


/* Exported constants --------------------------------------------------------*/
/** @defgroup PTD_Platform_Exported_Constants
  *  @{
  */
#define ST25R_SS_PIN             BUS_${NFC0XCOMMInstance}_NSS_GPIO_PIN    /*!< GPIO pin used for ST25R SPI SS                */
#define ST25R_SS_PORT            BUS_${NFC0XCOMMInstance}_NSS_GPIO_PORT   /*!< GPIO port used for ST25R SPI SS port          */

#define ST25R_INT_PIN            BUS_${NFC0XCOMMInstance}_IRQ_GPIO_PIN    /*!< GPIO pin used for ST25R IRQ                   */
#define ST25R_INT_PORT           BUS_${NFC0XCOMMInstance}_IRQ_GPIO_PORT   /*!< GPIO port used for ST25R IRQ port             */



#define IRQ_ST25R_EXTI_IRQn      ${NFC_INT_EXTI_IRQn}


#define PLATFORM_USER_BUTTON_PIN     USER_BUTTON_PIN          /*!< GPIO pin user button       */
#define PLATFORM_USER_BUTTON_PORT    USER_BUTTON_GPIO_PORT    /*!< GPIO port user button      */



#define USR_INT_LINE             H_EXTI_${NFC_INT_EXTI_LINE}
#define USR_INT_LINE_NUM         EXTI_LINE_${NFC_INT_EXTI_LINE}
#define BSP_NFC0XCOMM_Init             BSP_${NFC0XCOMMInstance}_Init
//#define BSP_NFC0XCOMM_IRQ_Callback     BSP_${NFC0XCOMMInstance}_IRQ_Callback

#define BSP_NFC0XCOMM_SequencialSend  BSP_${NFC0XCOMMInstance}_SequencialSend
#define BSP_NFC0XCOMM_SequencialRecv  BSP_${NFC0XCOMMInstance}_SequencialRecv

void BSP_${NFC0XCOMMInstance}_IRQ_Callback(void);

/**
  * @}
  */

/**
  * @}
  */

/*
******************************************************************************
* GLOBAL VARIABLES
******************************************************************************
*/

/* Exported variables --------------------------------------------------------*/
[#if useSPI]
extern SPI_HandleTypeDef h${NFC0XCOMMInstance?lower_case};          
[/#if] 


extern uint8_t globalCommProtectCnt;
extern  EXTI_HandleTypeDef H_EXTI_${NFC_INT_EXTI_LINE};
/* Exported functions ------------------------------------------------------- */
[#if useSPI]
int32_t BSP_NFC0XCOMM_SendRecv(const uint8_t *const pTxData, uint8_t *const pRxData, uint16_t Length);
[/#if]

int32_t BSP_NFC0XCOMM_Init(void);

#define  COMM_HANDLE                                     h${NFC0XCOMMInstance?lower_case}  

[#if useNFC12POLLINGTAGDETECT]
void MX_NFC12_PollingTagDetect_Process(void);
void MX_NFC12_PollingDemo_Init(void);
[/#if]

[#if useNFC12POLLINGTAGDETECTNDEF]
[/#if]


void _Error_Handler(char *file, int line);

#ifdef __cplusplus
}
#endif

#endif /* __NFC0XA1_CONF_H__*/



