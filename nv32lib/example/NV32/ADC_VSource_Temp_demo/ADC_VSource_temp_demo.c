/**********************************************************************
 *
 * 实验名称：内部温度传感器实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：ADC采集内部温度传感器对应的电压值，将其转换成数字量，然后计算出当前温度
 *           值通过串口打印出来  
 * 
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "adc.h"
#include "sysinit.h"
#include "pmc.h"

#define VDD  3.3 //测试R4,R5中间的电压值

int main (void);

/**********************************************************************/
int main (void)
{
		float Vtemp,m,temp,VBG;
		int32_t advrefh,adtemp,advcore;
    ADC_ConfigType  sADC_Config = {0};
	  PMC_ConfigType  PMC_Config={0};
    
    sysinit();

		PMC_Config.sCtrlstatus.bits.bBandgapEn = 1; //使能带隙
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; 
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;
    PMC_Init(PMC, &PMC_Config);  //初始化PMC模块
		
    UART_WaitTxComplete(TERM_PORT);   
    
    /*初始化ADC模块*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;     /*!< ADC时钟源分频系数为8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2; /*!< ADC时钟源选择总线时钟2分频*/
    sADC_Config.u8Mode = ADC_MODE_10BIT;  		/*!< 10位转换*/
		sADC_Config.sSetting.bLongSampleEn=1;    /*!<长采样模式,*/
    
    ADC_Init( ADC, &sADC_Config);   /*!<初始化ADC模块*/
  
	while(1)
	{
		advrefh=ADC_PollRead(ADC,ADC_CHANNEL_AD29_VREFH);     /*ADC读取Vrefh的值*/
		adtemp=ADC_PollRead(ADC,ADC_CHANNEL_AD22_TEMPSENSOR); /*!< ADC读取温传感器数值*/
		advcore= ADC_PollRead(ADC,ADC_CHANNEL_AD23_BANDGAP);  /*!< ADC读取带隙值*/
		VBG = VDD*advcore/advrefh;  /*<计算带隙电压值*/
			
		Vtemp = VBG*adtemp/advcore;  /*<计算当前温度下温度传感器对应的电压值*/
		printf("VBG value:%f\r\n",VBG);	
		printf("Temperature sensor  conversion value:%f\r\n",Vtemp);
   /**********************************************************************************************
		注：当温度为25 度时温度传感器对应的电压值，Vtemp25=1.419V。当Vtemp>Vtemp25 时表示当前温度低
            于25 度，当Vtemp<Vtemp25 时表示当前温度高于25 度
            M：温度相对电压变化率，当温度大于25 度时,M=-3.65mv/℃,当温度小于25 度时M=-3.4mv/℃
    **********************************************************************************************/
		if(Vtemp >1.419) m=-3.4;  
	  else m=-3.65;
		temp = 25 - (1.419-Vtemp)*1000/m;  /*<计算当前温度*/
	  printf("Temperature  value:%f\r\n",temp);
    UART_GetChar(TERM_PORT);
        
	} 

}

/********************************************************************/
