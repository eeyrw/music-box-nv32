/**********************************************************************
 *
 * ʵ�����ƣ�KBI�����ж�ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч������ʼ��KBIģ�飬ѡ���ж��������ţ��ͼ��ģʽ�����°�������KBI
 *           �ж��л�LED״̬
 *					 
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "kbi.h"
#include "sysinit.h"

int main (void);
void KBI0_Task(void);
void KBI1_Task(void);

/********************************************************************/
int main (void)
{
    uint8_t         i;
    KBI_ConfigType  sKBIConfig;
  
    /*ϵͳ��ʼ��*/
	  sysinit();
  	printf("\nRunning the KBI_demo project.\r\n");
    LED1_Init(); 
    LED2_Init();

	  PORT->IOFLT = 0xa4be0000; //KBI0��KBI1�����˲�,KBI0��KBIС��32MS��ë�̶��ᱻ�˳�   //Modify
	
    UART_WaitTxComplete(TERM_PORT);   
    
     /* ����KBI�ж���������*/
    for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
    {
     sKBIConfig.sPin[i].bEn	 = 0;
    }
    
    sKBIConfig.sBits.bMode   = 0;   /*!< ѡ��  ���ؼ��  */    
    sKBIConfig.sPin[0].bEdge = KBI_FALLING_EDGE_LOW_LEVEL; /*!< ѡ�� �½��ػ�͵�ƽ��� */
    sKBIConfig.sBits.bIntEn  = 1; //ʹ��KBI�ж�
    sKBIConfig.sPin[0].bEn   = 1; //ʹ��KBI�ж���������
    
    KBI_Init(KBI0, &sKBIConfig); //��ʼ��KBI0
    KBI_Init(KBI1, &sKBIConfig); //��ʼ��KBI1
    KBI_SetCallback(KBI0, &KBI0_Task); //���ûص�����
    KBI_SetCallback(KBI1, &KBI1_Task);
    
   
		while(1);

}

/*****************************************************************************//*!
*
* @brief KBI0�ص�����
*        
* @param  none.
*
* @return none
*
*****************************************************************************/

void KBI0_Task(void)
{
    LED1_Toggle();
    
    printf("KBI0 routinue.\r\n");
}

/*****************************************************************************//*!
*
* @brief KBI1�ص�����. 
*        
* @param  none.
*
* @return none.
*
*****************************************************************************/

void KBI1_Task(void)
{
    LED2_Toggle();
    
    printf("KBI1 routinue.\r\n");
}
/********************************************************************/
