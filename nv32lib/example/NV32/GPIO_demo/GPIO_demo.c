/**********************************************************************
 *
 * ʵ�����ƣ�GPIO�˿��������
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч��������GPIO�˿�Ϊ�������,����D4��1S��˸һ��
 * 
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "sysinit.h"

int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
    /*ϵͳ��ʼ��*/
    sysinit();

    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;  

    printf("\nRunning the GPIO_demo project.\n");

    /* ����RTCģ��ÿ��1s����һ���ж� */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* ʹ���ж�*/
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /* ѡ��1KHzʱ��Դ */
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /* ʱ�ӷ�Ƶϵ��100 */
    
    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

		/* ����1.��ʼ��PE7Ϊ�������--ͨ��32λ��������ȷ��Ҫ��ʼ�������� */
     GPIO_Init(GPIOB, GPIO_PTE7_MASK, GPIO_PinOutput);
    /* ����2.��ʼ��PE7Ϊ�������--ͨ�������GPIO������ȷ��Ҫ��ʼ��������*/
    //GPIO_PinInit(GPIO_PTE7, GPIO_PinOutput); 

    while (1);
}

/*****************************************************************************//*!
*
* @brief RTC�жϻص�����
*        
*****************************************************************************/

void RTC_Task(void)
{
    /* �л�PE7���������LED1 */
    /* ����1.�л�PE7�˿����������������ͨ��32λ��������ȷ��Ҫ�л���������� */
      GPIO_Toggle(GPIOB, GPIO_PTE7_MASK);
    
    /* ����2.�л�PE7�˿��������----ͨ�������GPIO������ȷ��Ҫ�л���������� */
    // GPIO_PinToggle(GPIO_PTE7); 
}
/********************************************************************/
