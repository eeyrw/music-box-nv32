/**********************************************************************
 *
 * 实验名称：ETM双边沿捕捉
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：跳变沿脉冲捕获，由 ETM0的通道1产生脉冲信号,周期为10000,
 *           脉冲宽度为5000,使用ETM2的通道0作为捕获口，将ETM2 Ch0（PC0）
 *           与ETM0 Ch0（PB2）短接。     
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "ETM.h"
#include "uart.h"
#include "sysinit.h"


void ETM2_Task(void);

volatile uint16_t u16Ch0Value,u16Ch1Value;
volatile uint8_t u8IntMark;
uint16_t x,y;

/********************************************************************/
int main (void)
{
    /* 初始化 */
    sysinit();

    /* ETM0通道0（即PB2脚）输出方波即PB2脚, 周期10000，脉冲宽度为5000 */ 
    SIM_RemapETM0CH0Pin();
    ETM_OutputCompareInit(ETM0, ETM_CHANNEL_CHANNEL0, ETM_OUTPUT_TOGGLE);//Modify
    /* 装载ETM0的MOD计数值 */
    ETM_SetModValue(ETM0, 5000);
    /* 设置时钟*/ 
    ETM_ClockSet(ETM0, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
   
    /* 配置ETM2的通道用来测出脉冲的宽度或周期*/
    ETM_DualEdgeCaptureInit(  ETM2,    
                              ETM_CHANNELPAIR0,                      /* 通道对0:通道0和通道1 */
                              ETM_INPUTCAPTURE_DUALEDGE_ONESHOT,     /* 单次模式 */
                              ETM_INPUTCAPTURE_DUALEDGE_RISINGEDGE,  /* 通道0检测上升沿 */
                              ETM_INPUTCAPTURE_DUALEDGE_FALLInGEDGE  /* 通道1检测下降沿 */
                            );
    /*ETM2的时钟设置,为系统时钟，1分频*/
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
    
    ETM_SetCallback(ETM2, ETM2_Task);
    NVIC_EnableIRQ(ETM2_IRQn);//使能IRQ中断
    ETM_EnableChannelInt(ETM2, (ETM_CHANNELPAIR0+1));//使能通道中断
    
    
	while(1)
	{
        if(u8IntMark) 
        {
            u16Ch0Value = ETM2->CONTROLS[0].CnV;
            u16Ch1Value = ETM2->CONTROLS[1].CnV;
            u8IntMark   = 0;//反转标志位
            x = u16Ch0Value;
            y = u16Ch1Value;
            printf("\n Dual edge capture end. The input wave period is %d\n",(uint16_t)(y-x));
            
             /* 重新开启跳变沿捕捉 */
            ETM2->COMBINE |=  (ETM_COMBINE_DECAP0_MASK << (ETM_CHANNELPAIR0 * 4)); 
        }
	} 
}

/*****************************************************************************//*!
*
* @brief ETM2中断回调任务函数
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void ETM2_Task(void)
{
    ETM_ClrChannelFlag(ETM2, ETM_CHANNELPAIR0);
    ETM_ClrChannelFlag(ETM2, ETM_CHANNELPAIR0+1);
    u8IntMark = 1;//置位标志位
}
/********************************************************************/
