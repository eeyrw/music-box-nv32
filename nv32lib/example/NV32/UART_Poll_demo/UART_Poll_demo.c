/**********************************************************************
 *
 * 实验名称：UART查询法样例
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：使用查询法发送字符串    
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
    
    /* 执行系统初始化 */
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;//选择时钟源为总线时钟
    sConfig.u32Baudrate = UART_PRINT_BITRATE;//设置波特率
    
    UART_Init(UART1,&sConfig);//初始化串口 1
	
    LED0_Init();
    printf("\nRunning the UART_Poll_demo project.\r\n");//打印运行工程名
  	      
    UART_SendWait(UART1, send_buf, 50);//发送缓冲区的数据放入 UART1 口的数据寄存器中
    
    while (1);
}

