/**********************************************************************
 *
 * ʵ�����ƣ������ж��շ�����(�����жϵķ�ʽ�շ�����)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч�����ṩ�����ж��շ����ݵĿ��
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "UART_app.h"
#include "sysinit.h"


#define SEND_BUF_LEN    50

uint8_t send_buf[SEND_BUF_LEN]; //���ͻ�����
volatile uint8_t u8IsSendDone; //���巢����ɱ�־λ

/******************************************************************************
******************************************************************************/

void printf_int(int8* str)
{
	uint32 len = 0;
    
	u8IsSendDone = 0;                   
    
	while(*str)
	{
		send_buf[len++] = *str;
		str++;
		if (len >= (SEND_BUF_LEN-1))
		{
			send_buf[SEND_BUF_LEN-1] = 0;
			break;
		}
	}


    UART_SendInt(UART1, send_buf, len);//UART1�ڴ��ڷ��ͳ�ʼ��

}
/*
�������ݷ�����ɺ���*/
void UART_SendDone(void)//�������ʱ������־λ��λ
{
    u8IsSendDone = 1;
}

int main (void)
{
    UART_ConfigType sConfig;
    
    /*ִ��ϵͳ��ʼ��*/
    sysinit();
    
    u8IsSendDone = 1;

    sConfig.u32SysClkHz = BUS_CLK_HZ;//����ϵͳʱ�ӺͲ�����
    sConfig.u32Baudrate = UART_PRINT_BITRATE;
    
    UART_Init(UART1,&sConfig);  //��ʼ������1
    UART_SetTxDoneCallback(UART1, UART_SendDone);//�����жϻص�����
    UART_SetCallback(UART_HandleInt);

    LED0_Init();//��ʼ��LED��
    printf("\nRunning the UART_Interrupt_demo project.\r\n");
  	
  	/* �򿪴���1�ж� */       
    NVIC_EnableIRQ(UART1_IRQn); 

    printf_int("\nPrint characters using interrupt mode.\r\n");
    while (!u8IsSendDone);         /* �ȴ�������� */
    
    printf_int("\nrepeat Print characters using interrupt mode.\r\n");
    while (!u8IsSendDone);         /* �ȴ�������� */
    
    while (1);
}

