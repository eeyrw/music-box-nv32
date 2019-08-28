/**********************************************************************
 *
 * ʵ�����ƣ�MCUʱ��ģʽ�л�ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����MCU��ʱ��ģʽ�ɵ�ǰ��FEEģʽ�л���FEIģʽ 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"


int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    ICS_ConfigType  sICSConfig;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
    UART_ConfigType sConfig;

    /*ϵͳ��ʼ��*/
    sysinit();
    
    printf("\nRunning the platinum project.\r\n");
    LED0_Init();  
    LED2_Init();

    /*��ʼ��RTCģ�飬ÿ��1s����һ���ж�*/
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /*ʹ���ж� */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*ʱ��Դ1KHz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*ʱ�ӷ�Ƶϵ��100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

    printf("\nIt is in FEE mode now.\r\n");
    UART_WaitTxComplete(TERM_PORT);   

    /*��ʱ��ģʽ��FEIģʽ�л���FEEģʽ*/ 
		sICSConfig.oscConfig.bGain = 1;			 /* ʹ�ܸ����� */
		sICSConfig.oscConfig.bRange = 1;     /* ʹ�ܸ߷�Χ */
    sICSConfig.oscConfig.bEnable = 1;    /* ʹ�� OSC  */
		sICSConfig.oscConfig.bIsCryst = 1;   /* OSC�����ѡ��ѡ������ʱ��Դ */ 
    sICSConfig.oscConfig.bWaitInit = 1;  /* �ȴ�������ʼ������� */       
    sICSConfig.u32ClkFreq = 10000;       /* ����Ϊ10M����*/
    ICS_SwitchMode(FEI,FEE, &sICSConfig);
    ICS_SetBusDivider(1); 							 //����ʱ�Ӳ��ø���40M��Ƶ������10M����256��Ƶ��1280��Ƶ��Ϊ50M����Ҫ����BDIV����ƵΪ25M
    /* ����ʱ��ģʽת�䣬����ʱ�ӷ����仯���³�ʼ��UARTģ��*/
    sConfig.u32SysClkHz = 25000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEE mode.\r\n");

    while(1);
}

/*****************************************************************************//*!
*
* @brief RTCģ��ص�����
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void RTC_Task(void)
{
    /*�л�LED0״̬ */
    LED0_Toggle();
}
/********************************************************************/
