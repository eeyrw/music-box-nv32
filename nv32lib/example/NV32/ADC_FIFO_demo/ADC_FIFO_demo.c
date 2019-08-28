/**********************************************************************
 *
 * 实验名称：模数转换试验(FIFO模式软件触发)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：FIFO深度设定为3级，初始化ADC模块后,当FIFO采集数据满则开始
 *           转换，然后通过读取转换结果寄存器读取FIFO中的转换结果。每次
 *           读取一个转换结果读取3次 
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

    /*系统初始化*/
	  sysinit();
  	printf("\nRunning the ADC_FIFO_demo project.\r\n");

    UART_WaitTxComplete(TERM_PORT);   
    
    /*初始化ADC模块*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;       /*!< ADC时钟源分频系数为8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_ADACK;   /*!< ADC时钟源选择异步时钟*/
    sADC_Config.u8Mode = ADC_MODE_12BIT;              /*!< 12位转换*/
    sADC_Config.sSetting.bIntEn = 1;                  /*!< 使能中断*/
    sADC_Config.u8FiFoLevel = ADC_FIFO_LEVEL3;        /*!< 3级FIFO */
    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);           /*!< 初始化ADC*/

		
	while(1)
	{
	      /*选择通道开始转换*/
	      u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
        ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
        /*等待转化完成 */
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
* @brief  ADC回调函数中，在回调函数里读取转换结果
*        
* @param  none
*
* @return none
*

*****************************************************************************/

void ADC_CallBack( void )
{    
	
    while( !ADC_IsFIFOEmptyFlag(ADC) ) //结果FIFO中有有效的新数据
    {
        if( u16ADC_ConversionCount < 3 )  //读取转换结果，将结果FIFO中的数据全部读出
        {
            u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
        }
        else
        {
            ADC_ReadResultReg(ADC); //读转换结果寄存器
        }		
    }

    u8ADC_ConversionFlag = 1;
}
/********************************************************************/
