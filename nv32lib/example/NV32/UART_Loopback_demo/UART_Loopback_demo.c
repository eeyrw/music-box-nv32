/**********************************************************************
 *
 * 实验名称：UART环回模式
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：本例程为回环测试,UART1口为正常模式完成收发,UART0口为循环模式      
 *           输入"loopback"在串口助手的窗口中回显
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

    /*执行系统初始化*/
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;//选择系统时钟
    sConfig.u32Baudrate = 115200;//配置波特率为115200

    LED0_Init();//初始化 LED
    printf("\nRunning the UART_Loopback_demo project.\r\n");  
    printf("\nEnter any character to echo...\r\n");
    UART_WaitTxComplete(UART1);//等待串口1发送完成
    
    UART_Init(UART0,&sConfig);//初始化串口 0
    UART_EnableLoopback(UART0);//开启串口0环回,设定UART0为循环模式
    UART_SetCallback(UART_HandleInt);	

    /* 禁用串口 1 收发中断 */
    UART_DisableInterrupt(TERM_PORT, UART_RxBuffFullInt);
    UART_DisableInterrupt(TERM_PORT, UART_TxBuffEmptyInt);
    /* 使能串口 1 接收溢出中断 */ 
    UART_EnableInterrupt(UART1, UART_RxOverrunInt);
    NVIC_EnableIRQ(UART1_IRQn);//打开串口1中断
     
    while (1)
    {
        send_buf[0] = UART_GetChar(TERM_PORT); //获取串口1上的字符,存放到发送缓冲区
        UART_SendWait(UART0, send_buf, 20);  //发送缓冲区字符到UART0口
        UART_ReceiveWait(UART0, receive_buf, 20);//把UART0的内容放入接收缓冲区中
        UART_PutChar(TERM_PORT, receive_buf[0]);//接收缓冲区的数据放入 UART1 口的数据寄存器    
    }
}

