/********************************************************************************
 *
 * 实验名称：硬件触发ADC转换(FIFO模式)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：设定FIFO深度，初始化ADC模块,ADC开始采集数据放入FIFO中，FIFO满
						后等待硬件触发信号。
						1:单次转换模式，  当ADC检测到第一个硬件触发信号后，开始转换FIFO
							                中第一个数据，检测到第二个硬件触发信号后，开始
															转换FIFO中第2个数据，以此类推直到FIFO中的数据全部
															转换完，然后触发ADC中断读取转换结果。当转换完成后，
															如果出现新的触发脉冲，将生产新的转换集合。
															例：当FIFO深度设定为3时，ADC要检测到三个硬件触发信号
															才能将FIFO中的数据全部转换完成，然后产生中断。
						2:连续转换模式,   当出现硬件触发信号时，会触发ADC连续采样，建议在使用
						                  FIFO数据模式时，在中断服务函数中读取FIFO结果前禁用ADC
															硬件触发使能，在读取完FIFO结果之后再打开硬件触发使能，
															避免数据错乱。
***********************************************************************************/
#include "common.h"
#include "ics.h"
#include "sim.h"
#include "rtc.h"
#include "pit.h"
#include "uart.h"
#include "adc.h"
#include "etm.h"
#include "sysinit.h"

uint16_t u16ADC_ConversionBuff[16];
uint16_t u16ADC_ConversionCount = 0;
volatile uint8_t  u8ADC_ConversionFlag = 0;

int main (void);
void ADC_CallBack( void );
/********************************************************************/
int main (void)
{
    uint16_t u16ModuloValue;
    uint32_t        u32LoadValue0;
    ADC_ConfigType  sADC_Config = {0};
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;
    PIT_ConfigType  sPITConfig0;
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0;
  
    /*系统初始化*/
    sysinit();
		
		
    printf("\nRunning the ADC_HWT_demo project.\r\n");
    /*********************ADC硬件触发源选择**********************************/
		/**************************************************************************
		 *注：ETM2通道4与NMI中断输入引脚共用PB4,当选择ETM2通道4作为ADC硬件触发源时，
		 *    需要禁用NMI中断输入引脚
     ****************************************************************************/
    SIM_TriggerADCByRTC();         //选择RTC溢出触发ADC转换
    //SIM_TriggerADCByPIT();         //选择PIT溢出触发ADC
    //SIM_TriggerADCByETM2Init();    //选择ETM2初始后延时一段时间触发ADC转换
    //SIM_TriggerADCByETM2Match();   //选择ETM2匹配后延时一段时间触发ADC转换
		
    /**********设置ETM2从匹配/初始化到触发ADC转换的延时时间*****************/
    SIM_SetClockOutputDivide(0x07); //总线时钟输出分频
    SIM_DelayETM2Trig2ADC(0xff);  //延时计数模数值
		
    UART_WaitTxComplete(TERM_PORT);
    LED0_Init();
		
    /* 初始化ADC模块*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;       /*!< ADC时钟源分频系数为8 */
    sADC_Config.u8ClockSource = CLOCK_SOURCE_ADACK;   /*!< ADC时钟源选择异步时钟 */
    sADC_Config.u8Mode = ADC_MODE_12BIT;              /*!< 12位转换 */
    sADC_Config.sSetting.bIntEn = 1;                  /*!< 使能中断*/
    sADC_Config.u8FiFoLevel = ADC_FIFO_LEVEL3;        /*!< 3级FIFO */
    sADC_Config.sSetting.bContinuousEn=0;             /*!< 单次转换模式 */
    sADC_Config.sSetting.bHardwareTriggerEn=1;        /*!< 使能硬件触发模式 */
    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);    //初始化ADC模块
		
    /*RTC溢出触发ADC */
	
    u16ModuloValue = 0x09;
    pRTC_Config->u16ModuloValue = u16ModuloValue; //设定模数值
    pRTC_Config->bInterruptEn=0;   //禁用中断
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;  //选择1KHz时钟源
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;   //时钟分频系数100
    RTC_Init(pRTC_Config);
		
		
    /*PIT0溢出触发ADC */
    /*
      u32LoadValue0   = 0xFFFFFF;       //设定装载值
      pPIT_Config0->u32LoadValue      = u32LoadValue0;
      pPIT_Config0->bFreeze           = FALSE;    //在调试模式时仍然运行
      pPIT_Config0->bModuleDis        = FALSE;    //使能PIT模块
      pPIT_Config0->bChainMode        = FALSE;    //禁用链模式
      pPIT_Config0->bInterruptEn      = FALSE;    //禁用中断
      pPIT_Config0->bETMerEn          = TRUE;     //使能PIT通道
      PIT_Init(PIT_CHANNEL0, pPIT_Config0);
    */
    /*选择ETM2匹配触发*/
    /*
     ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE);
     ETM_SetETMEnhanced(ETM2);
     ETM2->EXTTRIG |= ETM_EXTTRIG_CH1TRIG_MASK;//使能通道1匹配触发
     ETM_SetModValue(ETM2, 29999);  // 设置ETM2模值
     ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 5000); //设置ETM2通道值
     ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV128);  //设置ETM2模块时钟源及分频系数.
     NVIC_DisableIRQ(ETM2_IRQn);
    */
    /*选择ETM2初始触发*/
    /*
    ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE);
    ETM_SetETMEnhanced(ETM2);
    ETM_SetCounterInitValue(ETM2, 3000);
    ETM2->EXTTRIG |= ETM_EXTTRIG_INITTRIGEN_MASK; //初始化触发使能
    ETM_SetModValue(ETM2, 9999); // 设置ETM2模值
    ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 5000);  //设置ETM2通道值
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV128);  //设置ETM2模块时钟源及分频系数.
     */
		 
    /***********设置转换通道，等待触发信号 ***********************/
    ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
    ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
    ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
    while(1)
    {
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
    uint8_t         u8Ch;
    LED0_Toggle();
    while( !ADC_IsFIFOEmptyFlag(ADC) )  ////结果FIFO中有有效的新数据
    {
        if( u16ADC_ConversionCount < 3 )  //读取转换结果，将结果FIFO中的数据全部读出
        {
            u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
        }
        else
        {
            ADC_ReadResultReg(ADC);
        }
    }
    /*打印转换结果*/
    printf("ADC conversion result as below:\r\n");
    for( u8Ch =0 ; u8Ch< u16ADC_ConversionCount; u8Ch ++)
    {
        printf("0x%x,",u16ADC_ConversionBuff[u8Ch]);
    }
    printf("\r\n");
    u16ADC_ConversionCount = 0;;
}
/********************************************************************/
