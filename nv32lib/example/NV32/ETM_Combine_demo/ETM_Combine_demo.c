/**********************************************************************
 *
 * ʵ�����ƣ�ͨ������PWM���
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ETM2��ͨ��0��ͨ��1��ϲ���һ��PWM����,ͨ��0�ܽ�ΪPC0��ͨ��1�ܽ�ΪPC1      
 *
************************************************************************/
#include "common.h"
#include "rtc.h"
#include "ETM.h"
#include "uart.h"
#include "sysinit.h"

void ETM2_Task(void);

/********************************************************************/
int main (void)
{
    /* ϵͳ��ʼ��*/
    sysinit();

  	printf("\nRunning the ETM_demo project.\r\n");

    LED0_Init();     
    /* ����ETM2Ϊ���ģʽ */
    ETM_PWMInit(ETM2, ETM_PWMMODE_COMBINE, ETM_PWM_LOWTRUEPULSE);
    /* ��ETM2��MODֵ */
    ETM_SetModValue(ETM2, 3999); //Ƶ��Ϊ10KHz
    /* ʱ�Ӽ���Ƶϵ�������� */
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
    
    ETM_SetCallback(ETM2, ETM2_Task);//����ETM2�ص������� 
    NVIC_EnableIRQ(ETM2_IRQn);   //ʹ��IRQ�ж�
    ETM_EnableOverflowInt(ETM2); //��������ж�
    
    /* ����ռ�ձȣ�����ͨ����ռ�ձ�Ϊ50% */
    ETM_SetDutyCycleCombine(ETM2, ETM_CHANNEL_CHANNEL1, 50);
    
		while(1);
}

/*****************************************************************************//*!
*
* @brief ETM�жϻص�������. 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
void ETM2_Task(void)
{
    static uint32_t count;
    
    ETM_ClrOverFlowFlag(ETM2); //��������־λ
    
    if(count == 2000)
    {
        count = 0;
        UART_PutChar(TERM_PORT,'@');//�����ַ�@������1
    }
    else 
    {
        count++;
    }
}
/********************************************************************/

