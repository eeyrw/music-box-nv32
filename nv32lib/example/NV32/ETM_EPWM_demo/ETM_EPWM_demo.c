/**********************************************************************
 *
 * ʵ�����ƣ�ETM����PWM���--������
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ETM2ͨ��1������ض����PWM����������NV32F100���ϵ�D2��˸ 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"

void ETM2_Task(void);

/********************************************************************/
int main (void)
{     
    /* ��ʼ�� */
    sysinit();
    
    SIM_RemapETM2CH1Pin();    //ѡ��ӳ��ETM2CH1��PH1���ţ�Ĭ��ETM2CH1��ӦPC1
    
    ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE); 
	  ETM_disblechannel(ETM2,0);//ETM_PWMInit����Ĭ�ϴ�����ͨ�����ܣ������������������/�����������ܣ���Ҫ���á�
	  ETM_disblechannel(ETM2,2);
    ETM_disblechannel(ETM2,3);
	  ETM_disblechannel(ETM2,4);
   	ETM_disblechannel(ETM2,5);
    /* ����MOD��ֵ */
    ETM_SetModValue(ETM2, 9999);
    /* ����ʱ��Դ��ʹ�ܼ����� */
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);  
    /* ʹ��ETM2��IRQ�ж� */
    NVIC_EnableIRQ(ETM2_IRQn);
    /* �����жϻص����� */
    ETM_SetCallback(ETM2, ETM2_Task);
    /* ʹ��ETM2����ж� */
    ETM_EnableOverflowInt(ETM2);

		while(1);
}

/*****************************************************************************//*!
*
* @brief ETM2�жϻص�������
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/
uint16_t u16ChV_old, u16ChV_new;
void ETM2_Task(void)
{
    static uint16_t u16count;
    static uint8_t u8DirMark;
 
   
    ETM_ClrOverFlowFlag(ETM2);//��������־λ
          
    if(100 == u16count)
    {
        u16count = 0;
        u16ChV_old = ETM2->CONTROLS[1].CnV;
        if(!u8DirMark)
        {
            u16ChV_new = u16ChV_old + 200;
            if(u16ChV_new >= ETM2->MOD)
            {
                u16ChV_new = ETM2->MOD - 200; 
                u8DirMark  = 1;
            }
            else
            {
            }
        }
        else
        {
            u16ChV_new = u16ChV_old - 200;
            if(u16ChV_new <  200)
            {
                u16ChV_new = 200; 
                u8DirMark  = 0;
            }
            else
            {  
            }
        }
        /* ˢ��ͨ��ֵ */
        ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, u16ChV_new);
    }
    else
    {
        u16count++;
    }
}
/********************************************************************/
