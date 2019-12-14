/******************************************************************************
*
* @brief provide systick utility routines.
*
*******************************************************************************/

#include "common.h"
#include "systick.h"
//#include "stdint.h"
uint32_t cnt_start_value;
uint32_t cnt_end_value;
uint32_t overhead;
SysTick_CallbackType SysTick_Callback[1] = {NULL};
#if 0
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
#endif
void systick_init(void)
{
  
	  SysTick->VAL = 0x0;	/* clear current ETMer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;   
}

void SysTick_SetCallBack(SysTick_CallbackType pSysTick_CallBack)
{
    SysTick_Callback[0] = pSysTick_CallBack;
}

/***
void delay_us(int n)//微秒级别的延时
{		
	int temp;	    	 
	SysTick->LOAD= 48 *n; //????	  		 
	SysTick->VAL=0x00000000;        //?????
	SysTick->CTRL=0x00000005;
  while(SysTick->CTRL&0x00010000);	//???? 	 
	SysTick->CTRL=0x00000004;
}
***/

/****
void delay_ms(int n)//毫秒级别的延时
{		
	int temp;	    	 
	SysTick->LOAD= 48000 *n; //????	  		 
	SysTick->VAL=0x00000000;        //?????
	SysTick->CTRL=0x00000005;
  while(SysTick->CTRL&0x00010000);	//???? 	 
	SysTick->CTRL=0x00000004;
}******////

void systick_disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void cal_systick_read_overhead(void)
{
  	uint32_t cnt_start_value;
    uint32_t cnt_end_value;
	
	cnt_start_value = SysTick->VAL;
  
    cnt_end_value = SysTick->VAL;
	
	overhead = cnt_start_value - cnt_end_value;
	
#ifdef DEBUG_PRINT
	printf("systick start value: 0x%x\n\r", (unsigned int)cnt_start_value);
	printf("systick end value: 0x%x\n\r", (unsigned int) cnt_end_value);
	printf("systick current value read overhead: 0x%x\n\r", (unsigned int)overhead);
#endif

}

extern unsigned int SEGGER_SYSVIEW_TickCnt;
void SysTick_Isr(void)
{ 
	SEGGER_SYSVIEW_TickCnt++;
	//printf("input any character to start a new conversion!\n");
    if( SysTick_Callback[0] )
    {
        SysTick_Callback[0]();
    }
}

