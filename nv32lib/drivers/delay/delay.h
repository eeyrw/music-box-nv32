#ifndef __DELAY_H__
#define __DELAY_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "sysinit.h"
#include "common.h"
#include "core_cm0plus.h"
//������ʵ�ֵĽӿں����б�
void DelayInit(void);           //��ʱ��ʼ��
void DelayUs(uint32_t us);      //��ʱus
void DelayMs(uint32_t ms);      //��ʱms

#ifdef __cplusplus
}
#endif

#endif
