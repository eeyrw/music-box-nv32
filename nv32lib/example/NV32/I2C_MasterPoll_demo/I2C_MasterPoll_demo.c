/**********************************************************************
 *
 * 实验名称：I2C主机收发数据(采用查询的方式收发数据)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：提供I2C主机查询法收发数据的框架
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "i2c.h"
#include "sysinit.h"

#define I2C_SLAVE_ADDRESS1          0x50

uint8_t u8I2C_SendBuff[64];
uint8_t u8I2C_ReceiveBuff[64];

int main (void);
void RTC_Task(void);

/********************************************************************/
int main (void)
{
    I2C_ConfigType  sI2C_Config = {0};
    volatile uint32_t i,j;
    
    sysinit();
    for(i=0;i<0xfff;i++);
    
  	printf("\nRunning the I2C_MasterPoll_demo project.\r\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    /* 初始化I2C模块为查询模式 */
    sI2C_Config.u16F = 0x1F;//设置分频系数
    sI2C_Config.sSetting.bIntEn = 0;//禁用中断
    sI2C_Config.sSetting.bI2CEn = 1;//使能I2C

    I2C_Init(I2C0,&sI2C_Config );//设置分频系数

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;//给发送数据的数组里存放数据
    }
 
	while(1)
	{ 
          
          //等待数据收发，查询法
        I2C_MasterSendWait(I2C0,I2C_SLAVE_ADDRESS1,&u8I2C_SendBuff[0],64);
        I2C_MasterReadWait(I2C0,I2C_SLAVE_ADDRESS1,&u8I2C_ReceiveBuff[0],64);
        printf("Read data from I2C slave:\r\n");
       
        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                printf("0x%x,", u8I2C_ReceiveBuff[i*8+j]);
            }

            printf("\r\n");
        }
        u8I2C_SendBuff[0]++;
        for(i=1;i<64;i++)
        {
            u8I2C_SendBuff[i] = i+u8I2C_SendBuff[0];
        }
        for(i=0;i<0xfffff;i++);
	} 

}

