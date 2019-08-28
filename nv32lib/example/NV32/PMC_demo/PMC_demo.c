/**********************************************************************
 *
 * 实验名称：停止模式：RTC中断唤醒MCU
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：MCU进入STOP3模式后，经过10延时，RTC产生中断将MCU从STOP3模式下
 *          唤醒  
 * 
************************************************************************/
#include "common.h"
#include "rtc.h"
#include "pmc.h"
#include "uart.h"
#include "sysinit.h"
#include "sim.h"

int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
	  uint8_t u8Ch; 
    PMC_ConfigType  PMC_Config={0};
    RTC_ConfigType  RTC_Config={0};

    /*系统初始化*/
	  sysinit();

  	printf("\nRunning the PMC_demo project.\r\n");
    
    LED0_Init();
		LED0_Off();
  
    
    PMC_Config.sCtrlstatus.bits.bBandgapEn = 0;
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; //低压检测在停止模式下禁用
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;  //禁用低压检测复位
    PMC_Init(PMC, &PMC_Config);   //初始化PMC模块
    PMC_DisableLVWInterrupt(PMC); //禁用低压报警中断
    u8Ch = PMC_GetLVWFlag(PMC);
    
    RTC_Config.u16ModuloValue = 0x09;  //设置RTC模值
    RTC_Config.bClockSource   = RTC_CLKSRC_1KHZ; //选择1KHZ时钟源
    RTC_Config.bClockPresaler = RTC_CLK_PRESCALER_1000; //设置时钟分频系数
    RTC_SetCallback(RTC_Task);  //设置回调函数
    RTC_Config.bInterruptEn   = 1; //使能RTC中断
    RTC_Init(&RTC_Config);  //初始化RTC模块
		
   /*在执行STOP指令前完成RTC初始化*/
	 /*MCU进入停止模式，经过10延时，由RTC中断唤醒*/
	  
    printf("\nEnter stop mode and will be woken up in about 10s by RTC Irq.\r\n");
    PMC_SetMode(PMC,PmcModeStop3); 
    while(!(ICS->S & ICS_S_LOCK_MASK));//从停止模式唤醒后，需要等待FLL稳定
		printf("\nWake up now.\r\n");
	  while(1);
 
}

/*****************************************************************************//*!
*
* @brief RTC回调函数 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
void RTC_Task(void)
{
    LED0_Toggle();
}
/********************************************************************/
