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
#define BLOCK_SIZE           4
#define TAG_TOTAL_BLOCKS     128    /* ST25DV04K = 4 Kbit = 512 B = 128 blocks */

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

/*
 ******************************************************************************
 * STEVAL-SMARTAG1 sensor-data decoder ("st.com:smartag" external record)
 *
 * Layout (SmarTag protocol V1, from ST STNFCSensor / SmarTagLib):
 *   data base   = payload of the "st.com:smartag" NDEF external record
 *   +0          : FW version block
 *   +1..+4      : configuration (interval u16, mode u8, enabled-sensors u8, ...)
 *   +0x0F       : sample-info block (nSample u16, nextWriteByteOffset u16)
 *   +0x10..     : circular sample buffer, 2 blocks (8 bytes) per sample
 *
 * Sample (8 bytes): date(LE u32) + data(LE u32)
 *   date bit31      : 1 = event sample, 0 = sensor sample; date = value & 0x7FFFFFFF
 *   sensor data     : pressure=(d>>20)&0xFFF -> /10 +810 mbar  (0xFFF invalid)
 *                     temperature=(d>>13)&0x7F -> -40 C        (0x7F  invalid)
 *                     humidity=(d>>6)&0x7F -> +0 %             (0x7F  invalid)
 *                     acceleration=d&0x3F -> *256 mg           (0x3F  invalid)
 ******************************************************************************
 */
static uint16_t le16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static uint32_t le32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

/* Decode and print one 8-byte sample. */
static void decodeSmartagSample(int n, const uint8_t *s)
{
    uint32_t dateW = le32(s);
    uint32_t data  = le32(s + 4);
    uint32_t cv    = dateW & 0x7FFFFFFFu;

    int year   = (int)((cv >> 26) & 0x3F) + 2018;
    int month  = (int)((cv >> 17) & 0x0F);
    int day    = (int)((cv >> 21) & 0x1F);
    int hour   = (int)((cv >> 12) & 0x1F);
    int minute = (int)((cv >> 6)  & 0x3F);
    int second = (int)( cv        & 0x3F);

    char ts[24];
    snprintf(ts, sizeof(ts), "%04d-%02d-%02d %02d:%02d:%02d",
             year, month, day, hour, minute, second);

    if (dateW & 0x80000000u) /* event sample */
    {
        uint32_t orient = data & 0x07u;
        uint32_t accEv  = (data >> 3) & 0x3Fu;
        uint32_t accRaw = (data >> 9) & 0x3Fu;
        platformLog("  #%d %s EVENT orient=%lu accEvents=0x%02lX",
                    n, ts, (unsigned long)orient, (unsigned long)accEv);
        if (accRaw != 0x3F) platformLog(" Acc=%lumg", (unsigned long)(accRaw * 256));
        platformLog("\r\n");
        return;
    }

    uint32_t pRaw = (data >> 20) & 0xFFFu;
    uint32_t tRaw = (data >> 13) & 0x7Fu;
    uint32_t hRaw = (data >> 6)  & 0x7Fu;
    uint32_t aRaw =  data        & 0x3Fu;

    platformLog("  #%d %s", n, ts);
    if (tRaw != 0x7F)  platformLog(" T=%dC",      (int)tRaw - 40);
    if (hRaw != 0x7F)  platformLog(" H=%lu%%",    (unsigned long)hRaw);
    if (pRaw != 0xFFF) platformLog(" P=%lu.%lumbar",
                                   (unsigned long)(pRaw / 10 + 810),
                                   (unsigned long)(pRaw % 10));
    if (aRaw != 0x3F)  platformLog(" Acc=%lumg",  (unsigned long)(aRaw * 256));
    platformLog("\r\n");
}

/* Locate the "st.com:smartag" record and decode the logged samples. */
static void decodeSmartag(const uint8_t *mem, int memLen)
{
    static const char TYPE[] = "st.com:smartag";
    const int TLEN = (int)sizeof(TYPE) - 1;

    int typeStart = -1;
    for (int i = 0; i + TLEN <= memLen; i++)
    {
        if (memcmp(&mem[i], TYPE, (size_t)TLEN) == 0) { typeStart = i; break; }
    }
    if (typeStart < 1)
    {
        platformLog("  (no st.com:smartag record found)\r\n");
        return;
    }

    /* Parse the NDEF record header that precedes the type field to locate the
     * payload. The header byte position depends on SR (1- vs 4-byte payload
     * length) and IL (presence of an ID-length byte), so try each combination
     * and keep the one whose flags (TNF=external) and type length are valid. */
    int payloadBase = -1;
    for (int sr = 0; sr <= 1 && payloadBase < 0; sr++)
    {
        for (int il = 0; il <= 1; il++)
        {
            int plBytes = sr ? 1 : 4;
            int fpos    = typeStart - (2 + plBytes + (il ? 1 : 0));
            if (fpos < 0) continue;
            uint8_t flags = mem[fpos];
            if ((flags & 0x07) != 0x04) continue;        /* TNF = external      */
            if (((flags >> 4) & 1) != sr)  continue;      /* SR bit must match   */
            if (((flags >> 3) & 1) != il)  continue;      /* IL bit must match   */
            if (mem[fpos + 1] != TLEN)     continue;      /* type length = 14    */
            int idLen   = il ? mem[fpos + 2 + plBytes] : 0;
            payloadBase = typeStart + TLEN + idLen;       /* data base (FW block)*/
            break;
        }
    }
    if (payloadBase < 0 || payloadBase % 4 != 0)
    {
        platformLog("  (could not parse SmarTag NDEF header)\r\n");
        return;
    }
    int dataBaseBlk = payloadBase / 4;
    int cfgOff      = payloadBase + 4;            /* config block (+1) */
    int siOff       = payloadBase + 0x0F * 4;     /* sample-info block (+0x0F) */

    if (siOff + 4 > memLen)
    {
        platformLog("  (memory too short for SmarTag layout)\r\n");
        return;
    }

    /* An unconfigured / never-logged tag has 0xDEADBEEF in these blocks. */
    bool configured = (le32(&mem[cfgOff]) != 0xDEADBEEFu);

    uint16_t interval   = le16(&mem[cfgOff]);
    uint8_t  logMode    = mem[cfgOff + 2];
    uint8_t  enSensors  = mem[cfgOff + 3];
    uint16_t nSample    = configured ? le16(&mem[siOff]) : 0;
    uint16_t nextByteOf = le16(&mem[siOff + 2]);

    int firstSampleBlk = dataBaseBlk + 0x10;
    int nextSampleBlk  = nextByteOf / 4;
    int lastBlk        = memLen / 4 - 1;
    int numMax         = (lastBlk - firstSampleBlk) / 2;
    if (numMax < 1) numMax = 1;

    if (!configured)
    {
        platformLog("  -> tag not configured / no sensor data logged yet "
                    "(configure & start logging on the SMARTAG1 board)\r\n");
        return;
    }

    platformLog("  [SmarTag] interval=%lumin mode=%lu sensors=0x%02lX maxSamples=%d\r\n",
                (unsigned long)interval, (unsigned long)logMode,
                (unsigned long)enSensors, numMax);
    platformLog("  [SmarTag] nSample=%lu\r\n", (unsigned long)nSample);

    if (nSample == 0)
    {
        platformLog("  -> no sensor samples logged yet "
                    "(start logging on the SMARTAG1 board)\r\n");
        return;
    }
    if (nSample > numMax) nSample = (uint16_t)numMax;

    /* Oldest sample: if the ring is full it starts at the next write slot. */
    int nextIdx  = (nextSampleBlk - firstSampleBlk) / 2;
    int startIdx = (nSample >= numMax) ? nextIdx : 0;

    for (int k = 0; k < nSample; k++)
    {
        int idx = (startIdx + k) % numMax;
        int off = (firstSampleBlk + idx * 2) * 4;
        if (off + 8 > memLen) break;
        decodeSmartagSample(k + 1, &mem[off]);
    }
}

/* One polling pass: inventory the NFC-V tag in the field, read & decode it,
 * but only print when the content changes (new tag, or new logged data) so
 * the serial monitor isn't flooded with the same samples every cycle.
 * The RF field is left ON by demoIni and stays on; called every demoCycle(). */
static void demoPollOnce(void)
{
    static bool     hadTag = false;
    static char     lastUid[3 * RFAL_NFCV_UID_LEN] = "";
    static uint32_t lastSig = 0;

    rfalNfcvListenDevice devList[MAX_DEVICES];
    uint8_t              devCnt = 0;
    ReturnCode           err;

    /* --- 1. Inventory: find any NFC-V tag(s) present --- */
    err = rfalNfcvPollerCollisionResolution(
              RFAL_COMPLIANCE_MODE_NFC, MAX_DEVICES, devList, &devCnt);

    if (err != RFAL_ERR_NONE || devCnt == 0)
    {
        if (hadTag) /* report removal once, then stay quiet */
        {
            platformLog("(no tag)\r\n");
            hadTag = false; lastUid[0] = '\0'; lastSig = 0;
        }
        return;
    }

    /* Use the first tag for change detection (single-tag use case). */
    uint8_t *uid = devList[0].InvRes.UID;
    char uidStr[3 * RFAL_NFCV_UID_LEN];
    int  p = 0;
    for (int k = RFAL_NFCV_UID_LEN - 1; k >= 0; k--)
    {
        p += snprintf(&uidStr[p], sizeof(uidStr) - p,
                      (k == RFAL_NFCV_UID_LEN - 1) ? "%02X" : ":%02X", uid[k]);
    }

    /* Read the whole tag memory (ST25DV04K = 128 blocks / 512 B). */
    static uint8_t mem[TAG_TOTAL_BLOCKS * BLOCK_SIZE];
    int            memLen = 0;
    for (int b = 0; b < TAG_TOTAL_BLOCKS; b += 8)
    {
        uint8_t  buf[1 + (8 * BLOCK_SIZE) + RFAL_CRC_LEN];
        uint16_t rl = 0;
        err = rfalNfcvPollerReadMultipleBlocks(
                  RFAL_NFCV_REQ_FLAG_DEFAULT, uid,
                  (uint8_t)b, 7, buf, sizeof(buf), &rl);
        if (err != RFAL_ERR_NONE || rl < (1 + 8 * BLOCK_SIZE)) break;
        memcpy(&mem[b * BLOCK_SIZE], &buf[1], 8 * BLOCK_SIZE);
        memLen = b * BLOCK_SIZE + 8 * BLOCK_SIZE;
    }
    if (memLen < 64) return; /* transient read error: skip this pass */

    /* Signature over the whole memory (FNV-1a) to detect changes. */
    uint32_t sig = 2166136261u;
    for (int i = 0; i < memLen; i++) { sig ^= mem[i]; sig *= 16777619u; }

    /* Same tag and same data as last printed pass -> don't repeat. */
    if (hadTag && sig == lastSig && strcmp(uidStr, lastUid) == 0) return;

    hadTag  = true;
    lastSig = sig;
    strncpy(lastUid, uidStr, sizeof(lastUid) - 1);
    lastUid[sizeof(lastUid) - 1] = '\0';

    /* Content changed (tag placed or new data): print it once. */
    platformLog("--- SOF (%d tag) ---\r\n", devCnt);
    platformLog("Tag_1 UID=%s\r\n", uidStr);
    decodeSmartag(mem, memLen);
    if (devCnt > 1)
    {
        platformLog("  (+%d more tag(s) present)\r\n", devCnt - 1);
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
    platformDelay(1000);
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
