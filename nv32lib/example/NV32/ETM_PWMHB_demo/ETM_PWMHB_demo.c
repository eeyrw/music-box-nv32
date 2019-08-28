/**********************************************************************
 *
 * 实验名称：PWM互补输出
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：选用FEI内部时钟，总线时钟40Mhz，联合ETM2的通道0(PH0)和通道1(PH1)互补输出
 *
************************************************************************/
#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"


int main (void)
{     

    sysinit();//系统初始化
 
    SIM_RemapETM2CH0Pin();//映射对应通道管脚到PH0
    SIM_RemapETM2CH1Pin();//映射对应通道管脚到PH1
	
    SIM->SCGC  |= SIM_SCGC_ETM2_MASK;							//使能ETM2时钟
    ETM2->COMBINE &= ~ ETM_COMBINE_COMBINE0_MASK; //通道0和通道1独立
	  ETM2->SC |= ETM_SC_CPWMS_MASK;								//选择先增后减的计数方式
	  ETM2->COMBINE |= ETM_COMBINE_COMP0_MASK; 			//通道0和通道1的输出互补
	  ETM2->CONTROLS[0].CnSC = ETM_CnSC_ELSA_MASK;  //低真脉冲
    ETM2->CONTROLS[1].CnSC = ETM_CnSC_ELSA_MASK; 	
	 
   
	  ETM_SetModValue(ETM2, 3999);//设置频率10KHz
	
    ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL0, 2000);//设置占空比为50%
		ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 2000);	

		ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置


	  while(1);

}

