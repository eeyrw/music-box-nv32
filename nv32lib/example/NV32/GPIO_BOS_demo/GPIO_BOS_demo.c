/**********************************************************************
 *
 * ʵ�����ƣ�GPIOλ������
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ͨBOSλ������ʵ��GPIO�Ĵ�����λ������ ��������������ռ���ʺ�CPUִ��ʱ�䣬����ʡ����ռ�
 *
************************************************************************/
#include "common.h"
#include "systick.h"
#include "bos.h"
#include "gpio.h"
#include "delay.h"
#include "sysinit.h"
//#include "start.h"


#define LED0_OFF    PTE7_OUT_HIGH //PE7���Ϊ�ߵ�ƽ
#define LED0_ON     PTE7_OUT_LOW  //PE7���Ϊ�͵�ƽ
#define LED1_OFF    PTH1_OUT_HIGH
#define LED1_ON     PTH1_OUT_LOW 
#define LED2_OFF    PTH2_OUT_HIGH  
#define LED2_ON     PTH2_OUT_LOW 

int main (void);

int main (void)
{
	 uint32_t value;
	
   sysinit();
	
	 DelayInit(); //��ʱ��ʼ��

   BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTE7);  	//����PE7����Ϊ���
	 
	 BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTH1); 	//����PH1����Ϊ���
	 
	 BOS_BIT_SET(&GPIOB->PDDR,GPIO_PTH2); 	//����PH2����Ϊ���
	
	 BOS_BIT_CLEAR(&GPIOA->PDDR,GPIO_PTA1); //����PA1����Ϊ����
	 BOS_BIT_CLEAR(&GPIOA->PIDR,GPIO_PTA1); 
	 BOS_BIT_SET(&PORT->PUEL,GPIO_PTA1);  	//�˿�PA1ʹ������
	
	 value = PTA1_IN;               //��ȡPA1�˿�����
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
