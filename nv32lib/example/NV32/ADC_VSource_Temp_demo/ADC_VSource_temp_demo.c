/**********************************************************************
 *
 * ʵ�����ƣ��ڲ��¶ȴ�����ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ADC�ɼ��ڲ��¶ȴ�������Ӧ�ĵ�ѹֵ������ת������������Ȼ��������ǰ�¶�
 *           ֵͨ�����ڴ�ӡ����  
 * 
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "adc.h"
#include "sysinit.h"
#include "pmc.h"

#define VDD  3.3 //����R4,R5�м�ĵ�ѹֵ

int main (void);

/**********************************************************************/
int main (void)
{
		float Vtemp,m,temp,VBG;
		int32_t advrefh,adtemp,advcore;
    ADC_ConfigType  sADC_Config = {0};
	  PMC_ConfigType  PMC_Config={0};
    
    sysinit();

		PMC_Config.sCtrlstatus.bits.bBandgapEn = 1; //ʹ�ܴ�϶
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; 
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;
    PMC_Init(PMC, &PMC_Config);  //��ʼ��PMCģ��
		
    UART_WaitTxComplete(TERM_PORT);   
    
    /*��ʼ��ADCģ��*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;     /*!< ADCʱ��Դ��Ƶϵ��Ϊ8*/
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2; /*!< ADCʱ��Դѡ������ʱ��2��Ƶ*/
    sADC_Config.u8Mode = ADC_MODE_10BIT;  		/*!< 10λת��*/
		sADC_Config.sSetting.bLongSampleEn=1;    /*!<������ģʽ,*/
    
    ADC_Init( ADC, &sADC_Config);   /*!<��ʼ��ADCģ��*/
  
	while(1)
	{
		advrefh=ADC_PollRead(ADC,ADC_CHANNEL_AD29_VREFH);     /*ADC��ȡVrefh��ֵ*/
		adtemp=ADC_PollRead(ADC,ADC_CHANNEL_AD22_TEMPSENSOR); /*!< ADC��ȡ�´�������ֵ*/
		advcore= ADC_PollRead(ADC,ADC_CHANNEL_AD23_BANDGAP);  /*!< ADC��ȡ��϶ֵ*/
		VBG = VDD*advcore/advrefh;  /*<�����϶��ѹֵ*/
			
		Vtemp = VBG*adtemp/advcore;  /*<���㵱ǰ�¶����¶ȴ�������Ӧ�ĵ�ѹֵ*/
		printf("VBG value:%f\r\n",VBG);	
		printf("Temperature sensor  conversion value:%f\r\n",Vtemp);
   /**********************************************************************************************
		ע�����¶�Ϊ25 ��ʱ�¶ȴ�������Ӧ�ĵ�ѹֵ��Vtemp25=1.419V����Vtemp>Vtemp25 ʱ��ʾ��ǰ�¶ȵ�
            ��25 �ȣ���Vtemp<Vtemp25 ʱ��ʾ��ǰ�¶ȸ���25 ��
            M���¶���Ե�ѹ�仯�ʣ����¶ȴ���25 ��ʱ,M=-3.65mv/��,���¶�С��25 ��ʱM=-3.4mv/��
    **********************************************************************************************/
		if(Vtemp >1.419) m=-3.4;  
	  else m=-3.65;
		temp = 25 - (1.419-Vtemp)*1000/m;  /*<���㵱ǰ�¶�*/
	  printf("Temperature  value:%f\r\n",temp);
    UART_GetChar(TERM_PORT);
        
	} 

}

/********************************************************************/
