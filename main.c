#include "common.h"
#include "rtc.h"
#include "flash.h"
#include "sysinit.h"
#include "etm.h"
#include "eeprom.h"
#include "pit.h"
#include "sim.h"
#include "gpio.h"
#include "adc.h"
#include "Player.h"
#include "NV32.h"

Player mPlayer;

int main(void);
void TestInit(void);
void TestSynth(void);

void PIT_Task(void)
{
  FGPIOB->PSOR = GPIO_PTB2_MASK;
  Player32kProc(&mPlayer);
  FGPIOB->PCOR = GPIO_PTB2_MASK;
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

void ConfigADC(void)
{
  ADC_ConfigType sADC_Config = {0};

  /*初始化ADC模块*/
  sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;         /*!< ADC时钟源分频系数为8*/
  sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK; /*!< ADC时钟源选择总线时钟*/
  sADC_Config.u8Mode = ADC_MODE_12BIT;                /*!< 12位转换*/

  ADC_Init(ADC, &sADC_Config); /*!< 初始化ADC模块*/
}

uint32_t GetVolt(void)
{
  uint32_t voltChn = ADC_PollRead(ADC, ADC_CHANNEL_AD3);
  return voltChn;
}

uint32_t GetVolume(void)
{
  uint32_t volChn1 = ADC_PollRead(ADC, ADC_CHANNEL_AD2);
  return volChn1>>4;
}
/********************************************************************/
int main(void)
{
  uint32_t i;

  sysinit();
  GPIO_Init(GPIOB, GPIO_PTB2_MASK, GPIO_PinOutput);
  GPIO_Init(GPIOB, GPIO_PTB1_MASK, GPIO_PinInput);
  GPIO_Init(GPIOA, GPIO_PTA6_MASK, GPIO_PinInput);
  GPIO_Init(GPIOA, GPIO_PTA7_MASK, GPIO_PinInput);


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

  ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置

  ETM_SetModValue(ETM2, 255);
  

  PlayerInit(&mPlayer);

  if(GetVolt()<3000)
  {
    PlayerPlay(&mPlayer);
  }
  
  ConfigPIT();
  ConfigADC();
  while (1)
  {
    mPlayer.mainSynthesizer.mainVolume = GetVolume();
    PlayerProcess(&mPlayer);
  }
}
