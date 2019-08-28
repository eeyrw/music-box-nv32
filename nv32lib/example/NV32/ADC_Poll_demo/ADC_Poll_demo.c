/**********************************************************************
 *
 * 实验名称：模数转换实验(软件触发)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：初始化ADC模块，选择转换输入通道开始转换。采用轮询的方式读取
 *           转换结果 
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
    
    /* 系统初始化*/
    sysinit();
  	printf("\nRunning the ADC_Poll_demo project.\r\n");

    UART_WaitTxComplete(TERM_PORT);   
    
    /*初始化ADC模块*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;          /*!< ADC时钟源分频系数为8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;  /*!< ADC时钟源选择总线时钟*/
    sADC_Config.u8Mode = ADC_MODE_12BIT;                 /*!< 12位转换*/
    
    ADC_Init( ADC, &sADC_Config);  /*!< 初始化ADC模块*/

	while(1)
	{
	       /*设置转换通道开始转换，等待转换完成读取转换结果*/
        printf("VREFH conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD29_VREFH));
        printf("VREFL conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD30_VREFL));
        printf("Temperature sensor conversion value:0x%x\r\n",ADC_PollRead(ADC,ADC_CHANNEL_AD22_TEMPSENSOR));

        printf("input any character to start a new conversion!\r\n");
        UART_GetChar(TERM_PORT);
        
	} 

}

/********************************************************************/
