/**
  ******************************************************************************
  * @file           : demo_polling.c
  * @brief          : Smart Polling (Buffer & Print) with Multi-Block Read
  ******************************************************************************
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
#include "nfc_conf.h"
#include "st25r500_com.h"
#include <stdio.h>
#include <string.h>

#if RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE
#include "demo_ce.h"
#endif

/*
 ******************************************************************************
 * CONFIGURATION
 ******************************************************************************
 */

/* Generic read: detect whatever NFC-V (ISO15693) tag is in the field via
 * inventory, then read it by its own UID. No hard-coded UID list needed. */
#define MAX_DEVICES          5      /* max simultaneous tags to resolve */
#define READ_BLOCKS          16
#define BLOCK_SIZE           4
#define RAW_MEMORY_SIZE      (READ_BLOCKS * BLOCK_SIZE)

/*
 ******************************************************************************
 * GLOBAL DEFINES
 ******************************************************************************
 */
#define DEMO_ST_NOTINIT           0
#define DEMO_ST_START_DISCOVERY   1
#define DEMO_ST_DISCOVERY         2

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
static uint8_t NFCID3[] = {0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
static uint8_t GB[] = {0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03};

static rfalNfcDiscoverParam discParam;
static uint8_t              state = DEMO_ST_NOTINIT;

/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */
static void demoPollOnce(void);
static void demoNfcv(rfalNfcvListenDevice *nfcvDev);
static void demoCE(rfalNfcDevice *nfcDev);

/*
 ******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************
 */
uint16_t demoGetDiscoverTechs2Find(void) { return discParam.techs2Find; }
uint16_t demoGetDiscoverTotalDuration(void) { return discParam.totalDuration; }

/*
 ******************************************************************************
 * MAIN POLLING HANDLER - Buffer Strategy
 ******************************************************************************
 */

/* One polling pass: inventory any NFC-V tag(s) in the field, read & print.
 * The RF field is left ON by demoIni and stays on; called every demoCycle(). */
static void demoPollOnce(void)
{
    rfalNfcvListenDevice devList[MAX_DEVICES];
    uint8_t              devCnt = 0;
    ReturnCode           err;

    /* --- 1. Inventory: find whatever NFC-V tag(s) are present --- */
    err = rfalNfcvPollerCollisionResolution(
              RFAL_COMPLIANCE_MODE_NFC, MAX_DEVICES, devList, &devCnt);

    if (err != RFAL_ERR_NONE || devCnt == 0)
    {
        return; /* No tag in field: stay quiet */
    }

    /* --- 2. For each detected tag: print UID, read memory, parse --- */
    platformLog("--- SOF (%d tag) ---\r\n", devCnt);
    for (int i = 0; i < devCnt; i++)
    {
        /* InvRes.UID is in transmitted order (LSB first, UID[7]=0xE0).
         * Print it MSB-first to match the way UIDs are usually written. */
        uint8_t *uid = devList[i].InvRes.UID;
        char uidStr[3 * RFAL_NFCV_UID_LEN];
        int p = 0;
        for (int k = RFAL_NFCV_UID_LEN - 1; k >= 0; k--)
        {
            p += snprintf(&uidStr[p], sizeof(uidStr) - p,
                          (k == RFAL_NFCV_UID_LEN - 1) ? "%02X" : ":%02X",
                          uid[k]);
        }
        platformLog("Tag_%d UID=%s\r\n", i + 1, uidStr);

        /* Read 16 blocks (64 bytes) addressed by this tag's own UID */
        uint8_t  rxBuf[1 + RAW_MEMORY_SIZE + RFAL_CRC_LEN];
        uint16_t rcvLen = 0;

        err = rfalNfcvPollerReadMultipleBlocks(
                  RFAL_NFCV_REQ_FLAG_DEFAULT, uid,
                  0, READ_BLOCKS - 1, rxBuf, sizeof(rxBuf), &rcvLen);
        if (err != RFAL_ERR_NONE) /* retry once */
        {
            err = rfalNfcvPollerReadMultipleBlocks(
                      RFAL_NFCV_REQ_FLAG_DEFAULT, uid,
                      0, READ_BLOCKS - 1, rxBuf, sizeof(rxBuf), &rcvLen);
        }

        if (err != RFAL_ERR_NONE || rcvLen <= 1)
        {
            platformLog("  read failed (err=%d)\r\n", (int)err);
            continue;
        }

        /* Try to parse an NDEF Text record (D1 01 xx 54) */
        uint8_t *rawMemory = &rxBuf[1];
        int      memSize    = rcvLen - 1;
        bool     parsed     = false;

        for (int idx = 0; idx < memSize - 6; idx++)
        {
            if (rawMemory[idx] == 0xD1 && rawMemory[idx + 1] == 0x01 &&
                rawMemory[idx + 3] == 0x54)
            {
                uint8_t payloadLen = rawMemory[idx + 2];
                uint8_t statusByte = rawMemory[idx + 4];
                int     langLen    = statusByte & 0x1F;
                int     textStart  = idx + 5 + langLen;
                int     textLen    = payloadLen - 1 - langLen;

                if (textLen > 0 && textLen < 64 &&
                    (textStart + textLen) <= memSize)
                {
                    char parsedData[64] = {0};
                    memcpy(parsedData, &rawMemory[textStart], textLen);
                    parsedData[textLen] = '\0';
                    platformLog("  Text: %s\r\n", parsedData);
                    parsed = true;
                    break;
                }
            }
        }

        /* No NDEF text found: dump the raw memory as hex so nothing is lost */
        if (!parsed)
        {
            platformLog("  RAW: %s\r\n", hex2Str(rawMemory, (size_t)memSize));
        }
    }
    platformLog("--- EOF ---\r\n");
}

/*
 ******************************************************************************
 * INITIALIZATION
 ******************************************************************************
 */
bool demoIni(void)
{
    ReturnCode err;

    /* --- SPI sanity check: read ST25R500 IC_ID directly before RFAL init --- */
    /* SPI was already initialized by BSP_NFC0XCOMM_Init() before "Welcome".  */
    {
        uint8_t icid = 0xFFU;
        st25r500ReadRegister(ST25R500_REG_IC_ID, &icid);
        platformLog("[demoIni] IRQ pin(PA0) = %s\r\n",
                    platformGpioIsHigh(ST25R_INT_PORT, ST25R_INT_PIN) ? "HIGH" : "low");
        platformLog("[demoIni] ST25R500 IC_ID = 0x%02X (ST25R500 if (id & 0xF8)==0xB0)\r\n",
                    icid);
        if ((icid & 0xF8U) != 0xB0U)
        {
            platformLog("[demoIni] !! SPI/chip NOT responding (bad IC_ID). "
                        "Check shield seating / SPI wiring.\r\n");
        }
    }

    platformLog("[demoIni] calling rfalNfcInitialize()...\r\n");
    err = rfalNfcInitialize();
    platformLog("[demoIni] rfalNfcInitialize ret=%d\r\n", (int)err);
    if (err != RFAL_ERR_NONE)
    {
        return false;
    }

    /* Direct NFC-V poller mode: turn the RF field on and keep it on.
     * We poll tags ourselves in demoCycle() via inventory + read, instead of
     * relying on the rfalNfcDiscover()/notifyCb path (which never fired here). */
    rfalNfcvPollerInitialize();
    rfalFieldOnAndStartGT();
    platformDelay(300);                 /* let field/tags settle */

    platformLog(">>> NFC-V Read Mode ready (field ON)\r\n");
    state = DEMO_ST_DISCOVERY;
    return true;
}

void demoCycle(void)
{
    demoPollOnce();
    platformDelay(200);
}

void demoStop(void)
{
    rfalNfcDeactivate(RFAL_NFC_DEACTIVATE_IDLE);
    state = DEMO_ST_NOTINIT;
}

/* Unused stubs */
static void demoNfcv(rfalNfcvListenDevice *nfcvDev) { (void)nfcvDev; }
static void demoCE(rfalNfcDevice *nfcDev) { (void)nfcDev; }
ReturnCode demoTransceiveBlocking(uint8_t *txBuf, uint16_t txBufSize, uint8_t **rxData, uint16_t **rcvLen, uint32_t fwt) { return RFAL_ERR_NONE; }
