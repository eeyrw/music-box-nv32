/**********************************************************************
 *
 * ʵ�����ƣ�UART����ģʽ
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����������Ϊ�ػ�����,UART1��Ϊ����ģʽ����շ�,UART0��Ϊѭ��ģʽ      
 *           ����"loopback"�ڴ������ֵĴ����л���
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "uart_app.h"
#include "sysinit.h"

#define SEND_BUF_LEN    1
#define RECEIVE_BUF_LEN 1

uint8_t send_buf[SEND_BUF_LEN] = {'L'};
uint8_t receive_buf[RECEIVE_BUF_LEN] = {0};

/******************************************************************************/
int main (void)
{	
    UART_ConfigType sConfig;

    /*ִ��ϵͳ��ʼ��*/
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;//ѡ��ϵͳʱ��
    sConfig.u32Baudrate = 115200;//���ò�����Ϊ115200

    LED0_Init();//��ʼ�� LED
    printf("\nRunning the UART_Loopback_demo project.\r\n");  
    printf("\nEnter any character to echo...\r\n");
    UART_WaitTxComplete(UART1);//�ȴ�����1�������
    
    UART_Init(UART0,&sConfig);//��ʼ������ 0
    UART_EnableLoopback(UART0);//��������0����,�趨UART0Ϊѭ��ģʽ
    UART_SetCallback(UART_HandleInt);	

    /* ���ô��� 1 �շ��ж� */
    UART_DisableInterrupt(TERM_PORT, UART_RxBuffFullInt);
    UART_DisableInterrupt(TERM_PORT, UART_TxBuffEmptyInt);
    /* ʹ�ܴ��� 1 ��������ж� */ 
    UART_EnableInterrupt(UART1, UART_RxOverrunInt);
    NVIC_EnableIRQ(UART1_IRQn);//�򿪴���1�ж�
     
    while (1)
    {
        send_buf[0] = UART_GetChar(TERM_PORT); //��ȡ����1�ϵ��ַ�,��ŵ����ͻ�����
        UART_SendWait(UART0, send_buf, 20);  //���ͻ������ַ���UART0��
        UART_ReceiveWait(UART0, receive_buf, 20);//��UART0�����ݷ�����ջ�������
        UART_PutChar(TERM_PORT, receive_buf[0]);//���ջ����������ݷ��� UART1 �ڵ����ݼĴ���    
    }
}

