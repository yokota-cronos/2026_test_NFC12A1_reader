[#ftl]
[#assign moduleName = "x-cube-nfcx"]
[#if ModuleName??]
    [#assign moduleName = ModuleName]
[/#if]
[#assign useSPI = false]
[#assign useI2C = false]
[#assign useNFC12POLLINGTAGDETECT = false]
[#assign useNFC12POLLINGTAGDETECTNDEF = false]


[#if RTEdatas??]
[#list RTEdatas as define]

[#if define?contains("USE_I2C")]
[#assign useI2C = true]
[/#if]

[#if define?contains("USE_SPI")]
[#assign useSPI = true]
[/#if]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT")]
[#assign useNFC12POLLINGTAGDETECT = true]
[/#if]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT_NDEF")]
[#assign useNFC12POLLINGTAGDETECTNDEF = true]
[#assign useNFC12POLLINGTAGDETECT = false]
[/#if]


[/#list]
[/#if] 
/**
  ******************************************************************************
  * @file           : rfal_platform.h
  * @brief          : Platform header file. Defining platform independent functionality. 
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
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "timer.h"
#include "main.h"
[#if useNFC12POLLINGTAGDETECT | useNFC12POLLINGTAGDETECTNDEF]
#include "logger.h"
[/#if] 
#include "RTE_Components.h"
#include "nfc_conf.h"
#include "demo.h"

/** @addtogroup X-CUBE-NFC12_Applications
  *  @{
  */

/** @addtogroup PollingTagDetect
  *  @{
  */

/** @defgroup PTD_Platform
  *  @brief Demo functions containing the example code
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PTD_Platform_Exported_Macro
  *  @{
  */
#define platformProtectST25RComm()                do{ globalCommProtectCnt++;                  \
                                                          __DSB();NVIC_DisableIRQ(IRQ_ST25R_EXTI_IRQn); \
                                                          __DSB();                             \
                                                          __ISB();                             \
                                                        }while(0)                                   /*!< Protect unique access to ST25R communication channel - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment      */
#define platformUnprotectST25RComm()              do{ globalCommProtectCnt--;             \
                                                          if (globalCommProtectCnt == 0U) \
                                                          {                               \
                                                            NVIC_EnableIRQ(IRQ_ST25R_EXTI_IRQn);   \
                                                          }                               \
                                                        }while(0)                                   /*!< Unprotect unique access to ST25R communication channel - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment */

#define platformProtectST25RIrqStatus()           platformProtectST25RComm()                /*!< Protect unique access to IRQ status var - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment */
#define platformUnprotectST25RIrqStatus()         platformUnprotectST25RComm()              /*!< Unprotect the IRQ status var - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment         */

#define platformProtectWorker()                                                                     /* Protect RFAL Worker/Task/Process from concurrent execution on multi thread platforms   */
#define platformUnprotectWorker()                                                                   /* Unprotect RFAL Worker/Task/Process from concurrent execution on multi thread platforms */

#define platformIrqST25RSetCallback( cb )
#define platformIrqST25RPinInitialize()

#define platformLedsInitialize()                                                                    /*!< Initializes the pins used as LEDs to outputs*/

#define platformLedOff( port, pin )                   platformGpioClear(port, pin)                  /*!< Turns the given LED Off                     */
#define platformLedOn( port, pin )                    platformGpioSet(port, pin)                    /*!< Turns the given LED On                      */
#define platformLedToogle( port, pin )                platformGpioToogle(port, pin)                 /*!< Toggle the given LED                        */

#define platformGpioSet( port, pin )                  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET)    /*!< Turns the given GPIO High                   */
#define platformGpioClear( port, pin )                HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET)  /*!< Turns the given GPIO Low                    */
#define platformGpioToogle( port, pin )               HAL_GPIO_TogglePin(port, pin)                 /*!< Toggle the given GPIO                      */
#define platformGpioIsHigh( port, pin )               (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) /*!< Checks if the given LED is High             */
#define platformGpioIsLow( port, pin )                (!platformGpioIsHigh(port, pin))              /*!< Checks if the given LED is Low              */

#define platformTimerCreate( t )                      timerCalculateTimer(t)                        /*!< Create a timer with the given time (ms)     */
#define platformTimerIsExpired( timer )               timerIsExpired(timer)                         /*!< Checks if the given timer is expired        */
#define platformTimerDestroy( timer )                                                               /*!< Stop and release the given timer            */
#define platformDelay( t )                            HAL_Delay( t )                                /*!< Performs a delay for the given time (ms)    */
#define platformGetSysTick()                          BSP_GetTick()                                 /*!< Get System Tick ( 1 tick = 1 ms)            */
#define platformTimerGetRemaining(t)                  (timerGetRemaining(t))


#define platformErrorHandle()                         _Error_Handler(__FILE__,__LINE__)             /*!< Global error handler or trap                */
[#if useSPI]
#define platformSpiSelect()                           platformGpioClear(ST25R_SS_PORT, ST25R_SS_PIN)/*!< SPI SS\CS: Chip|Slave Select                */
#define platformSpiDeselect()                         platformGpioSet(ST25R_SS_PORT, ST25R_SS_PIN)  /*!< SPI SS\CS: Chip|Slave Deselect              */
#define platformSpiTxRx( txBuf, rxBuf, len )          BSP_NFC0XCOMM_SendRecv(txBuf, rxBuf, len)          /*!< SPI transceive                              */
[/#if]

[#if useI2C]
#define platformI2CTx( txBuf, len, last, txOnly )     BSP_NFC0XCOMM_SequencialSend((uint16_t)0xA0, (uint8_t *)(txBuf), (len), last, txOnly ) /*!< I2C Transmit                                */
#define platformI2CRx( txBuf, len )                   BSP_NFC0XCOMM_SequencialRecv((uint16_t)0xA0, rxBuf, len )           /*!< I2C Receive                                 */
#define platformI2CStart()                                                                          /*!< I2C Start condition                         */
#define platformI2CStop()                                                                           /*!< I2C Stop condition                          */
#define platformI2CRepeatStart()                                                                    /*!< I2C Repeat Start                            */
#define platformI2CSlaveAddrWR(add)                                                                 /*!< I2C Slave address for Write operation       */
#define platformI2CSlaveAddrRD(add)                                                                 /*!< I2C Slave address for Read operation        */
[/#if]

#define platformLog(...)                              logUsart(__VA_ARGS__)                         /*!< Log  method                                 */

/* Exported functions ------------------------------------------------------- */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H */





