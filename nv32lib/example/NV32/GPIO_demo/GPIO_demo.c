/**********************************************************************
 *
 * 实验名称：GPIO端口输出配置
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：配置GPIO端口为输出引脚,板载D4灯1S闪烁一次
 * 
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "sysinit.h"

int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
    /*系统初始化*/
    sysinit();

    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;  

    printf("\nRunning the GPIO_demo project.\n");

    /* 配置RTC模块每隔1s产生一次中断 */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* 使能中断*/
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /* 选择1KHz时钟源 */
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /* 时钟分频系数100 */
    
    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

		/* 方法1.初始化PE7为输出引脚--通过32位引脚掩码确定要初始化的引脚 */
     GPIO_Init(GPIOB, GPIO_PTE7_MASK, GPIO_PinOutput);
    /* 方法2.初始化PE7为输出引脚--通过定义的GPIO引脚名确定要初始化的引脚*/
    //GPIO_PinInit(GPIO_PTE7, GPIO_PinOutput); 

    while (1);
}

/*****************************************************************************//*!
*
* @brief RTC中断回调函数
*        
*****************************************************************************/

void RTC_Task(void)
{
    /* 切换PE7输出，闪亮LED1 */
    /* 方法1.切换PE7端口数据输出————通过32位引脚掩码确定要切换输出的引脚 */
      GPIO_Toggle(GPIOB, GPIO_PTE7_MASK);
    
    /* 方法2.切换PE7端口数据输出----通过定义的GPIO引脚名确定要切换输出的引脚 */
    // GPIO_PinToggle(GPIO_PTE7); 
}
/********************************************************************/
