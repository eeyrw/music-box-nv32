/**********************************************************************
 *
 * 实验名称：FLASH擦写
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：使用FLASH模拟EEPROM,实现字节的编程
 * 实验说明：NV32F100x系列芯片不带有硬件EEPROM，
 *					除128K FLASH空间外，还带有5K的NVR空间:0x0040_0000-0x0040_13FF
 *					操作与前128K一致，不过该区域在执行整片擦除时不会被擦除，可供用户用来作为数据存储区域
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "flash.h"
#include "sysinit.h"
#include "etm.h"
#include "eeprom.h"
#include "pit.h"
#include "sim.h"
#include "gpio.h"
#include "Player.h"

Player mPlayer;

int main (void);
void TestInit(void);
void TestSynth(void);

void PIT_Task(void)
{
        FGPIOB->PSOR=GPIO_PTE7_MASK;
        Player32kProc(&mPlayer);
        //GPIO_Toggle(GPIOB, GPIO_PTE7_MASK);
        FGPIOB->PCOR=GPIO_PTE7_MASK;

  
}

void ConfigPIT(void)
{
      PIT_ConfigType  sPITConfig0;   
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0;
  /* PIT时钟源为总线时钟 */
    /* 通道0装载值为 = (1000000-1),通道1装载值为 = (40-1) */ 
    
    
    /* 配置通道0, 仅仅使能 */   
    pPIT_Config0->u32LoadValue      = 1249;
    pPIT_Config0->bFreeze           = FALSE;    //定时器在调试模式下继续运行
    pPIT_Config0->bModuleDis        = FALSE;    //使能定时器模块      
    pPIT_Config0->bInterruptEn      = TRUE;    
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bETMerEn          = TRUE;     //定时器使能

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);  //初始化PIT模块通道0               
      
    PIT_SetCallback(PIT_CHANNEL0, PIT_Task);  //设置通道1中断回调函数   
    
}
/********************************************************************/
int main (void)
{ 
    uint32_t i;

		sysinit();
     GPIO_Init(GPIOB, GPIO_PTE7_MASK, GPIO_PinOutput);
  	// TestInit();
    // TestSynth();
    SIM_RemapETM2CH0Pin();//映射对应通道管脚到PH0
    SIM_RemapETM2CH1Pin();//映射对应通道管脚到PH1
	
    SIM->SCGC  |= SIM_SCGC_ETM2_MASK;							//使能ETM2时钟
    ETM2->COMBINE &= ~ ETM_COMBINE_COMBINE0_MASK; //通道0和通道1独立
	  ETM2->SC |= ETM_SC_CPWMS_MASK;								//选择先增后减的计数方式
	  ETM2->COMBINE |= ETM_COMBINE_COMP0_MASK; 			//通道0和通道1的输出互补
	  ETM2->CONTROLS[0].CnSC = ETM_CnSC_ELSA_MASK;  //低真脉冲
    ETM2->CONTROLS[1].CnSC = ETM_CnSC_ELSA_MASK; 	
	 
   
	  ETM_SetModValue(ETM2, 255);//设置频率10KHz
	
    ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL0, 0);//设置占空比为50%
		ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 0);	

		ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置
    

        PlayerInit(&mPlayer);
    PlayerPlay(&mPlayer);
    ConfigPIT();
    while (1)
    {
        PlayerProcess(&mPlayer);
    }
}

