/**********************************************************************
 *
 * 实验名称：串口中断收发数据(采用中断的方式收发数据)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：提供串口中断收发数据的框架
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "UART_app.h"
#include "sysinit.h"


#define SEND_BUF_LEN    50

uint8_t send_buf[SEND_BUF_LEN]; //发送缓冲区
volatile uint8_t u8IsSendDone; //定义发送完成标志位

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


    UART_SendInt(UART1, send_buf, len);//UART1口串口发送初始化

}
/*
串口数据发送完成函数*/
void UART_SendDone(void)//发送完成时，将标志位置位
{
    u8IsSendDone = 1;
}

int main (void)
{
    UART_ConfigType sConfig;
    
    /*执行系统初始化*/
    sysinit();
    
    u8IsSendDone = 1;

    sConfig.u32SysClkHz = BUS_CLK_HZ;//配置系统时钟和波特率
    sConfig.u32Baudrate = UART_PRINT_BITRATE;
    
    UART_Init(UART1,&sConfig);  //初始化串口1
    UART_SetTxDoneCallback(UART1, UART_SendDone);//串口中断回调函数
    UART_SetCallback(UART_HandleInt);

    LED0_Init();//初始化LED灯
    printf("\nRunning the UART_Interrupt_demo project.\r\n");
  	
  	/* 打开串口1中断 */       
    NVIC_EnableIRQ(UART1_IRQn); 

    printf_int("\nPrint characters using interrupt mode.\r\n");
    while (!u8IsSendDone);         /* 等待发送完成 */
    
    printf_int("\nrepeat Print characters using interrupt mode.\r\n");
    while (!u8IsSendDone);         /* 等待发送完成 */
    
    while (1);
}

