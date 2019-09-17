#include "KeyScan.h"
#include <stdint.h>
#include <stddef.h>

uint32_t keyStatusList[KEY_NUM];
uint32_t keyValueList[KEY_NUM];
void (*keyCallBackList[KEY_NUM])(uint32_t);

void KeyRawInput(uint32_t keyIndex, uint32_t keyValue)
{
    if (keyIndex < KEY_NUM)
        keyValueList[keyIndex] = keyValue;
}

void KeyDefaultCallBack(uint32_t param)
{
    param = param;
}

void KeySetCallBack(uint32_t keyIndex, void (*cb)(uint32_t))
{
    if (keyIndex < KEY_NUM)
        keyCallBackList[keyIndex] = cb;
}

void KeyProcess(uint32_t currentMills)
{
    static uint32_t lastMills = 0;
    if ((currentMills - lastMills) >= SCAN_INTERVAL_MS)
    {
        lastMills = currentMills;

        for (size_t keyIndex = 0; keyIndex < KEY_NUM; keyIndex++)
        {
            switch (keyStatusList[keyIndex])
            {
            case KEY_STATUS_IDLE:
                if (keyValueList[keyIndex] == KEY_ACTIVE)
                    keyStatusList[keyIndex] = KEY_STATUS_PRESSED;
                break;
            case KEY_STATUS_PRESSED:
                if (keyValueList[keyIndex] != KEY_ACTIVE)
                    keyStatusList[keyIndex] = KEY_STATUS_RELEASED;
                break;
            case KEY_STATUS_RELEASED:
                keyCallBackList[keyIndex](keyStatusList[keyIndex]);
                keyStatusList[keyIndex] = KEY_STATUS_IDLE;
                break;
            default:
                break;
            }
        }
    }
}

void KeyScanInit(void)
{
    for (size_t keyIndex = 0; keyIndex < KEY_NUM; keyIndex++)
    {
        keyCallBackList[keyIndex] = KeyDefaultCallBack;
        keyStatusList[keyIndex] = KEY_STATUS_IDLE;
        keyValueList[keyIndex] = !KEY_ACTIVE;
    }
}