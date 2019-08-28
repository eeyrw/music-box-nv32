/**********************************************************************
 *
 * ʵ�����ƣ��δ�ʱ��Systick�ж�����
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч�����ṩSYSTICK��ʱ�����жϿ�� �����LED��˸
 *           ���������100mS�Ķ�ʱ�жϣ�SYSTICKʱ��Ϊ�ں�ʱ�ӣ���Tick_Init������
 *           ��SysTick->LOAD��ֵ������(BUS_CLK_HZ)/10-1Ϊ100MS��(BUS_CLK_HZ/1000)-1Ϊ1MS��
 *           �Դ����ơ�
 *           ע����LOAD�Ĵ����ĸ�ֵ���ó���24λ���üĴ���Ϊ��24λ��Ч�����ֵΪ16777215
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
* @brief SYSTICK��ʱ�жϳ�ʼ��
*        

*****************************************************************************/
void Tick_Init(void)  
{ 
  SysTick_SetCallBack(SysTick_CallBack);	
  SysTick->CTRL =0; 
  SysTick->LOAD  =  (BUS_CLK_HZ)/10-1; //���ö�ʱ�ж�ʱ��Ϊ100MS        
  SysTick->VAL   = 0;        
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk| SysTick_CTRL_CLKSOURCE_Msk; //���жϿ���ʱ��	

}


/****************************************************************************//*!
*
* @brief Systick�жϷ����Ӻ���
*
*****************************************************************************/
void SysTick_CallBack(void)        
{ 
	LED2_Toggle();
}
/********************************************************************/
