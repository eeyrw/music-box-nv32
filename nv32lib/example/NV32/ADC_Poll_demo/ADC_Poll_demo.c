/**********************************************************************
 *
 * ʵ�����ƣ�ģ��ת��ʵ��(�������)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч������ʼ��ADCģ�飬ѡ��ת������ͨ����ʼת����������ѯ�ķ�ʽ��ȡ
 *           ת����� 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "adc.h"
#include "sysinit.h"

int main (void);

/********************************************************************/
int main (void)
{
    ADC_ConfigType  sADC_Config = {0};
    
    /* ϵͳ��ʼ��*/
    sysinit();
  	printf("\nRunning the ADC_Poll_demo project.\r\n");

    UART_WaitTxComplete(TERM_PORT);   
    
    /*��ʼ��ADCģ��*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;          /*!< ADCʱ��Դ��Ƶϵ��Ϊ8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;  /*!< ADCʱ��Դѡ������ʱ��*/
    sADC_Config.u8Mode = ADC_MODE_12BIT;                 /*!< 12λת��*/
    
    ADC_Init( ADC, &sADC_Config);  /*!< ��ʼ��ADCģ��*/

	while(1)
	{
	       /*����ת��ͨ����ʼת�����ȴ�ת����ɶ�ȡת�����*/
        printf("VREFH conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD29_VREFH));
        printf("VREFL conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD30_VREFL));
        printf("Temperature sensor conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD22_TEMPSENSOR));

        printf("input any character to start a new conversion!\r\n");
        UART_GetChar(TERM_PORT);
        
	} 

}

/********************************************************************/
