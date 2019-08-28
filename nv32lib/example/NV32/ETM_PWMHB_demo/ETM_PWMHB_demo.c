/**********************************************************************
 *
 * ʵ�����ƣ�PWM�������
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ѡ��FEI�ڲ�ʱ�ӣ�����ʱ��40Mhz������ETM2��ͨ��0(PH0)��ͨ��1(PH1)�������
 *
************************************************************************/
#include "common.h"
#include "ics.h"
#include "etm.h"
#include "uart.h"
#include "sysinit.h"


int main (void)
{     

    sysinit();//ϵͳ��ʼ��
 
    SIM_RemapETM2CH0Pin();//ӳ���Ӧͨ���ܽŵ�PH0
    SIM_RemapETM2CH1Pin();//ӳ���Ӧͨ���ܽŵ�PH1
	
    SIM->SCGC  |= SIM_SCGC_ETM2_MASK;							//ʹ��ETM2ʱ��
    ETM2->COMBINE &= ~ ETM_COMBINE_COMBINE0_MASK; //ͨ��0��ͨ��1����
	  ETM2->SC |= ETM_SC_CPWMS_MASK;								//ѡ����������ļ�����ʽ
	  ETM2->COMBINE |= ETM_COMBINE_COMP0_MASK; 			//ͨ��0��ͨ��1���������
	  ETM2->CONTROLS[0].CnSC = ETM_CnSC_ELSA_MASK;  //��������
    ETM2->CONTROLS[1].CnSC = ETM_CnSC_ELSA_MASK; 	
	 
   
	  ETM_SetModValue(ETM2, 3999);//����Ƶ��10KHz
	
    ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL0, 2000);//����ռ�ձ�Ϊ50%
		ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 2000);	

		ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1); //ETM2ʱ������


	  while(1);

}

