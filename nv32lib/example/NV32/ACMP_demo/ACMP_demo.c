/**********************************************************************
 *
 * ʵ�����ƣ�ģ��Ƚ�ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ѡ��ACMP��������,��ʼ��ACMPģ��,�������������������ݿ�ʼ
 *          �Ƚ�,�Ƚ���ɲ����ж�,���жϷ�����������жϱ�־λ 
 * 
************************************************************************/

#include "common.h"
#include "acmp.h"
#include "pmc.h"
#include "sysinit.h"

int main (void);
void FunForCallback(void);

/********************************************************************/
int main (void)
{
    ACMP_ConfigType ACMP_Config;
    PMC_ConfigType  PMC_Config={0};
		
    /*ϵͳ��ʼ��*/
    sysinit();    
    PMC_Config.sCtrlstatus.bits.bBandgapEn = 1; //ʹ�ܴ�϶
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0; //��ѹ�����ֹͣģʽ�½���
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;  //���õ�ѹ��⸴λ
    PMC_Init(PMC, &PMC_Config);  								//��ʼ��PMCģ��
    PMC_DisableLVWInterrupt(PMC); 							//���õ�ѹ�����ж�

    printf("\nRunning the ACMP_demo project.\r\n");

		
    /*��ʼ��ACMPģ�� */
    ACMP_Config.sCtrlStatus.bits.bIntEn = TRUE; /*ʹ���ж�*/
		ACMP_Config.sCtrlStatus.bits.bMod= 1; 			/*�ж�����������ش���*/
    ACMP_Config.sCtrlStatus.bits.bOutEn = 1;    /* ACMP��������ⲿ����  */
    ACMP_Config.sPinSelect.bits.bNegPin = 0x3;  /* ����������:DAC */
    ACMP_Config.sPinSelect.bits.bPosPin = 0;    /* ����������:�ⲿ��׼0��ACMP0_IN0-PTA0 */
    ACMP_Config.sDacSet.bits.bEn        = TRUE; /* ʹ�� DAC */    
    ACMP_Config.sDacSet.bits.bRef       = DAC_REF_VDDA;    /* DAC��׼��ѹѡ��:Vdda */
    ACMP_Config.sDacSet.bits.bVal       = 0x1F; 					 /* DAC�����ƽλ��Vin/2 */
    ACMP_Config.sPinEnable.bits.bEn     = TRUE; /* ʹ���ⲿ��׼0�������� */
    
    ACMP_SetCallback(ACMP0,FunForCallback);     /*ע��ص�����*/
    
    ACMP_Init(ACMP0, &ACMP_Config);             /*��ʼ��ACMPģ��*/
    
    ACMP_Enable(ACMP0);                         /*ʹ��ACMP */
  
  /************************************************************
		printf("\nEnter stop mode and will be woken up  by ACMP Irq.\r\n");
    PMC_SetMode(PMC,PmcModeStop3);  //MCU����ֹͣģʽ��ͨ��ACMP�жϻ���
    printf("\nWake up now.\r\n");
	 *******************************************************************/
	 while(1);

}

/*****************************************************************************//*!
*
* @brief ACMP�ص�����.
*        
* @param none.
*
* @return none.
*
*****************************************************************************/
void FunForCallback(void)
{
    if(ACMP_GetFlag(ACMP0))
    {    
        ACMP_ClrFlag(ACMP0);  //����жϱ�־λ
    }
    
    printf("\nCallback happens.\r\n");
}
