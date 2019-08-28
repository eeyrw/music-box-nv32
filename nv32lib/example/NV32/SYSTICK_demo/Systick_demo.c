/**********************************************************************
 *
 * 实验名称：滴答定时器Systick中断例程
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：提供SYSTICK定时器的中断框架 ，完成LED闪烁
 *           工程中完成100mS的定时中断，SYSTICK时钟为内核时钟，在Tick_Init函数中
 *           对SysTick->LOAD赋值，其中(BUS_CLK_HZ)/10-1为100MS，(BUS_CLK_HZ/1000)-1为1MS，
 *           以此类推。
 *           注：对LOAD寄存器的赋值不得超过24位，该寄存器为低24位有效，最大值为16777215
 *          
************************************************************************/
#include "common.h"
#include "rtc.h"
#include "pmc.h"
#include "uart.h"
#include "sysinit.h"
#include "sim.h"
#include "core_cm0plus.h"
#include "systick.h"

int main (void);         
void Tick_Init(void);
void SysTick_CallBack(void);  

int main (void)
{	
	sysinit(); 
	printf("\nRunning the Systick_demo project.\n"); 
	LED2_Init(); 
	Tick_Init();
	
	while(1);
}

/****************************************************************************//*!
*
* @brief SYSTICK定时中断初始化
*        

*****************************************************************************/
void Tick_Init(void)  
{ 
  SysTick_SetCallBack(SysTick_CallBack);	
  SysTick->CTRL =0; 
  SysTick->LOAD  =  (BUS_CLK_HZ)/10-1; //配置定时中断时间为100MS        
  SysTick->VAL   = 0;        
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk| SysTick_CTRL_CLKSOURCE_Msk; //开中断开定时器	

}


/****************************************************************************//*!
*
* @brief Systick中断服务子函数
*
*****************************************************************************/
void SysTick_CallBack(void)        
{ 
	LED2_Toggle();
}
/********************************************************************/
