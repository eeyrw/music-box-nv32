/**********************************************************************
 *
 * ʵ�����ƣ�UART��ѯ������
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ʹ�ò�ѯ�������ַ���    
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"

#define SEND_BUF_LEN    50

uint8_t send_buf[SEND_BUF_LEN] = "\nUART send char by polling\n\r";


/******************************************************************************/
int main (void)
{
    UART_ConfigType sConfig;
    
    /* ִ��ϵͳ��ʼ�� */
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;//ѡ��ʱ��ԴΪ����ʱ��
    sConfig.u32Baudrate = UART_PRINT_BITRATE;//���ò�����
    
    UART_Init(UART1,&sConfig);//��ʼ������ 1
	
    LED0_Init();
    printf("\nRunning the UART_Poll_demo project.\r\n");//��ӡ���й�����
  	      
    UART_SendWait(UART1, send_buf, 50);//���ͻ����������ݷ��� UART1 �ڵ����ݼĴ�����
    
    while (1);
}

