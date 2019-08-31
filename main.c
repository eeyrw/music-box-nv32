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
#include "NV32.h"

Player mPlayer;

int main(void);
void TestInit(void);
void TestSynth(void);

void PIT_Task(void)
{
  FGPIOB->PSOR = GPIO_PTE7_MASK;
  Player32kProc(&mPlayer);
  //GPIO_Toggle(GPIOB, GPIO_PTE7_MASK);
  FGPIOB->PCOR = GPIO_PTE7_MASK;
}

void ConfigPIT(void)
{
  PIT_ConfigType sPITConfig0;
  PIT_ConfigType *pPIT_Config0 = &sPITConfig0;
  /* PIT时钟源为总线时钟 */
  /* 通道0装载值为 = (1000000-1),通道1装载值为 = (40-1) */

  /* 配置通道0, 仅仅使能 */
  pPIT_Config0->u32LoadValue = 1249;
  pPIT_Config0->bFreeze = FALSE;    //定时器在调试模式下继续运行
  pPIT_Config0->bModuleDis = FALSE; //使能定时器模块
  pPIT_Config0->bInterruptEn = TRUE;
  pPIT_Config0->bChainMode = FALSE;
  pPIT_Config0->bETMerEn = TRUE; //定时器使能

  PIT_Init(PIT_CHANNEL0, pPIT_Config0); //初始化PIT模块通道0

  PIT_SetCallback(PIT_CHANNEL0, PIT_Task); //设置通道1中断回调函数
}
/********************************************************************/
int main(void)
{
  uint32_t i;

  sysinit();
  GPIO_Init(GPIOB, GPIO_PTE7_MASK, GPIO_PinOutput);
  // TestInit();
  // TestSynth();
  //SIM_RemapETM2CH0Pin();//映射对应通道管脚到PH0
  //SIM_RemapETM2CH1Pin();//映射对应通道管脚到PH1

  (*((uint32_t *)0x4004900C)) = PORT_HDRVE_PTB4_MASK | PORT_HDRVE_PTB5_MASK | PORT_HDRVE_PTH1_MASK | PORT_HDRVE_PTH0_MASK;

  SIM->SCGC |= SIM_SCGC_ETM2_MASK;             //使能ETM2时钟
  ETM2->COMBINE &= ~ETM_COMBINE_COMBINE2_MASK; //通道0和通道1独立
  ETM2->SC |= ETM_SC_CPWMS_MASK;               //选择先增后减的计数方式
  ETM2->COMBINE |= ETM_COMBINE_COMP2_MASK;     //通道0和通道1的输出互补
  ETM2->CONTROLS[4].CnSC = ETM_CnSC_ELSA_MASK; //低真脉冲
  ETM2->CONTROLS[5].CnSC = ETM_CnSC_ELSA_MASK;

  ETM_SetModValue(ETM2, 255); //设置频率10KHz

  ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL4, 0); //设置占空比为50%
  ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL5, 0);

  ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置

  PlayerInit(&mPlayer);
  PlayerPlay(&mPlayer);
  ConfigPIT();
  while (1)
  {
    PlayerProcess(&mPlayer);
  }
}
