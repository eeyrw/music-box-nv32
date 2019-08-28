/**********************************************************************
 *
 * 实验名称：模拟比较实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：选择ACMP正负输入,初始化ACMP模块,正负输入引脚输入数据开始
 *          比较,比较完成产生中断,在中断服务函数中清除中断标志位 
 * 
************************************************************************/

#include "common.h"
#include "acmp.h"
#include "pmc.h"
#include "sysinit.h"

int main (void);
void FunForCallback(void);

/********************************************************************/
int main (void)
{
    ACMP_ConfigType ACMP_Config;
    PMC_ConfigType  PMC_Config={0};
		
    /*系统初始化*/
    sysinit();    
    PMC_Config.sCtrlstatus.bits.bBandgapEn = 1; //使能带隙
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; //低压检测在停止模式下禁用
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;  //禁用低压检测复位
    PMC_Init(PMC, &PMC_Config);  								//初始化PMC模块
    PMC_DisableLVWInterrupt(PMC); 							//禁用低压报警中断

    printf("\nRunning the ACMP_demo project.\r\n");

		
    /*初始化ACMP模块 */
    ACMP_Config.sCtrlStatus.bits.bIntEn = TRUE; /*使能中断*/
		ACMP_Config.sCtrlStatus.bits.bMod= 1; 			/*中断在输出上升沿触发*/
    ACMP_Config.sCtrlStatus.bits.bOutEn = 1;    /* ACMP输出置于外部引脚  */
    ACMP_Config.sPinSelect.bits.bNegPin = 0x3;  /* 负输入引脚:DAC */
    ACMP_Config.sPinSelect.bits.bPosPin = 0;    /* 正输入引脚:外部基准0，ACMP0_IN0-PTA0 */
    ACMP_Config.sDacSet.bits.bEn        = TRUE; /* 使能 DAC */    
    ACMP_Config.sDacSet.bits.bRef       = DAC_REF_VDDA;    /* DAC基准电压选择:Vdda */
    ACMP_Config.sDacSet.bits.bVal       = 0x1F; 					 /* DAC输出电平位：Vin/2 */
    ACMP_Config.sPinEnable.bits.bEn     = TRUE; /* 使能外部基准0引脚输入 */
    
    ACMP_SetCallback(ACMP0,FunForCallback);     /*注册回调函数*/
    
    ACMP_Init(ACMP0, &ACMP_Config);             /*初始化ACMP模块*/
    
    ACMP_Enable(ACMP0);                         /*使能ACMP */
  
  /************************************************************
		printf("\nEnter stop mode and will be woken up  by ACMP Irq.\r\n");
    PMC_SetMode(PMC,PmcModeStop3);  //MCU进入停止模式，通过ACMP中断唤醒
    printf("\nWake up now.\r\n");
	 *******************************************************************/
	 while(1);

}

/*****************************************************************************//*!
*
* @brief ACMP回调函数.
*        
* @param none.
*
* @return none.
*
*****************************************************************************/
void FunForCallback(void)
{
    if(ACMP_GetFlag(ACMP0))
    {    
        ACMP_ClrFlag(ACMP0);  //清除中断标志位
    }
    
    printf("\nCallback happens.\r\n");
}
