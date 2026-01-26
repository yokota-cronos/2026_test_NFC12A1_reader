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
#include <stdio.h>
#include <string.h>

#if RFAL_SUPPORT_CE && RFAL_FEATURE_LISTEN_MODE
#include "demo_ce.h"
#endif

/*
 ******************************************************************************
 * CONFIGURATION - Modify your Tag UIDs here
 ******************************************************************************
 */

/* Target Tag UIDs (MSB first) */
static uint8_t TARGET_TAGS[5][8] = {
    {0xE0, 0x02, 0x27, 0x69, 0x7A, 0x67, 0x59, 0xD1},  /* Tag 1 */
    {0xE0, 0x02, 0x27, 0x69, 0x7A, 0x6B, 0x8C, 0x75},  /* Tag 2 */
    {0xE0, 0x02, 0x27, 0x00, 0x51, 0xCE, 0xDA, 0xF7},  /* Tag 3 */
	{0xE0, 0x02, 0x27, 0x68, 0x01, 0x8F, 0xCA, 0x5B},  /* Tag 4 */
	{0xE0, 0x02, 0x27, 0x68, 0x01, 0x8F, 0xC8, 0x66}   /* Tag 5 */
};

#define NUM_TARGETS          5
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
static void demoNotif(rfalNfcState st);
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

static void demoNotif(rfalNfcState st)
{
    if (st == RFAL_NFC_STATE_POLL_SELECT || st == RFAL_NFC_STATE_START_DISCOVERY)
    {
        static bool isRunning = false;
        if (isRunning) return;
        isRunning = true;

        platformLog("\r\n>>> Smart Polling Mode (Skip Empty Frames)\r\n");

        /* Initialize NFC-V and enable RF field */
        rfalNfcvPollerInitialize();
        rfalFieldOnAndStartGT();

        platformLog(">>> Field ON, charging tags (500ms)...\r\n");
        platformDelay(500);

        /* Main polling loop */
        while (1)
        {
            /* --- 1. 定义缓存区 --- */
            /* * 我们先把数据存在这里，而不是直接打印。
             * 只有当至少一个 Tag 读成功时，才把这批数据发给串口。
             */
            char printBuffer[NUM_TARGETS][80];
            bool frameHasData = false;         /* 本帧是否有效标志位 */

            /* --- 2. 轮询所有 Tag (静默读取) --- */
            for (int i = 0; i < NUM_TARGETS; i++)
            {
                rfalNfcWorker();

                /* 默认填入 MISS，防止后面有乱码 */
                snprintf(printBuffer[i], sizeof(printBuffer[i]), "MISS");

                /* Skip empty UID entries */
                if (TARGET_TAGS[i][0] == 0x00 && TARGET_TAGS[i][7] == 0x00)
                    continue;

                /* Convert UID to machine format (LSB first) */
                uint8_t machineUID[RFAL_NFCV_UID_LEN];
                for (int k = 0; k < 8; k++) {
                    machineUID[k] = TARGET_TAGS[i][7-k];
                }

                uint8_t  rxBuf[1 + RAW_MEMORY_SIZE + RFAL_CRC_LEN];
                uint16_t rcvLen;
                ReturnCode err;

                /* Read 16 blocks (64 bytes) in one go */
                err = rfalNfcvPollerReadMultipleBlocks(
                    RFAL_NFCV_REQ_FLAG_DEFAULT,
                    machineUID,
                    0, READ_BLOCKS - 1,
                    rxBuf, sizeof(rxBuf), &rcvLen
                );

                /* Retry logic */
                if (err != RFAL_ERR_NONE) {
                    err = rfalNfcvPollerReadMultipleBlocks(
                        RFAL_NFCV_REQ_FLAG_DEFAULT,
                        machineUID,
                        0, READ_BLOCKS - 1,
                        rxBuf, sizeof(rxBuf), &rcvLen
                    );
                }

                /* 解析数据 */
                if (err == RFAL_ERR_NONE && rcvLen > 1)
                {
                    uint8_t *rawMemory = &rxBuf[1];
                    int memSize = rcvLen - 1;

                    /* Search for NDEF header D1 01 xx 54 */
                    for (int idx = 0; idx < memSize - 6; idx++)
                    {
                        if (rawMemory[idx] == 0xD1 && rawMemory[idx+1] == 0x01 && rawMemory[idx+3] == 0x54)
                        {
                            uint8_t payloadLen = rawMemory[idx+2];
                            uint8_t statusByte = rawMemory[idx+4];
                            int langLen = statusByte & 0x1F;
                            int textStart = idx + 5 + langLen;
                            int textLen = payloadLen - 1 - langLen;

                            if (textLen > 0 && textLen < 64 && (textStart + textLen) <= memSize)
                            {
                                char parsedData[64] = {0};
                                memcpy(parsedData, &rawMemory[textStart], textLen);
                                parsedData[textLen] = '\0';

                                /* 解析成功，写入缓存 */
                                snprintf(printBuffer[i], sizeof(printBuffer[i]), "%s", parsedData);

                                /* !!! 关键点：只要有一个成功，标记本帧有效 !!! */
                                frameHasData = true;
                                break;
                            }
                        }
                    }
                    /* 如果读到了数据但不是 NDEF，也算是有数据 (可选) */
                    // if (!frameHasData) { snprintf(printBuffer[i], 80, "RAW_DATA"); frameHasData = true; }
                }
            }

            /* --- 3. 决定是否打印 --- */
            if (frameHasData)
            {
                platformLog("--- SOF ---\r\n");
                for (int i = 0; i < NUM_TARGETS; i++)
                {
                    /* 打印所有 Tag 状态，包括 MISS 的 */
                    platformLog("Tag_%d | %s\r\n", i+1, printBuffer[i]);
                }
                platformLog("--- EOF ---\r\n");
            }
            else
            {
                /* 全是空的，什么都不打印，保持串口安静 */
                // platformLog("."); // 调试用，证明系统还活着
            }

            platformDelay(50);
        }
    }
}

/*
 ******************************************************************************
 * INITIALIZATION
 ******************************************************************************
 */
bool demoIni(void)
{
    ReturnCode err;
    err = rfalNfcInitialize();
    if (err == RFAL_ERR_NONE)
    {
        rfalNfcDefaultDiscParams(&discParam);
        discParam.devLimit      = 1U;
        discParam.techs2Find    = RFAL_NFC_POLL_TECH_V;
        discParam.notifyCb      = demoNotif;
        discParam.totalDuration = 1000U;

        err = rfalNfcDiscover(&discParam);
        if (err != RFAL_ERR_NONE) return false;

        state = DEMO_ST_START_DISCOVERY;
        return true;
    }
    return false;
}

void demoCycle(void)
{
    rfalNfcWorker();
    if (state == DEMO_ST_START_DISCOVERY) state = DEMO_ST_DISCOVERY;
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
