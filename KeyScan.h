#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__
#include <stdint.h>

#define KEY_ACTIVE 0
#define SCAN_INTERVAL_MS 20

enum KEY_INDEX
{
    USER_KEY_1 = 0,
    USER_KEY_2,
    KEY_NUM
};

enum KEY_STATUS
{
    KEY_STATUS_IDLE = 0,
    KEY_STATUS_PRESSED,
    KEY_STATUS_RELEASED,
};

void KeyRawInput(uint32_t keyIndex, uint32_t keyValue);
void KeyDefaultCallBack(uint32_t param);
void KeySetCallBack(uint32_t keyIndex, void (*cb)(uint32_t));
void KeyProcess(uint32_t currentMills);
void KeyScanInit(void);

#endif

