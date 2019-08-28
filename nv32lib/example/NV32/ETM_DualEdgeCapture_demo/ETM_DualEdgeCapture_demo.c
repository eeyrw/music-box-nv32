/**********************************************************************
 *
 * ʵ�����ƣ�ETM˫���ز�׽
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч�������������岶���� ETM0��ͨ��1���������ź�,����Ϊ10000,
 *           ������Ϊ5000,ʹ��ETM2��ͨ��0��Ϊ����ڣ���ETM2 Ch0��PC0��
 *           ��ETM0 Ch0��PB2���̽ӡ�     
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "ETM.h"
#include "uart.h"
#include "sysinit.h"


void ETM2_Task(void);

volatile uint16_t u16Ch0Value,u16Ch1Value;
volatile uint8_t u8IntMark;
uint16_t x,y;

/********************************************************************/
int main (void)
{
    /* ��ʼ�� */
    sysinit();

    /* ETM0ͨ��0����PB2�ţ����������PB2��, ����10000��������Ϊ5000 */ 
    SIM_RemapETM0CH0Pin();
    ETM_OutputCompareInit(ETM0, ETM_CHANNEL_CHANNEL0, ETM_OUTPUT_TOGGLE);//Modify
    /* װ��ETM0��MOD����ֵ */
    ETM_SetModValue(ETM0, 5000);
    /* ����ʱ��*/ 
    ETM_ClockSet(ETM0, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
   
    /* ����ETM2��ͨ�������������Ŀ�Ȼ�����*/
    ETM_DualEdgeCaptureInit(  ETM2,    
                              ETM_CHANNELPAIR0,                      /* ͨ����0:ͨ��0��ͨ��1 */
                              ETM_INPUTCAPTURE_DUALEDGE_ONESHOT,     /* ����ģʽ */
                              ETM_INPUTCAPTURE_DUALEDGE_RISINGEDGE,  /* ͨ��0��������� */
                              ETM_INPUTCAPTURE_DUALEDGE_FALLInGEDGE  /* ͨ��1����½��� */
                            );
    /*ETM2��ʱ������,Ϊϵͳʱ�ӣ�1��Ƶ*/
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV1);
    
    ETM_SetCallback(ETM2, ETM2_Task);
    NVIC_EnableIRQ(ETM2_IRQn);//ʹ��IRQ�ж�
    ETM_EnableChannelInt(ETM2, (ETM_CHANNELPAIR0+1));//ʹ��ͨ���ж�
    
    
	while(1)
	{
        if(u8IntMark) 
        {
            u16Ch0Value = ETM2->CONTROLS[0].CnV;
            u16Ch1Value = ETM2->CONTROLS[1].CnV;
            u8IntMark   = 0;//��ת��־λ
            x = u16Ch0Value;
            y = u16Ch1Value;
            printf("\n Dual edge capture end. The input wave period is %d\n",(uint16_t)(y-x));
            
             /* ���¿��������ز�׽ */
            ETM2->COMBINE |=  (ETM_COMBINE_DECAP0_MASK << (ETM_CHANNELPAIR0 * 4)); 
        }
	} 
}

/*****************************************************************************//*!
*
* @brief ETM2�жϻص�������
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void ETM2_Task(void)
{
    ETM_ClrChannelFlag(ETM2, ETM_CHANNELPAIR0);
    ETM_ClrChannelFlag(ETM2, ETM_CHANNELPAIR0+1);
    u8IntMark = 1;//��λ��־λ
}
/********************************************************************/
