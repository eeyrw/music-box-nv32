/**********************************************************************
 *
 * 实验名称：ETM输出比较
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：ETM0的通道1产生触发波形，该通道所对应的管脚为PB3         
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"

/********************************************************************/
int main (void)
{
    /* 系统初始化 */
    sysinit();

    SIM_RemapETM0CH1Pin();//映射通道到管脚PB3
     /* ETM0为输出比较模式, 通道1产生触发波形 */
    ETM_OutputCompareInit(ETM0, ETM_CHANNEL_CHANNEL1, ETM_OUTPUT_TOGGLE);
    //加载对应的MOD数值
    ETM_SetModValue(ETM0, 5000);
    //设置CnV的值
    ETM_SetChannelValue(ETM0, ETM_CHANNEL_CHANNEL1, 2000);
    //时钟配置/
    ETM_ClockSet(ETM0, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
       
    
		while(1);
}
