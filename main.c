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
  /* PITʱ��ԴΪ����ʱ�� */
  /* ͨ��0װ��ֵΪ = (1000000-1),ͨ��1װ��ֵΪ = (40-1) */

  /* ����ͨ��0, ����ʹ�� */
  pPIT_Config0->u32LoadValue = 1249;
  pPIT_Config0->bFreeze = FALSE;    //��ʱ���ڵ���ģʽ�¼�������
  pPIT_Config0->bModuleDis = FALSE; //ʹ�ܶ�ʱ��ģ��
  pPIT_Config0->bInterruptEn = TRUE;
  pPIT_Config0->bChainMode = FALSE;
  pPIT_Config0->bETMerEn = TRUE; //��ʱ��ʹ��

  PIT_Init(PIT_CHANNEL0, pPIT_Config0); //��ʼ��PITģ��ͨ��0

  PIT_SetCallback(PIT_CHANNEL0, PIT_Task); //����ͨ��1�жϻص�����
}

void ConfigADC(void)
{
  ADC_ConfigType sADC_Config = {0};

  /*��ʼ��ADCģ��*/
  sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;         /*!< ADCʱ��Դ��Ƶϵ��Ϊ8*/
  sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK; /*!< ADCʱ��Դѡ������ʱ��*/
  sADC_Config.u8Mode = ADC_MODE_12BIT;                /*!< 12λת��*/

  ADC_Init(ADC, &sADC_Config); /*!< ��ʼ��ADCģ��*/
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
  //SIM_RemapETM2CH0Pin();//ӳ���Ӧͨ���ܽŵ�PH0
  //SIM_RemapETM2CH1Pin();//ӳ���Ӧͨ���ܽŵ�PH1

  (*((uint32_t *)0x4004900C)) = PORT_HDRVE_PTB4_MASK | PORT_HDRVE_PTB5_MASK | PORT_HDRVE_PTH1_MASK | PORT_HDRVE_PTH0_MASK;

  SIM->SCGC |= SIM_SCGC_ETM2_MASK;             //ʹ��ETM2ʱ��
  ETM2->COMBINE &= ~ETM_COMBINE_COMBINE2_MASK; //ͨ��0��ͨ��1����
  ETM2->SC |= ETM_SC_CPWMS_MASK;               //ѡ����������ļ�����ʽ
  ETM2->COMBINE |= ETM_COMBINE_COMP2_MASK;     //ͨ��0��ͨ��1���������
  ETM2->CONTROLS[4].CnSC = ETM_CnSC_ELSA_MASK; //��������
  ETM2->CONTROLS[5].CnSC = ETM_CnSC_ELSA_MASK;

  ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2ʱ������

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
