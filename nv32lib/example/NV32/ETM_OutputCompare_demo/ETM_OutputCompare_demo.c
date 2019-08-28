/**********************************************************************
 *
 * ʵ�����ƣ�ETM����Ƚ�
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ETM0��ͨ��1�����������Σ���ͨ������Ӧ�Ĺܽ�ΪPB3         
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"

/********************************************************************/
int main (void)
{
    /* ϵͳ��ʼ�� */
    sysinit();

    SIM_RemapETM0CH1Pin();//ӳ��ͨ�����ܽ�PB3
     /* ETM0Ϊ����Ƚ�ģʽ, ͨ��1������������ */
    ETM_OutputCompareInit(ETM0, ETM_CHANNEL_CHANNEL1, ETM_OUTPUT_TOGGLE);
    //���ض�Ӧ��MOD��ֵ
    ETM_SetModValue(ETM0, 5000);
    //����CnV��ֵ
    ETM_SetChannelValue(ETM0, ETM_CHANNEL_CHANNEL1, 2000);
    //ʱ������/
    ETM_ClockSet(ETM0, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
       
    
		while(1);
}
