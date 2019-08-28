/**********************************************************************
 *
 * 实验名称：PIT定时中断
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验说明：PIT例程设置定时器1链接至定时器0产生每秒1次的中断.
 *				  以总线时钟在本例程中为40MHZ，所以说，时钟周期为25ns ,产生一秒的中断需要40,000,000个时钟周期，中断任务函数中LED灯翻转. 
 * 
 *					设定PIT0触发1,000,000个周期, PIT1定时器1触发40次，总周期为40,000,000个时钟周期 
 *
************************************************************************/

#include "common.h"
#include "uart.h"
#include "pit.h"
#include "sysinit.h"


int main (void);
void PIT_Task(void);
/*****************************************************************************
*****************************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    uint32_t        u32LoadValue0, u32LoadValue1;     
    PIT_ConfigType  sPITConfig0, sPITConfig1;    
    PIT_ConfigType  *pPIT_Config1   =&sPITConfig1;
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 

    /* 系统初始化 */
    sysinit();
    
    printf("\nRunning the PIT_demo project.\r\n");
    
    LED0_Init();

    /* PIT时钟源为总线时钟 */
    /* 通道0装载值为 = (1000000-1),通道1装载值为 = (40-1) */
    u32LoadValue0   = 0xF423F;                  /* 通道0装载值 */ 
    u32LoadValue1   = 0x13;                     /* 通道1装载值 */      
    
    /* 配置通道1为链接模式,开启中断并且使能 */
    pPIT_Config1->u32LoadValue      = u32LoadValue1;
    pPIT_Config1->bFreeze           = FALSE;    //定时器在调试模式下继续运行
    pPIT_Config1->bModuleDis        = FALSE;    //使能定时器模块
    pPIT_Config1->bInterruptEn      = TRUE;     //开启对应通道的IRQ中断
    pPIT_Config1->bChainMode        = TRUE;     //定时器链接到PIT0
    pPIT_Config1->bETMerEn          = TRUE;     //定时器使能
    
    /* 配置通道0, 仅仅使能 */   
    pPIT_Config0->u32LoadValue      = u32LoadValue0;
    pPIT_Config0->bFreeze           = FALSE;    //定时器在调试模式下继续运行
    pPIT_Config0->bModuleDis        = FALSE;    //使能定时器模块      
    pPIT_Config0->bInterruptEn      = FALSE;    
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bETMerEn          = TRUE;     //定时器使能

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);  //初始化PIT模块通道0               
    PIT_Init(PIT_CHANNEL1, pPIT_Config1);  //初始化PIT模块通道1
      
    PIT_SetCallback(PIT_CHANNEL1, PIT_Task);  //设置通道1中断回调函数   
    
		while(1)     
		{
		} 

}

/*****************************************************************************//*!
*
* @PIT中断任务函数
*        
* @无输入
*
* @无返回
*
*****************************************************************************/

void PIT_Task(void)
{
    LED0_Toggle();          /*!< 闪烁  */
}
/********************************************************************/
