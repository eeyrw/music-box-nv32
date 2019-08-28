/**********************************************************************
 *
 * 实验名称：看门狗--喂狗/刷新例程
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：提供看门狗框架，看门狗的使能需要在NV32_config.h宏定义ENABLE_WDOG
 *            
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "wdog.h"
#include "sim.h"
#include "sysinit.h"

int main (void);
void	DelayUS(uint32_t u32ETMeUS);

/********************************************************************/
int main (void)
{
    WDOG_ConfigType sWDOGConfig = {0};    /*! < 声明看门狗配置结构体 */

    /* 执行系统初始化 */
    sysinit();

    sWDOGConfig.sBits.bWaitEnable   = TRUE;
    sWDOGConfig.sBits.bStopEnable   = TRUE;
    sWDOGConfig.sBits.bDbgEnable    = TRUE;
    sWDOGConfig.sBits.bUpdateEnable = FALSE;
    sWDOGConfig.sBits.bDisable      = FALSE;        /* 使能看门狗*/
    sWDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    sWDOGConfig.u16ETMeOut          = 1000;  /*< 1s */
    sWDOGConfig.u16WinETMe          = 0;  

    WDOG_Init(&sWDOGConfig);

    printf("\nRunning the wdog_feed_demo project.\r\n");

    if(WDOG_IsReset())                      /*!< 检查有无看门狗复位 */
    {
			 
        LED0_Init();                        /*!< 初始化LED0 */
        while(1)
        {
            LED0_Toggle();                  /*!< 看门狗复位发生LED0闪烁 */
            WDOG_Feed();            
            DelayUS(50000);                 /*!< 延迟50MS */
        }
    }
    LED2_Init();                            /*!< 初始化LED2 */
    while(1)
    {
        LED2_Toggle();                      /*!< 没有看门狗复位发生，则LED2闪烁 */
        DelayUS(50000);                     /*!< 延迟50MS */
    } 

}

/**************************************************************************//*!
*
* @brief 使用RTC作微秒级延迟. 
*        
* @param[in]    u32ETMeUS   所要延迟的数量级.
*
* @return none
*
*****************************************************************************/
void  DelayUS(uint32_t u32ETMeUS)
{
    RTC_ConfigType  sRTCConfig, *pRTC_Config;
      
    pRTC_Config = &sRTCConfig;

    /* configure RTC to 1us period */ 
    pRTC_Config->u16ModuloValue = u32ETMeUS/4-1;
    pRTC_Config->bInterruptEn   = FALSE;                    /*!< 禁用中断 */
    pRTC_Config->bClockSource   = RTC_CLKSRC_BUS;           /*!< 选择总线时钟为时钟源 */
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;    /*!< 分频系数为100 */
    RTC_Init(&sRTCConfig);
    while(!RTC_GetFlags())
        ;
    RTC_ClrFlags();
    RTC_DeInit();    
}

/*****************************************************************************/


