/**********************************************************************
 *
 * 实验名称：GPIO位带操作
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：通BOS位操作，实现GPIO寄存器的位操作。 不仅降低了总线占有率和CPU执行时间，还节省代码空间
 *
************************************************************************/
#include "common.h"
#include "systick.h"
#include "bos.h"
#include "gpio.h"
#include "delay.h"
#include "sysinit.h"
//#include "start.h"


#define LED0_OFF    PTE7_OUT_HIGH //PE7输出为高电平
#define LED0_ON     PTE7_OUT_LOW  //PE7输出为低电平
#define LED1_OFF    PTH1_OUT_HIGH
#define LED1_ON     PTH1_OUT_LOW 
#define LED2_OFF    PTH2_OUT_HIGH  
#define LED2_ON     PTH2_OUT_LOW 

int main (void);

int main (void)
{
	 uint32_t value;
	
   sysinit();
	
	 DelayInit(); //延时初始化

   BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTE7);  	//设置PE7引脚为输出
	 
	 BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTH1); 	//设置PH1引脚为输出
	 
	 BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTH2); 	//设置PH2引脚为输出
	
	 BOS_BIT_CLEAR(&GPIOA->PDDR,GPIO_PTA1); //设置PA1引脚为输入
	 BOS_BIT_CLEAR(&GPIOA->PIDR,GPIO_PTA1); 
	 BOS_BIT_SET(&PORT->PUEL,GPIO_PTA1);  	//端口PA1使能上拉
	
	 value = PTA1_IN;               //读取PA1端口输入
 	 printf("PA1=%X.\r\n",value);
	
   while(1)
   {
			DelayMs(1000);
			LED0_OFF;
			DelayMs(1000);
			LED1_OFF;
			DelayMs(1000);
		  LED2_OFF;
			DelayMs(1000);
			LED2_ON;
			DelayMs(1000);
			LED1_ON;
			DelayMs(1000);
			LED0_ON;
			DelayMs(1000);
			LED0_ON;
			LED1_ON;
			LED2_ON;
			DelayMs(1000);
			LED0_ON;
			LED1_ON;
			LED2_ON;
   } 
}

/********************************************************************/
