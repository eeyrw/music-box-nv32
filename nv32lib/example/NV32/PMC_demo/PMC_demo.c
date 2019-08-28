/**********************************************************************
 *
 * ʵ�����ƣ�ֹͣģʽ��RTC�жϻ���MCU
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����MCU����STOP3ģʽ�󣬾���10��ʱ��RTC�����жϽ�MCU��STOP3ģʽ��
 *          ����  
 * 
************************************************************************/
#include "common.h"
#include "rtc.h"
#include "pmc.h"
#include "uart.h"
#include "sysinit.h"
#include "sim.h"

int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
	  uint8_t u8Ch; 
    PMC_ConfigType  PMC_Config={0};
    RTC_ConfigType  RTC_Config={0};

    /*ϵͳ��ʼ��*/
	  sysinit();

  	printf("\nRunning the PMC_demo project.\r\n");
    
    LED0_Init();
		LED0_Off();
  
    
    PMC_Config.sCtrlstatus.bits.bBandgapEn = 0;
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; //��ѹ�����ֹͣģʽ�½���
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;  //���õ�ѹ��⸴λ
    PMC_Init(PMC, &PMC_Config);   //��ʼ��PMCģ��
    PMC_DisableLVWInterrupt(PMC); //���õ�ѹ�����ж�
    u8Ch = PMC_GetLVWFlag(PMC);
    
    RTC_Config.u16ModuloValue = 0x09;  //����RTCģֵ
    RTC_Config.bClockSource   = RTC_CLKSRC_1KHZ; //ѡ��1KHZʱ��Դ
    RTC_Config.bClockPresaler = RTC_CLK_PRESCALER_1000; //����ʱ�ӷ�Ƶϵ��
    RTC_SetCallback(RTC_Task);  //���ûص�����
    RTC_Config.bInterruptEn   = 1; //ʹ��RTC�ж�
    RTC_Init(&RTC_Config);  //��ʼ��RTCģ��
		
   /*��ִ��STOPָ��ǰ���RTC��ʼ��*/
	 /*MCU����ֹͣģʽ������10��ʱ����RTC�жϻ���*/
	  
    printf("\nEnter stop mode and will be woken up in about 10s by RTC Irq.\r\n");
    PMC_SetMode(PMC,PmcModeStop3); 
    while(!(ICS->S & ICS_S_LOCK_MASK));//��ֹͣģʽ���Ѻ���Ҫ�ȴ�FLL�ȶ�
		printf("\nWake up now.\r\n");
	  while(1);
 
}

/*****************************************************************************//*!
*
* @brief RTC�ص����� 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
void RTC_Task(void)
{
    LED0_Toggle();
}
/********************************************************************/
