/**********************************************************************
 *
 * 实验名称：RTC定时中断
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：RTC时钟源选择LPOCLK-1KHZ，板载LED每隔1S轮显一次       
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"

int main (void);
void RTC_Task(void);

uint8_t i=0;
/********************************************************************/
int main (void)
{
    uint8_t u8Ch; 
    uint16_t u16ModuloValue;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;

    /* 系统初始化 */
    sysinit();

    printf("\nRunning the RTC_demo project.\r\n");
    LED0_Init();  
    LED1_Init();
    LED2_Init();
    
    /* 配置RTC的中断频率为1HZ */ 
    u16ModuloValue = 0x09;//模值为10
    pRTC_Config->u16ModuloValue = u16ModuloValue;//装载值到模数寄存器中
    pRTC_Config->bInterruptEn   = RTC_INTERRUPT_ENABLE;    //使能中断 
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;         //选取时钟源为1KHZ
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;   //分频数为100
    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTC_Config); 
	
    while(1)
    {
    } 
}

/*****************************************************************************//*!
*
* @RTC中断任务函数，闪烁LED，三个LED轮显
*       
* @无返回
*
*****************************************************************************/

void RTC_Task(void)
{	
	i=i+1;

	if(i%3==2)
	{
		LED0_On();  
		LED1_Off();
		LED2_Off();
	}
	else if(i%3==1)
	{
		LED0_Off();
		LED1_On(); 
		LED2_Off();
	}
else
	{
		LED0_Off();
		LED1_Off();
		LED2_On();  
	}

}
/********************************************************************/
