/**********************************************************************
 *
 * 实验名称：模数转换实验(软件触发)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：初始化ADC模块，选择转换输入通道开始转换。转换完成触发中断， 
 *           在中断服务函数中读取转换结果
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "adc.h"
#include "sysinit.h"

/******************************************************************************/

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
  	printf("\nRunning the ADC_Int_demo project.\r\n");
  
    UART_WaitTxComplete(TERM_PORT);   
    
    /*初始化ADC模块*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;            /*!< ADC时钟源分频系数为4*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;    /*!< ADC时钟源选择总线时钟*/
    sADC_Config.u8Mode = ADC_MODE_12BIT;                   /*!< 12位转换*/
    sADC_Config.sSetting.bIntEn = 1;                       /*!< 使能中断*/

    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);    /*!< 初始化ADC模块*/
    
	while(1)
	{
	    /* 设置转换通道，开始转换*/
	    u8ADC_ConversionFlag = 0;
      ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
      /* 等待转换完成*/
      while( !u8ADC_ConversionFlag);

       /* 设置转换通道，开始转换*/
	    u8ADC_ConversionFlag = 0;
      ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
      /* 等待转换完成*/
      while( !u8ADC_ConversionFlag);

      /* 设置转换通道，开始转换*/
	    u8ADC_ConversionFlag = 0;
      ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
      /* 等待转换完成*/
      while( !u8ADC_ConversionFlag);

        printf("ADC conversion result as below:\r\n");
        for( u8Ch =0 ;u8Ch< u16ADC_ConversionCount; u8Ch ++)
        {

            printf("0x%x,",u16ADC_ConversionBuff[u8Ch]);
        }
        printf("\r\n");
        printf("input any character to start a new conversion!\r\n");
        //u8Ch = UART_GetChar(TERM_PORT);
        u16ADC_ConversionCount = 0;
	} 

}

/***************************************************************************
+FUNCTION----------------------------------------------------------------
*
* @brief 回调函数读取转换结果
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void ADC_CallBack( void )
{
    if( u16ADC_ConversionCount < 3 ) //读取转换结果
    {
        u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
    }

     u8ADC_ConversionFlag = 1;
}
/********************************************************************/
