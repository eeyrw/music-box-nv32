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
  //FGPIOB->PSOR = GPIO_PTB2_MASK;
  Player32kProc(&mPlayer);
  //FGPIOB->PCOR = GPIO_PTB2_MASK;
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

void DeConfigPIT(void)
{
  PIT_ConfigType sPITConfig0;
  PIT_ConfigType *pPIT_Config0 = &sPITConfig0;
  /* PIT时钟源为总线时钟 */
  /* 通道0装载值为 = (1000000-1),通道1装载值为 = (40-1) */

  /* 配置通道0, 仅仅使能 */
  pPIT_Config0->u32LoadValue = 1249;
  pPIT_Config0->bFreeze = FALSE;   //定时器在调试模式下继续运行
  pPIT_Config0->bModuleDis = TRUE; //使能定时器模块
  pPIT_Config0->bInterruptEn = FALSE;
  pPIT_Config0->bChainMode = FALSE;
  pPIT_Config0->bETMerEn = FALSE; //定时器使能

  PIT_Init(PIT_CHANNEL0, pPIT_Config0); //初始化PIT模块通道0
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
  return volChn1 >> 4;
}

void ETM0Config(void)
{
  SIM->SCGC |= SIM_SCGC_ETM0_MASK;                                         //使能ETM2时钟
  ETM0->CONTROLS[0].CnSC = ETM_CnSC_ELSA_MASK | (1 << ETM_CnSC_MSB_SHIFT); //低真脉冲

  ETM_ClockSet(ETM0, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置

  ETM0->MOD = 0xFFFF;
  ETM0->CONTROLS[0].CnV = 0xFFFF;
}

void ETM2Config(void)
{
  SIM->SCGC |= SIM_SCGC_ETM2_MASK;             //使能ETM2时钟
  ETM2->COMBINE &= ~ETM_COMBINE_COMBINE2_MASK; //通道0和通道1独立
  ETM2->SC |= ETM_SC_CPWMS_MASK;               //选择先增后减的计数方式
  ETM2->COMBINE |= ETM_COMBINE_COMP2_MASK;     //通道0和通道1的输出互补
  ETM2->CONTROLS[4].CnSC = ETM_CnSC_ELSA_MASK; //低真脉冲
  ETM2->CONTROLS[5].CnSC = ETM_CnSC_ELSA_MASK;

  ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2时钟设置

  ETM_SetModValue(ETM2, 255);
}
/********************************************************************/
int main(void)
{
  uint32_t i;

  sysinit();
  //GPIO_Init(GPIOA, GPIO_PTB2_MASK, GPIO_PinOutput);
  GPIO_Init(GPIOA, GPIO_PTB1_MASK, GPIO_PinInput);
  GPIO_Init(GPIOA, GPIO_PTA6_MASK, GPIO_PinInput);
  GPIO_Init(GPIOA, GPIO_PTA7_MASK, GPIO_PinInput);

  SIM_RemapETM0CH0Pin();

  (*((uint32_t *)0x4004900C)) = PORT_HDRVE_PTB4_MASK | PORT_HDRVE_PTB5_MASK | PORT_HDRVE_PTH1_MASK | PORT_HDRVE_PTH0_MASK;

  PlayerInit(&mPlayer);

  ConfigPIT();
  ConfigADC();
  ETM0Config();
  ETM2Config();

  //if(GetVolt()<3000)
  {
    PlayerPlay(&mPlayer);
  }

  while (1)
  {
    mPlayer.mainSynthesizer.mainVolume = GetVolume();
    uint32_t playStatus = PlayerProcess(&mPlayer);
    ETM0->CONTROLS[0].CnV = abs(mPlayer.mainSynthesizer.mixOut) >> 8;
    if (playStatus == STATUS_STOP)
    {
      DeConfigPIT();
      while (1)
        ;
    }
  }
}
