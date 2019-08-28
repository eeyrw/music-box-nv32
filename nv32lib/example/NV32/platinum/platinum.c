/**********************************************************************
 *
 * 实验名称：MCU时钟模式切换实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：MCU的时钟模式由当前的FEE模式切换到FEI模式 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"


int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    ICS_ConfigType  sICSConfig;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
    UART_ConfigType sConfig;

    /*系统初始化*/
    sysinit();
    
    printf("\nRunning the platinum project.\r\n");
    LED0_Init();  
    LED2_Init();

    /*初始化RTC模块，每隔1s产生一次中断*/
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /*使能中断 */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*时钟源1KHz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*时钟分频系数100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

    printf("\nIt is in FEE mode now.\r\n");
    UART_WaitTxComplete(TERM_PORT);   

    /*将时钟模式由FEI模式切换到FEE模式*/ 
		sICSConfig.oscConfig.bGain = 1;			 /* 使能高增益 */
		sICSConfig.oscConfig.bRange = 1;     /* 使能高范围 */
    sICSConfig.oscConfig.bEnable = 1;    /* 使能 OSC  */
		sICSConfig.oscConfig.bIsCryst = 1;   /* OSC的输出选择选择振动器时钟源 */ 
    sICSConfig.oscConfig.bWaitInit = 1;  /* 等待振荡器初始化化完成 */       
    sICSConfig.u32ClkFreq = 10000;       /* 板载为10M晶振*/
    ICS_SwitchMode(FEI,FEE, &sICSConfig);
    ICS_SetBusDivider(1); 							 //总线时钟不得高于40M主频，所以10M晶振256分频再1280倍频后为50M，需要经过BDIV二分频为25M
    /* 由于时钟模式转变，总线时钟发生变化从新初始化UART模块*/
    sConfig.u32SysClkHz = 25000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEE mode.\r\n");

    while(1);
}

/*****************************************************************************//*!
*
* @brief RTC模块回调函数
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void RTC_Task(void)
{
    /*切换LED0状态 */
    LED0_Toggle();
}
/********************************************************************/
