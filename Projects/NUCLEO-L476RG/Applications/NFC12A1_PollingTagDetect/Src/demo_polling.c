/**
  ******************************************************************************
  * @file           : ndef_poling.c
  * @brief          : Ndef Polling file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/*! \file
 *
 *  \author
 *
 *  \brief Demo application
 *
 *  This demo shows how to poll for several types of NFC cards/devices and how
 *  to exchange data with these devices, using the RFAL library.
 *
 *  This demo does not fully implement the activities according to the standards,
 *  it performs the required to communicate with a card/device and retrieve
 *  its UID. Also blocking methods are used for data exchange which may lead to
 *  long periods of blocking CPU/MCU.
 *  For standard compliant example please refer to the Examples provided
 *  with the RFAL library.
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "demo.h"
#include "utils.h"
#include "rfal_nfc.h"
#include "rfal_t2t.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#define FAST_READ_BLOCKS 10
#if RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE
#include "demo_ce.h"
#endif /* RFAL_FEATURE_LISTEN_MODE */

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/

/* Definition of possible states the demo state machine could have */
#define DEMO_ST_NOTINIT               0     /*!< Demo State:  Not initialized | Stopped */
#define DEMO_ST_START_DISCOVERY       1     /*!< Demo State:  Start Discovery           */
#define DEMO_ST_DISCOVERY             2     /*!< Demo State:  Discovery                 */
#define FAST_READ_BLOCKS 10
#define DEMO_NFCV_BLOCK_LEN           4     /*!< NFCV Block len                         */
#define BLOCKS_TO_READ                32    /* read 32 block */
#define BYTES_PER_BLOCK               4     /* ST25DV each block 4 bytes */
#define DEMO_NFCV_USE_SELECT_MODE     false /*!< NFCV demonstrate select mode           */
#define DEMO_NFCV_WRITE_TAG           false /*!< NFCV demonstrate Write Single Block    */

/* Definition of various Listen Mode constants */
#if defined(DEMO_LISTEN_MODE_TARGET)
#define DEMO_LM_SEL_RES       0x40U         /*!<NFC-A SEL_RES configured for the NFC-DEP protocol    */
#define DEMO_LM_NFCID2_BYTE1  0x01U         /*!<NFC-F SENSF_RES configured for the NFC-DEP protocol  */
#define DEMO_LM_SC_BYTE1      0xFFU         /*!<NFC-F System Code byte 1                             */
#define DEMO_LM_SC_BYTE2      0xFFU         /*!<NFC-F System Code byte 2                             */
#define DEMO_LM_PAD0          0xFFU         /*!<NFC-F PAD0                                           */
#else
#define DEMO_LM_SEL_RES       0x20U         /*!<NFC-A SEL_RES configured for Type 4A Tag Platform    */
#define DEMO_LM_NFCID2_BYTE1  0x02U         /*!<NFC-F SENSF_RES configured for Type 3 Tag Platform   */
#define DEMO_LM_SC_BYTE1      0x12U         /*!<NFC-F System Code byte 1                             */
#define DEMO_LM_SC_BYTE2      0xFCU         /*!<NFC-F System Code byte 2                             */
#define DEMO_LM_PAD0          0x00U         /*!<NFC-F PAD0                                           */
#endif

/*
 ******************************************************************************
 * GLOBAL MACROS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/* P2P communication data */
static uint8_t NFCID3[] = {0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
static uint8_t GB[] = {0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03};

/* APDUs communication data */
#if RFAL_FEATURE_ISO_DEP_POLL
static uint8_t ndefSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01, 0x00 };
static uint8_t ccSelectFile[] = { 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x03};
static uint8_t readBinary[] = { 0x00, 0xB0, 0x00, 0x00, 0x0F };

/* For a Payment application a Select PPSE would be needed:
   ppseSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 } */
#endif /* RFAL_FEATURE_ISO_DEP_POLL */

#if RFAL_FEATURE_NFC_DEP
/* P2P communication data */
static uint8_t ndefLLCPSYMM[] = {0x00, 0x00};
static uint8_t ndefInit[] = {0x05, 0x20, 0x06, 0x0F, 0x75, 0x72, 0x6E, 0x3A, 0x6E, 0x66, 0x63, 0x3A, 0x73, 0x6E, 0x3A, 0x73, 0x6E, 0x65, 0x70, 0x02, 0x02, 0x07, 0x80, 0x05, 0x01, 0x02};
static uint8_t ndefUriSTcom[] = {0x13, 0x20, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x19, 0xc1, 0x01, 0x00, 0x00, 0x00, 0x12, 0x55, 0x00, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x73, 0x74, 0x2e, 0x63, 0x6f, 0x6d};
#endif /* RFAL_FEATURE_NFC_DEP */

#if RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE
#if RFAL_SUPPORT_MODE_LISTEN_NFCA
/* NFC-A CE config */
/* 4-byte UIDs with first byte 0x08 would need random number for the subsequent 3 bytes.
 * 4-byte UIDs with first byte 0x*F are Fixed number, not unique, use for this demo
 * 7-byte UIDs need a manufacturer ID and need to assure uniqueness of the rest.*/
static uint8_t ceNFCA_NFCID[]     = {0x5F, 'S', 'T', 'M'};    /* =_STM, 5F 53 54 4D NFCID1 / UID (4 bytes) */
static uint8_t ceNFCA_SENS_RES[]  = {0x02, 0x00};             /* SENS_RES / ATQA for 4-byte UID            */
static uint8_t ceNFCA_SEL_RES     = DEMO_LM_SEL_RES;          /* SEL_RES / SAK                             */
#endif /* RFAL_SUPPORT_MODE_LISTEN_NFCA */

static uint8_t ceNFCF_nfcid2[]     = {DEMO_LM_NFCID2_BYTE1, 0xFE, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

#if RFAL_SUPPORT_MODE_LISTEN_NFCF
  /* NFC-F CE config */
static uint8_t ceNFCF_SC[]         = {DEMO_LM_SC_BYTE1, DEMO_LM_SC_BYTE2};
static uint8_t ceNFCF_SENSF_RES[]  = {0x01,                                                       /* SENSF_RES                                */
                                  0x02, 0xFE, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,                 /* NFCID2                                   */
                                  DEMO_LM_PAD0, DEMO_LM_PAD0, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, /* PAD0, PAD1, MRTIcheck, MRTIupdate, PAD2  */
                                  0x00, 0x00 };                                                   /* RD                                       */
#endif /* RFAL_SUPPORT_MODE_LISTEN_NFCF */
#endif /* RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

static rfalNfcDiscoverParam discParam;
static uint8_t              state = DEMO_ST_NOTINIT;
static bool                 multiSel;

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

static void demoP2P(rfalNfcDevice *nfcDev);
static void demoAPDU(void);
static void demoNfcv(rfalNfcvListenDevice *nfcvDev);
static void demoNfcf(rfalNfcfListenDevice *nfcfDev);
static void demoT2t(void);
static void demoCE(rfalNfcDevice *nfcDev);
static void demoNotif(rfalNfcState st);
ReturnCode  demoTransceiveBlocking(uint8_t *txBuf, uint16_t txBufSize, uint8_t **rxBuf, uint16_t **rcvLen, uint32_t fwt);

/*!
 *****************************************************************************
 * \brief Returns the RFAL NFC Discovery Parameter: Tech2Find
 *****************************************************************************
 */
uint16_t demoGetDiscoverTechs2Find(void)
{
    return discParam.techs2Find;
}

/*!
 *****************************************************************************
 * \brief Returns the RFAL NFC Discovery Parameter: Total Duration
 *****************************************************************************
 */
uint16_t demoGetDiscoverTotalDuration(void)
{
    return discParam.totalDuration;
}

/*!
 *****************************************************************************
 * \brief Demo Notification
 *
 *  This function receives the event notifications from RFAL
 *****************************************************************************
 */
/* =========================================================================
 * 终极修复版：自动处理 UID 顺序 + 错误调试信息 + 场强制开启
 * ========================================================================= */

/* 【请在这里修改你的 TAG UID】 */
static uint8_t TARGET_TAGS[3][8] = {
    {0xE0, 0x02, 0x27, 0x69, 0x7A, 0x67, 0x59, 0xD1},  // Tag 1
    {0xE0, 0x02, 0x27, 0x69, 0x7A, 0x6B, 0x8C, 0x75},  // Tag 2
    {0xE0, 0x02, 0x27, 0x00, 0x51, 0xCE, 0xDA, 0xF7}   // Tag 3 (没有填0)
};

/* 定义要轮询的 Tag 数量 */
#define NUM_TARGETS 3
#define FAST_READ_BLOCKS 10

static void demoNotif(rfalNfcState st)
{
    /* 只要进入发现流程，无论有没有卡，我们都接管 */
    if( st == RFAL_NFC_STATE_POLL_SELECT || st == RFAL_NFC_STATE_START_DISCOVERY )
    {
        static bool isRunning = false;
        if(isRunning) return;
        isRunning = true;

        platformLog("\r\n>>> [Hardcoded Mode] System Init...\r\n");

        /* 1. 强制初始化 NFC-V 协议栈并开启射频场 */
        /* 这是为了防止在没有卡时，底层协议栈没有把场打开 */
        rfalNfcvPollerInitialize();
        rfalFieldOnAndStartGT();

        platformLog(">>> Field is ON. Pre-charging (1000ms)...\r\n");
        platformDelay(500);

        /* ============================================================ */
        /* 死循环 */
        /* ============================================================ */
        while(1)
        {
            int successCountInThisLoop = 0;

            for(int i = 0; i < NUM_TARGETS; i++)
            {
                rfalNfcWorker();

                /* 如果 UID 是全 0，跳过 */
                if(TARGET_TAGS[i][0] == 0x00 && TARGET_TAGS[i][7] == 0x00) continue;

                /* --- 自动翻转 UID --- */
                /* 将人类可读的 E0 开头的 UID，转换为机器需要的 LSB 格式 */
                uint8_t machineUID[RFAL_NFCV_UID_LEN];
                for(int k=0; k<8; k++) {
                    machineUID[k] = TARGET_TAGS[i][7-k];
                }

                uint8_t  rxBuf[ 1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN ];
                uint16_t rcvLen;
                uint8_t  rawMemory[FAST_READ_BLOCKS * 4];
                ReturnCode err;
                bool readSuccess = true;

                /* --- 尝试读取 --- */
                for (int b = 0; b < FAST_READ_BLOCKS; b++)
                {
                    err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, machineUID, b, rxBuf, sizeof(rxBuf), &rcvLen);

                    if(err != RFAL_ERR_NONE) {
                        /* 快速重试 */
                        err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, machineUID, b, rxBuf, sizeof(rxBuf), &rcvLen);
                    }

                    if (err == RFAL_ERR_NONE) {
                        memcpy(&rawMemory[b * 4], &rxBuf[1], 4);
                    } else {
                        readSuccess = false;
                        /* [调试] 如果你想知道为什么失败，可以解开下面这行 */
                        // platformLog("[Debug] Tag_%d Block_%d Error: %d\r\n", i+1, b, err);
                        break;
                    }
                }

                /* --- 解析与打印 --- */
                if (readSuccess)
                {
                    successCountInThisLoop++;
                    int parseIdx = 0;
                    while (parseIdx < 32)
                    {
                        if (rawMemory[parseIdx] == 0xD1 && rawMemory[parseIdx+1] == 0x01 && rawMemory[parseIdx+3] == 0x54)
                        {
                            uint8_t payloadLen = rawMemory[parseIdx+2];
                            uint8_t statusByte = rawMemory[parseIdx+4];
                            int langLen = statusByte & 0x1F;
                            int textStartIdx = parseIdx + 5 + langLen;
                            int textLen = payloadLen - 1 - langLen;

                            if (textLen > 0 && (textStartIdx + textLen) <= sizeof(rawMemory))
                            {
                                char msg[64] = {0};
                                memcpy(msg, &rawMemory[textStartIdx], textLen);
                                msg[textLen] = '\0';

                                platformLog("[Tag_%d] %s  ", i+1, msg);
                                break;
                            }
                        }
                        parseIdx++;
                    }
                }
            } // end for loop

            /* 只有当本轮读到了数据，才换行。防止空白回车刷屏 */
            if(successCountInThisLoop > 0) {
                platformLog("\r\n");
            } else {
                /* 如果谁都没读到（比如卡都拿走了），这里什么都不打印，保持安静 */
                /* 或者打印一个点表示活着: platformLog("."); */
            }

            platformDelay(50);

        } // end while(1)
    }
}
/*!
 *****************************************************************************
 * \brief Demo Ini
 *
 *  This function initializes the required layers for the demo
 *
 * \return true  : Initialization ok
 * \return false : Initialization failed
 *****************************************************************************
 */
/*
 * Initialize the Demo.
 * OPTIMIZATION: Only enable NFC-V to maximize Field-On time for Smartags.
 */
bool demoIni(void)
{
  ReturnCode err;

  err = rfalNfcInitialize();
  if( err == RFAL_ERR_NONE )
  {
    rfalNfcDefaultDiscParams( &discParam );

    discParam.devLimit      = 1U; /* 只找一张卡，专心供电 */

    ST_MEMCPY( &discParam.nfcid3, NFCID3, sizeof(NFCID3) );
    ST_MEMCPY( &discParam.GB, GB, sizeof(GB) );
    discParam.GBLen         = sizeof(GB);
    discParam.p2pNfcaPrio   = true;

    discParam.notifyCb      = demoNotif;
    discParam.totalDuration = 1000U;

    /* 【核心修改 1】：只开启 NFC-V (ISO15693) */
    /* 屏蔽掉其他所有协议，确保 Field 不会因为切换协议而闪断 */
    discParam.techs2Find    = RFAL_NFC_POLL_TECH_V;

    // discParam.techs2Find |= RFAL_NFC_POLL_TECH_A;
    // discParam.techs2Find |= RFAL_NFC_POLL_TECH_B;
    // discParam.techs2Find |= RFAL_NFC_POLL_TECH_F;
    // discParam.techs2Find |= RFAL_NFC_POLL_TECH_ST25TB;

    err = rfalNfcDiscover( &discParam );
    if( err != RFAL_ERR_NONE )
    {
        return false;
    }

    state = DEMO_ST_START_DISCOVERY;
    return true;
    }
   return false;
}
/*!
 *****************************************************************************
 * \brief Demo Cycle
 *
 *  This function executes the demo state machine.
 *  It must be called periodically
 *****************************************************************************
 */
void demoCycle(void)
{
  static rfalNfcDevice *nfcDevice;

  rfalNfcWorker();                                    /* Run RFAL worker periodically */

#if defined(PLATFORM_USER_BUTTON_PORT) && defined(PLATFORM_USER_BUTTON_PIN)

/* Check if button is supported (even if the pins are defined). MCUs may have the pin but not connected | shorted to GND */
#ifdef platformIsUserButtonSupported
  if( platformIsUserButtonSupported() )
#endif  /*platformIsUserButtonSupported()*/

    /*******************************************************************************/
    /* Check if USER button is pressed */
    if( platformGpioIsLow(PLATFORM_USER_BUTTON_PORT, PLATFORM_USER_BUTTON_PIN))
    {
      discParam.wakeupEnabled = !discParam.wakeupEnabled;    /* enable/disable wakeup */
      state = DEMO_ST_START_DISCOVERY;                       /* restart loop          */
      platformLog("Toggling Wake Up mode %s\r\n", discParam.wakeupEnabled ? "ON": "OFF");

      /* Re-start discovery with new Wake-up configuration */
      rfalNfcDeactivate( RFAL_NFC_DEACTIVATE_IDLE );
      rfalNfcDiscover( &discParam );

      /* Debounce button */
      while( platformGpioIsLow(PLATFORM_USER_BUTTON_PORT, PLATFORM_USER_BUTTON_PIN) );
    }
#endif /* PLATFORM_USER_BUTTON_PIN */

    switch( state )
    {
      /*******************************************************************************/
      case DEMO_ST_START_DISCOVERY:

        platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
        platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
        platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
        platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
        platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);

        multiSel = false;
        state    = DEMO_ST_DISCOVERY;
        break;

      /*******************************************************************************/
      case DEMO_ST_DISCOVERY:

          if( rfalNfcIsDevActivated( rfalNfcGetState() ) )
          {
              rfalNfcGetActiveDevice( &nfcDevice );

              switch( nfcDevice->type )
              {
                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_NFCA:

                    platformLedOn(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
                    switch( nfcDevice->dev.nfca.type )
                    {
                        case RFAL_NFCA_T1T:
                            platformLog("ISO14443A/Topaz (NFC-A T1T) TAG found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                            break;

                        case RFAL_NFCA_T4T:
                            platformLog("NFCA Passive ISO-DEP device found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );

                            demoAPDU();
                            break;

                        case RFAL_NFCA_T4T_NFCDEP:
                        case RFAL_NFCA_NFCDEP:
                            platformLog("NFCA Passive P2P device found. NFCID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );

                            demoP2P( nfcDevice );
                            break;

                        default:
                            platformLog("ISO14443A/NFC-A card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );

                            demoT2t();
                            break;
                    }
                    break;

                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_NFCB:

                    platformLedOn(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
                    platformLog("ISO14443B/NFC-B card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );

                    if( rfalNfcbIsIsoDepSupported( &nfcDevice->dev.nfcb ) )
                    {
                        demoAPDU();
                    }
                    break;

                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_NFCF:

                    platformLedOn(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
                    if( rfalNfcfIsNfcDepSupported( &nfcDevice->dev.nfcf ) )
                    {
                        platformLog("NFCF Passive P2P device found. NFCID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                        demoP2P( nfcDevice );
                    }
                    else
                    {
                        platformLog("Felica/NFC-F card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ));

                        demoNfcf( &nfcDevice->dev.nfcf );
                    }

                    break;

                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_NFCV:
                    {
                        uint8_t devUID[RFAL_NFCV_UID_LEN];

                        platformLedOn(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);

                        ST_MEMCPY( devUID, nfcDevice->nfcid, nfcDevice->nfcidLen );   /* Copy the UID into local var */
                        REVERSE_BYTES( devUID, RFAL_NFCV_UID_LEN );                 /* Reverse the UID for display purposes */
                        platformLog("ISO15693/NFC-V card found. UID: %s\r\n", hex2Str(devUID, RFAL_NFCV_UID_LEN));

                        demoNfcv( &nfcDevice->dev.nfcv );
                    }
                    break;

                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_ST25TB:

                    platformLedOn(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
                    platformLog("ST25TB card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ));
                    break;

                /*******************************************************************************/
                case RFAL_NFC_LISTEN_TYPE_AP2P:
                case RFAL_NFC_POLL_TYPE_AP2P:

                    platformLog("NFC Active P2P device found. NFCID3: %s\r\n", hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));

                    demoP2P( nfcDevice );
                    break;

                /*******************************************************************************/
                case RFAL_NFC_POLL_TYPE_NFCA:
                case RFAL_NFC_POLL_TYPE_NFCF:

                    platformLedOn( ((nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? PLATFORM_LED_A_PORT : PLATFORM_LED_F_PORT),
                                   ((nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? PLATFORM_LED_A_PIN  : PLATFORM_LED_F_PIN)  );
                    platformLog("Activated in CE %s mode.\r\n", (nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? "NFC-A" : "NFC-F");

                    if( nfcDevice->rfInterface == RFAL_NFC_INTERFACE_NFCDEP )
                    {
                        demoP2P( nfcDevice );
                    }
                    else
                    {
                        demoCE( nfcDevice );
                    }
                    break;

                /*******************************************************************************/
                default:
                    break;
              }

              rfalNfcDeactivate( RFAL_NFC_DEACTIVATE_DISCOVERY );
              state = DEMO_ST_START_DISCOVERY;
          }
          break;

      /*******************************************************************************/
      case DEMO_ST_NOTINIT:
      default:
          break;
    }
}

/*!
 *****************************************************************************
 * \brief Demo Stop
 *
 *  This function stops the demo operation
 *
 *****************************************************************************
 */
void demoStop(void)
{
  rfalNfcDeactivate( RFAL_NFC_DEACTIVATE_IDLE );
  state = DEMO_ST_NOTINIT;
}

static void demoCE(rfalNfcDevice *nfcDev)
{
#if RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE

  ReturnCode err = RFAL_ERR_INTERNAL;
  uint8_t *rxData;
  uint16_t *rcvLen;
  uint8_t  txBuf[150];
  uint16_t txLen;

  do
  {
    rfalNfcWorker();

    switch( rfalNfcGetState() )
    {
      case RFAL_NFC_STATE_ACTIVATED:
          err = demoTransceiveBlocking( NULL, 0, &rxData, &rcvLen, 0);
          break;

      case RFAL_NFC_STATE_DATAEXCHANGE:
      case RFAL_NFC_STATE_DATAEXCHANGE_DONE:

          txLen = ( (nfcDev->type == RFAL_NFC_POLL_TYPE_NFCA) ? demoCeT4T( rxData, *rcvLen, txBuf, sizeof(txBuf) ) : rfalConvBytesToBits( demoCeT3T( rxData, rfalConvBitsToBytes(*rcvLen), txBuf, sizeof(txBuf) ) ) );
          err   = demoTransceiveBlocking( txBuf, txLen, &rxData, &rcvLen, RFAL_FWT_NONE );
          break;

      case RFAL_NFC_STATE_START_DISCOVERY:
          return;

      case RFAL_NFC_STATE_LISTEN_SLEEP:
      default:
          break;
    }
  }
  while ((err == RFAL_ERR_NONE) || (err == RFAL_ERR_SLEEP_REQ));

#else
  NO_WARNING(nfcDev);
#endif /* RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE */
}

/*!
 *****************************************************************************
 * \brief Demo NFC-F
 *
 * Example how to exchange read and write blocks on a NFC-F tag
 *
 *****************************************************************************
 */
static void demoNfcf(rfalNfcfListenDevice *nfcfDev)
{
#if RFAL_FEATURE_NFCF

  ReturnCode                 err;
  uint8_t                    buf[ (RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN + (3*RFAL_NFCF_BLOCK_LEN)) ];
  uint16_t                   rcvLen;
  rfalNfcfServ               srv = RFAL_NFCF_SERVICECODE_RDWR;
  rfalNfcfBlockListElem      bl[3];
  rfalNfcfServBlockListParam servBlock;
  //uint8_t                    wrData[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

  servBlock.numServ   = 1;                            /* Only one Service to be used           */
  servBlock.servList  = &srv;                         /* Service Code: NDEF is Read/Writeable  */
  servBlock.numBlock  = 1;                            /* Only one block to be used             */
  servBlock.blockList = bl;
  bl[0].conf     = RFAL_NFCF_BLOCKLISTELEM_LEN_BIT;   /* Two-byte Block List Element           */
  bl[0].blockNum = 0x0001;                            /* Block: NDEF Data                      */

  err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
  platformLog(" Check Block: %s Data:  %s \r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );

#if 0  /* Writing example */
  err = rfalNfcfPollerUpdate( nfcfDev->sensfRes.NFCID2, &servBlock, buf , sizeof(buf), wrData, buf, sizeof(buf) );
  platformLog(" Update Block: %s Data: %s \r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( wrData, RFAL_NFCF_BLOCK_LEN) );
  err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
  platformLog(" Check Block:  %s Data: %s \r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
#endif

#else
  NO_WARNING(nfcfDev);
#endif /* RFAL_FEATURE_NFCF */
}

/*!
 *****************************************************************************
 * \brief Demo NFC-V Exchange
 *
 * Example how to exchange read and write blocks on a NFC-V tag
 *
 *****************************************************************************
 */
//static void demoNfcv(rfalNfcvListenDevice *nfcvDev)
//{
//#if RFAL_FEATURE_NFCV
//
//  ReturnCode            err;
//  uint16_t              rcvLen;
//  uint8_t               blockNum = 1;
//  uint8_t               rxBuf[ 1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN ];                        /* Flags + Block Data + CRC */
//  uint8_t               *uid;
//  uint8_t               reqFlag;
//#if DEMO_NFCV_WRITE_TAG
//  uint8_t               wrData[DEMO_NFCV_BLOCK_LEN] = { 0x11, 0x22, 0x33, 0x99 };             /* Write block example */
//#endif /* DEMO_NFCV_WRITE_TAG */
//
//  uid     = nfcvDev->InvRes.UID;
//  reqFlag = RFAL_NFCV_REQ_FLAG_DEFAULT;
//
//#if DEMO_NFCV_USE_SELECT_MODE
//  /*
//  * Activate selected state
//  */
//  err = rfalNfcvPollerSelect( reqFlag, nfcvDev->InvRes.UID );
//  platformLog(" Select %s \r\n", (err != RFAL_ERR_NONE) ? "FAIL (revert to addressed mode)": "OK" );
//  if( err == RFAL_ERR_NONE )
//  {
//      reqFlag = (RFAL_NFCV_REQ_FLAG_DEFAULT | RFAL_NFCV_REQ_FLAG_SELECT);
//      uid     = NULL;
//  }
//#endif /* DEMO_NFCV_USE_SELECT_MODE */
//
//  /*
//  * Read block using Read Single Block command
//  * with addressed mode (uid != NULL) or selected mode (uid == NULL)
//  */
//  err = rfalNfcvPollerReadSingleBlock(reqFlag, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
//  platformLog(" Read Block: %s %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK Data:", (err != RFAL_ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
//
//#if DEMO_NFCV_WRITE_TAG /* Writing example */
//  err = rfalNfcvPollerWriteSingleBlock(reqFlag, uid, blockNum, wrData, sizeof(wrData));
//  platformLog(" Write Block: %s Data: %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", hex2Str( wrData, DEMO_NFCV_BLOCK_LEN) );
//  err = rfalNfcvPollerReadSingleBlock(reqFlag, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
//  platformLog(" Read Block: %s %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK Data:", (err != RFAL_ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
//#endif /* DEMO_NFCV_WRITE_TAG */
//
//#endif /* RFAL_FEATURE_NFCV */
//}


/* 修改函数签名为 bool 返回类型，用于报告状态 */
static void demoNfcv(rfalNfcvListenDevice *nfcvDev)
{
//#if RFAL_FEATURE_NFCV
//
//  ReturnCode            err;
//  uint16_t              rcvLen;
//  uint8_t               rxBuf[ 1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN ];
//
//  /* 定义读取参数 */
//  #define FAST_READ_BLOCKS 10
//  uint8_t               rawMemory[FAST_READ_BLOCKS * 4];
//
//  /* 定义寻卡（Inventory）所需的变量 */
//  rfalNfcvInventoryRes  invRes[RFAL_NFCV_MAX_DEVICES]; /* 存放找到的 Tag 列表 */
//  uint16_t              numFound;                      /* 找到的 Tag 数量 */
//
//  platformLog(">>> Entering Dynamic Multi-Tag Mode (Field Always ON)...\r\n");
//
//  /* ============================================================ */
//  /* 超级循环：集成了“寻卡”和“读取” */
//  /* ============================================================ */
//  while(1)
//  {
//      /* 0. 喂狗，维持底层状态机 */
//      rfalNfcWorker();
//
//      /* 1. 【关键】在循环内主动寻卡！ */
//      /* 这条命令会扫描场内当前所有的 Tag，并把结果存入 invRes */
//      /* 即使没有 Tag，它也不会关场，只是返回 0 个设备 */
//      err = rfalNfcvPollerInventory16Slots( RFAL_NFCV_REQ_FLAG_DEFAULT, 0, NULL, invRes, &numFound );
//
//      /* 如果寻卡出错（比如干扰），不要死机，稍等重试 */
//      if( err != RFAL_ERR_NONE )
//      {
//          // platformLog("Inventory Fail (Error: %d)\r\n", err);
//          platformDelay(10);
//          continue;
//      }
//
//      /* 2. 遍历刚才找到的所有 Tag */
//      if (numFound > 0)
//      {
//          for(int i = 0; i < numFound; i++)
//          {
//              uint8_t *currentUID = invRes[i].UID;
//              bool readSuccess = true;
//
//              // --- 3. 读取数据 (使用 currentUID 寻址) ---
//              for (int b = 0; b < FAST_READ_BLOCKS; b++)
//              {
//                  // 再次喂狗，防止在读长数据时超时
//                  rfalNfcWorker();
//
//                  err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, currentUID, b, rxBuf, sizeof(rxBuf), &rcvLen);
//
//                  // 失败原地快速重试一次
//                  if (err != RFAL_ERR_NONE) {
//                      platformDelay(1);
//                      err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, currentUID, b, rxBuf, sizeof(rxBuf), &rcvLen);
//                  }
//
//                  if (err == RFAL_ERR_NONE)
//                  {
//                      memcpy(&rawMemory[b * 4], &rxBuf[1], 4);
//                  }
//                  else
//                  {
//                      readSuccess = false;
//                      break; // 读取中断，跳过这个 Tag
//                  }
//              }
//
//              // --- 4. 解析并打印 ---
//              if (readSuccess)
//              {
//                  int parseIdx = 0;
//                  bool found = false;
//
//                  while (parseIdx < (FAST_READ_BLOCKS * 4) - 6)
//                  {
//                      if (rawMemory[parseIdx] == 0xD1 && rawMemory[parseIdx+1] == 0x01 && rawMemory[parseIdx+3] == 0x54)
//                      {
//                          uint8_t payloadLen = rawMemory[parseIdx+2];
//                          uint8_t statusByte = rawMemory[parseIdx+4];
//                          int langLen = statusByte & 0x1F;
//                          int textStartIdx = parseIdx + 5 + langLen;
//                          int textLen = payloadLen - 1 - langLen;
//
//                          if (textLen > 0 && (textStartIdx + textLen) <= sizeof(rawMemory))
//                          {
//                              char msg[64] = {0};
//                              memcpy(msg, &rawMemory[textStartIdx], textLen);
//                              msg[textLen] = '\0';
//
//                              platformLog("[ID:%02X%02X] %s\r\n", currentUID[7], currentUID[6], msg);
//                              found = true;
//                              break;
//                          }
//                      }
//                      parseIdx++;
//                  }
//              }
//
//              /* 读完一个 Tag 稍微停顿 */
//              platformDelay(5);
//          }
//      }
//      else
//      {
//          /* 如果当前没有卡，不要狂刷 Inventory，稍微等一下 */
//          /* 注意：这里没有关场，Field 依然是开着的 */
//          platformDelay(50);
//      }
//
//      /* 所有卡轮询完一圈，长延时 */
//      platformDelay(100);
//
//  } // end of while(1)
//
//#endif /* RFAL_FEATURE_NFCV */
}
/*!
 *****************************************************************************
 * \brief Demo T2T Exchange
 *
 * Example how to exchange read blocks on a T2T tag
 *
 *****************************************************************************
 */
static void demoT2t(void)
{
#if RFAL_FEATURE_T2T
  ReturnCode            err;
  uint16_t              rcvLen;
  uint8_t               blockNum = 0;
  uint8_t               rxBuf[ RFAL_T2T_READ_DATA_LEN ];

  err = rfalT2TPollerRead(blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
  platformLog(" Read Block: %s %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK Data:", (err != RFAL_ERR_NONE) ? "" : hex2Str( rxBuf, RFAL_T2T_READ_DATA_LEN));

#endif
}

/*!
 *****************************************************************************
 * \brief Demo P2P Exchange
 *
 * Sends a NDEF URI record 'http://www.ST.com' via NFC-DEP (P2P) protocol.
 *
 * This method sends a set of static predefined frames which tries to establish
 * a LLCP connection, followed by the NDEF record, and then keeps sending
 * LLCP SYMM packets to maintain the connection.
 *
 *
 *****************************************************************************
 */
void demoP2P(rfalNfcDevice *nfcDev)
{
#if RFAL_FEATURE_NFC_DEP

  uint16_t   *rxLen;
  uint8_t    *rxData;
  ReturnCode err;

  /* In Listen mode retrieve the first request from Initiator */
  if( (nfcDev->type == RFAL_NFC_POLL_TYPE_AP2P) ||  (nfcDev->type == RFAL_NFC_POLL_TYPE_NFCA ) ||  (nfcDev->type == RFAL_NFC_POLL_TYPE_NFCF ) )
  {
      demoTransceiveBlocking( NULL, 0, &rxData, &rxLen, 0);

      /* Initiator request is being ignored/discarded  */
  }

  platformLog(" Initialize device .. ");
  err = demoTransceiveBlocking(ndefInit, sizeof(ndefInit), &rxData, &rxLen, RFAL_FWT_NONE);
  if( err != RFAL_ERR_NONE )
  {
      platformLog("failed.\r\n");
      return;
  }
  platformLog("succeeded.\r\n");

  platformLog(" Push NDEF Uri: www.st.com .. ");
  err = demoTransceiveBlocking(ndefUriSTcom, sizeof(ndefUriSTcom), &rxData, &rxLen, RFAL_FWT_NONE);
  if( err != RFAL_ERR_NONE )
  {
      platformLog("failed.\r\n");
      return;
  }
  platformLog("succeeded.\r\n");

  platformLog(" Device present, maintaining connection ");
  while(err == RFAL_ERR_NONE)
  {
      err = demoTransceiveBlocking(ndefLLCPSYMM, sizeof(ndefLLCPSYMM), &rxData, &rxLen, RFAL_FWT_NONE);
      platformLog(".");
      platformDelay(50);
  }
  platformLog("\r\n Device removed.\r\n");

#endif /* RFAL_FEATURE_NFC_DEP */
}

/*!
 *****************************************************************************
 * \brief Demo APDUs Exchange
 *
 * Example how to exchange a set of predefined APDUs with PICC. The NDEF
 * application will be selected and then CC will be selected and read.
 *
 *****************************************************************************
 */
void demoAPDU(void)
{
#if RFAL_FEATURE_ISO_DEP_POLL
  ReturnCode err;
  uint16_t   *rxLen;
  uint8_t    *rxData;

  /* Exchange APDU: NDEF Tag Application Select command */
  err = demoTransceiveBlocking( ndefSelectApp, sizeof(ndefSelectApp), &rxData, &rxLen, RFAL_FWT_NONE );
  platformLog(" Select NDEF Application: %s Data: %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( rxData, *rxLen) );

  if( (err == RFAL_ERR_NONE) && (rxData[0] == 0x90U) && (rxData[1] == 0x00U) )
  {
      /* Exchange APDU: Select Capability Container File */
      err = demoTransceiveBlocking( ccSelectFile, sizeof(ccSelectFile), &rxData, &rxLen, RFAL_FWT_NONE );
      platformLog(" Select CC: %s Data: %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( rxData, *rxLen) );

      /* Exchange APDU: Read Capability Container File  */
      err = demoTransceiveBlocking( readBinary, sizeof(readBinary), &rxData, &rxLen, RFAL_FWT_NONE );
      platformLog(" Read CC: %s Data: %s\r\n", (err != RFAL_ERR_NONE) ? "FAIL": "OK", (err != RFAL_ERR_NONE) ? "" : hex2Str( rxData, *rxLen) );
  }
#endif /* RFAL_FEATURE_ISO_DEP_POLL */
}

/*!
 *****************************************************************************
 * \brief Demo Blocking Transceive
 *
 * Helper function to send data in a blocking manner via the rfalNfc module
 *
 * \warning A protocol transceive handles long timeouts (several seconds),
 * transmission errors and retransmissions which may lead to a long period of
 * time where the MCU/CPU is blocked in this method.
 * This is a demo implementation, for a non-blocking usage example please
 * refer to the Examples available with RFAL
 *
 * \param[in]  txBuf      : data to be transmitted
 * \param[in]  txBufSize  : size of the data to be transmitted
 * \param[out] rxData     : location where the received data has been placed
 * \param[out] rcvLen     : number of data bytes received
 * \param[in]  fwt        : FWT to be used (only for RF frame interface,
 *                                          otherwise use RFAL_FWT_NONE)
 *
 *
 *  \return RFAL_ERR_PARAM   : Invalid parameters
 *  \return RFAL_ERR_TIMEOUT : Timeout error
 *  \return RFAL_ERR_FRAMING : Framing error detected
 *  \return RFAL_ERR_PROTO   : Protocol error detected
 *  \return RFAL_ERR_NONE    : No error, activation successful
 *
 *****************************************************************************
 */
ReturnCode demoTransceiveBlocking(uint8_t *txBuf, uint16_t txBufSize, uint8_t **rxData, uint16_t **rcvLen, uint32_t fwt)
{
  ReturnCode err;

  err = rfalNfcDataExchangeStart(txBuf, txBufSize, rxData, rcvLen, fwt);
  if (err == RFAL_ERR_NONE)
  {
      do{
          rfalNfcWorker();
          err = rfalNfcDataExchangeGetStatus();
      }
      while (err == RFAL_ERR_BUSY);
  }
  return err;
}

