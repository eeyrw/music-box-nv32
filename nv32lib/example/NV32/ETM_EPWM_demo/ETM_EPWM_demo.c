/**********************************************************************
 *
 * 实验名称：ETM边沿PWM输出--呼吸灯
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：ETM2通道1输出边沿对齐的PWM波，来控制NV32F100板上的D2闪烁 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"

void ETM2_Task(void);

/********************************************************************/
int main (void)
{     
    /* 初始化 */
    sysinit();
    
    SIM_RemapETM2CH1Pin();    //选择映射ETM2CH1到PH1引脚，默认ETM2CH1对应PC1
    
    ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE); 
	  ETM_disblechannel(ETM2,0);//ETM_PWMInit函数默认打开所有通道功能，若不想其他引脚输出/用作其他功能，需要禁用。
	  ETM_disblechannel(ETM2,2);
    ETM_disblechannel(ETM2,3);
	  ETM_disblechannel(ETM2,4);
   	ETM_disblechannel(ETM2,5);
    /* 更新MOD的值 */
    ETM_SetModValue(ETM2, 9999);
    /* 设置时钟源，使能计数器 */
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);  
    /* 使能ETM2的IRQ中断 */
    NVIC_EnableIRQ(ETM2_IRQn);
    /* 设置中断回调函数 */
    ETM_SetCallback(ETM2, ETM2_Task);
    /* 使能ETM2溢出中断 */
    ETM_EnableOverflowInt(ETM2);

		while(1);
}

/*****************************************************************************//*!
*
* @brief ETM2中断回调任务函数
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
uint16_t u16ChV_old, u16ChV_new;
void ETM2_Task(void)
{
    static uint16_t u16count;
    static uint8_t u8DirMark;
 
   
    ETM_ClrOverFlowFlag(ETM2);//清除溢出标志位
          
    if(100 == u16count)
    {
        u16count = 0;
        u16ChV_old = ETM2->CONTROLS[1].CnV;
        if(!u8DirMark)
        {
            u16ChV_new = u16ChV_old + 200;
            if(u16ChV_new >= ETM2->MOD)
            {
                u16ChV_new = ETM2->MOD - 200; 
                u8DirMark  = 1;
            }
            else
            {
            }
        }
        else
        {
            u16ChV_new = u16ChV_old - 200;
            if(u16ChV_new <  200)
            {
                u16ChV_new = 200; 
                u8DirMark  = 0;
            }
            else
            {  
            }
        }
        /* 刷新通道值 */
        ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, u16ChV_new);
    }
    else
    {
        u16count++;
    }
}
/********************************************************************/
