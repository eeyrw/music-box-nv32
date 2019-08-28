/**********************************************************************
 *
 * 实验名称：KBI键盘中断实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：初始化KBI模块，选择中断输入引脚，和检测模式，按下按键触发KBI
 *           中断切换LED状态
 *					 
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "kbi.h"
#include "sysinit.h"

int main (void);
void KBI0_Task(void);
void KBI1_Task(void);

/********************************************************************/
int main (void)
{
    uint8_t         i;
    KBI_ConfigType  sKBIConfig;
  
    /*系统初始化*/
	  sysinit();
  	printf("\nRunning the KBI_demo project.\r\n");
    LED1_Init(); 
    LED2_Init();

	  PORT->IOFLT = 0xa4be0000; //KBI0、KBI1输入滤波,KBI0和KBI小于32MS的毛刺都会被滤除   //Modify
	
    UART_WaitTxComplete(TERM_PORT);   
    
     /* 禁用KBI中断输入引脚*/
    for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
    {
     sKBIConfig.sPin[i].bEn	 = 0;
    }
    
    sKBIConfig.sBits.bMode   = 0;   /*!< 选择  边沿检测  */    
    sKBIConfig.sPin[0].bEdge = KBI_FALLING_EDGE_LOW_LEVEL; /*!< 选择 下降沿或低电平检测 */
    sKBIConfig.sBits.bIntEn  = 1; //使能KBI中断
    sKBIConfig.sPin[0].bEn   = 1; //使能KBI中断输入引脚
    
    KBI_Init(KBI0, &sKBIConfig); //初始化KBI0
    KBI_Init(KBI1, &sKBIConfig); //初始化KBI1
    KBI_SetCallback(KBI0, &KBI0_Task); //设置回调函数
    KBI_SetCallback(KBI1, &KBI1_Task);
    
   
		while(1);

}

/*****************************************************************************//*!
*
* @brief KBI0回调函数
*        
* @param  none.
*
* @return none
*
*****************************************************************************/

void KBI0_Task(void)
{
    LED1_Toggle();
    
    printf("KBI0 routinue.\r\n");
}

/*****************************************************************************//*!
*
* @brief KBI1回调函数. 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/

void KBI1_Task(void)
{
    LED2_Toggle();
    
    printf("KBI1 routinue.\r\n");
}
/********************************************************************/
