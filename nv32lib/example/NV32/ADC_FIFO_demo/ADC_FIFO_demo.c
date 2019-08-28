/**********************************************************************
 *
 * ʵ�����ƣ�ģ��ת������(FIFOģʽ�������)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����FIFO����趨Ϊ3������ʼ��ADCģ���,��FIFO�ɼ���������ʼ
 *           ת����Ȼ��ͨ����ȡת������Ĵ�����ȡFIFO�е�ת�������ÿ��
 *           ��ȡһ��ת�������ȡ3�� 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "adc.h"
#include "pmc.h"
#include "sysinit.h"

/**********************************************************************/
uint16_t u16ADC_ConversionBuff[16];
uint16_t u16ADC_ConversionCount = 0;
volatile uint8_t  u8ADC_ConversionFlag = 0;

int main (void);
void ADC_CallBack( void );
/******************************************************************************/

int main (void)
{
    uint8_t         u8Ch;
    ADC_ConfigType  sADC_Config = {0};

    /*ϵͳ��ʼ��*/
	  sysinit();
  	printf("\nRunning the ADC_FIFO_demo project.\r\n");

    UART_WaitTxComplete(TERM_PORT);   
    
    /*��ʼ��ADCģ��*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;       /*!< ADCʱ��Դ��Ƶϵ��Ϊ8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_ADACK;   /*!< ADCʱ��Դѡ���첽ʱ��*/
    sADC_Config.u8Mode = ADC_MODE_12BIT;              /*!< 12λת��*/
    sADC_Config.sSetting.bIntEn = 1;                  /*!< ʹ���ж�*/
    sADC_Config.u8FiFoLevel = ADC_FIFO_LEVEL3;        /*!< 3��FIFO */
    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);           /*!< ��ʼ��ADC*/

		
	while(1)
	{
	      /*ѡ��ͨ����ʼת��*/
	      u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
        /*�ȴ�ת����� */
        while( !u8ADC_ConversionFlag);

        printf("ADC conversion result as below:\r\n");
        for( u8Ch =0 ;u8Ch< u16ADC_ConversionCount; u8Ch ++)
        {

            printf("0x%x,",u16ADC_ConversionBuff[u8Ch]);
        }
        printf("\r\n");
        printf("input any character to start a new conversion!\r\n");
       // u8Ch = UART_GetChar(TERM_PORT);
        u16ADC_ConversionCount = 0;
	} 

}


/***************************************************************************
+FUNCTION----------------------------------------------------------------
*
* @brief  ADC�ص������У��ڻص��������ȡת�����
*        
* @param  none
*
* @return none
*

*****************************************************************************/

void ADC_CallBack( void )
{    
	
    while( !ADC_IsFIFOEmptyFlag(ADC) ) //���FIFO������Ч��������
    {
        if( u16ADC_ConversionCount < 3 )  //��ȡת������������FIFO�е�����ȫ������
        {
            u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
        }
        else
        {
            ADC_ReadResultReg(ADC); //��ת������Ĵ���
        }		
    }

    u8ADC_ConversionFlag = 1;
}
/********************************************************************/
