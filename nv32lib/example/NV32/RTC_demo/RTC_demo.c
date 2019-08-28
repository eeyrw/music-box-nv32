/**********************************************************************
 *
 * ʵ�����ƣ�RTC��ʱ�ж�
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����RTCʱ��Դѡ��LPOCLK-1KHZ������LEDÿ��1S����һ��       
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"

int main (void);
void RTC_Task(void);

uint8_t i=0;
/********************************************************************/
int main (void)
{
    uint8_t u8Ch; 
    uint16_t u16ModuloValue;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;

    /* ϵͳ��ʼ�� */
    sysinit();

    printf("\nRunning the RTC_demo project.\r\n");
    LED0_Init();  
    LED1_Init();
    LED2_Init();
    
    /* ����RTC���ж�Ƶ��Ϊ1HZ */ 
    u16ModuloValue = 0x09;//ģֵΪ10
    pRTC_Config->u16ModuloValue = u16ModuloValue;//װ��ֵ��ģ���Ĵ�����
    pRTC_Config->bInterruptEn   = RTC_INTERRUPT_ENABLE;    //ʹ���ж� 
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;         //ѡȡʱ��ԴΪ1KHZ
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;   //��Ƶ��Ϊ100
    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTC_Config); 
	
    while(1)
    {
    } 
}

/*****************************************************************************//*!
*
* @RTC�ж�����������˸LED������LED����
*       
* @�޷���
*
*****************************************************************************/

void RTC_Task(void)
{	
	i=i+1;

	if(i%3==2)
	{
		LED0_On();  
		LED1_Off();
		LED2_Off();
	}
	else if(i%3==1)
	{
		LED0_Off();
		LED1_On(); 
		LED2_Off();
	}
else
	{
		LED0_Off();
		LED1_Off();
		LED2_On();  
	}

}
/********************************************************************/
