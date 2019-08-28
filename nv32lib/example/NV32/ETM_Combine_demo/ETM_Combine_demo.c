/**********************************************************************
 *
 * 实验名称：通道联合PWM输出
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：ETM2的通道0和通道1组合产生一个PWM方波,通道0管脚为PC0，通道1管脚为PC1      
 *
************************************************************************/
#include "common.h"
#include "rtc.h"
#include "ETM.h"
#include "uart.h"
#include "sysinit.h"

void ETM2_Task(void);

/********************************************************************/
int main (void)
{
    /* 系统初始化*/
    sysinit();

  	printf("\nRunning the ETM_demo project.\r\n");

    LED0_Init();     
    /* 设置ETM2为组合模式 */
    ETM_PWMInit(ETM2, ETM_PWMMODE_COMBINE, ETM_PWM_LOWTRUEPULSE);
    /* 设ETM2的MOD值 */
    ETM_SetModValue(ETM2, 3999); //频率为10KHz
    /* 时钟及分频系数的配置 */
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
    
    ETM_SetCallback(ETM2, ETM2_Task);//设置ETM2回调函数点 
    NVIC_EnableIRQ(ETM2_IRQn);   //使能IRQ中断
    ETM_EnableOverflowInt(ETM2); //开启溢出中断
    
    /* 设置占空比，设置通道对占空比为50% */
    ETM_SetDutyCycleCombine(ETM2, ETM_CHANNEL_CHANNEL1, 50);
    
		while(1);
}

/*****************************************************************************//*!
*
* @brief ETM中断回调任务函数. 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
void ETM2_Task(void)
{
    static uint32_t count;
    
    ETM_ClrOverFlowFlag(ETM2); //清除溢出标志位
    
    if(count == 2000)
    {
        count = 0;
        UART_PutChar(TERM_PORT,'@');//传输字符@到串口1
    }
    else 
    {
        count++;
    }
}
/********************************************************************/

