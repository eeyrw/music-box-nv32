/**********************************************************************
 *
 * ʵ�����ƣ�PIT��ʱ�ж�
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��˵����PIT�������ö�ʱ��1��������ʱ��0����ÿ��1�ε��ж�.
 *				  ������ʱ���ڱ�������Ϊ40MHZ������˵��ʱ������Ϊ25ns ,����һ����ж���Ҫ40,000,000��ʱ�����ڣ��ж���������LED�Ʒ�ת. 
 * 
 *					�趨PIT0����1,000,000������, PIT1��ʱ��1����40�Σ�������Ϊ40,000,000��ʱ������ 
 *
************************************************************************/

#include "common.h"
#include "uart.h"
#include "pit.h"
#include "sysinit.h"


int main (void);
void PIT_Task(void);
/*****************************************************************************
*****************************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    uint32_t        u32LoadValue0, u32LoadValue1;     
    PIT_ConfigType  sPITConfig0, sPITConfig1;    
    PIT_ConfigType  *pPIT_Config1   =&sPITConfig1;
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 

    /* ϵͳ��ʼ�� */
    sysinit();
    
    printf("\nRunning the PIT_demo project.\r\n");
    
    LED0_Init();

    /* PITʱ��ԴΪ����ʱ�� */
    /* ͨ��0װ��ֵΪ = (1000000-1),ͨ��1װ��ֵΪ = (40-1) */
    u32LoadValue0   = 0xF423F;                  /* ͨ��0װ��ֵ */ 
    u32LoadValue1   = 0x13;                     /* ͨ��1װ��ֵ */      
    
    /* ����ͨ��1Ϊ����ģʽ,�����жϲ���ʹ�� */
    pPIT_Config1->u32LoadValue      = u32LoadValue1;
    pPIT_Config1->bFreeze           = FALSE;    //��ʱ���ڵ���ģʽ�¼�������
    pPIT_Config1->bModuleDis        = FALSE;    //ʹ�ܶ�ʱ��ģ��
    pPIT_Config1->bInterruptEn      = TRUE;     //������Ӧͨ����IRQ�ж�
    pPIT_Config1->bChainMode        = TRUE;     //��ʱ�����ӵ�PIT0
    pPIT_Config1->bETMerEn          = TRUE;     //��ʱ��ʹ��
    
    /* ����ͨ��0, ����ʹ�� */   
    pPIT_Config0->u32LoadValue      = u32LoadValue0;
    pPIT_Config0->bFreeze           = FALSE;    //��ʱ���ڵ���ģʽ�¼�������
    pPIT_Config0->bModuleDis        = FALSE;    //ʹ�ܶ�ʱ��ģ��      
    pPIT_Config0->bInterruptEn      = FALSE;    
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bETMerEn          = TRUE;     //��ʱ��ʹ��

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);  //��ʼ��PITģ��ͨ��0               
    PIT_Init(PIT_CHANNEL1, pPIT_Config1);  //��ʼ��PITģ��ͨ��1
      
    PIT_SetCallback(PIT_CHANNEL1, PIT_Task);  //����ͨ��1�жϻص�����   
    
		while(1)     
		{
		} 

}

/*****************************************************************************//*!
*
* @PIT�ж�������
*        
* @������
*
* @�޷���
*
*****************************************************************************/

void PIT_Task(void)
{
    LED0_Toggle();          /*!< ��˸  */
}
/********************************************************************/
