/**********************************************************************
 *
 * 实验名称：I2C从机通讯
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：本例为I2C通讯中从机的设定
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "i2c.h"
#include "i2C_app.h"
#include "sysinit.h"

#define I2C_SLAVE_ADDRESS       0x50<<1

uint8_t u8I2C_SendBuff[64];
uint8_t u8I2C_ReceiveBuff[64];

int main (void);


/********************************************************************/
int main (void)
{
    uint8_t         u8I2C_ReceiveLength;
    uint32_t        i;

    I2C_ConfigType  sI2C_Config = {0};
     
    sysinit();
  	printf("\nRunning the I2C_Slave_demo project.\r\n");
    LED0_Init();  
    LED2_Init();
    
    UART_WaitTxComplete(TERM_PORT);//等待串口发送完成   
    
    /* 初始化I2C接收数据大小以及中断回调入口 */
    I2C_InitGlobalVariable(  );
 
    
    sI2C_Config.u16Slt = 0;//设置低超时scl低电平保持时间为 0
    sI2C_Config.u16F = 0x1F;//设置分频系数
    sI2C_Config.u16OwnA1 = I2C_SLAVE_ADDRESS;//设置I2C从机地址
    sI2C_Config.sSetting.bIntEn = 1;//打开中断使能
    sI2C_Config.sSetting.bI2CEn = 1;//打开I2C使能

    I2C_Init(I2C0,&sI2C_Config);//初始化I2C

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;//给发送数据的数组里存放数据
    }
    u8I2C_SendBuff[0] = 0xa0;
    I2C_SlaveSend(u8I2C_SendBuff,64);
    
	while(1)
	{
        u8I2C_ReceiveLength = I2C_SlaveReceive(&u8I2C_ReceiveBuff[0]);
        if( u8I2C_ReceiveLength < 64 )
        {
            /* 发送旧数据给主机 */
            I2C_SlaveSend(&u8I2C_SendBuff[0],64);
        }
        else
        {
            /* 发送从主机接收到的新数据 */
            I2C_SlaveSend(&u8I2C_ReceiveBuff[0],64);
        }
        if( u8I2C_ReceiveLength )
        {
            printf("I2C received data:\r\n");
            for(i=0;i<u8I2C_ReceiveLength;i++)
            {
            		if( (i%8) == 0 )
                {
                   printf("\r\n"); 
                }
                printf("0x%x,", u8I2C_ReceiveBuff[i]);
            }
            printf("\r\n");
        }
      
        for(i=0;i<0xfffff;i++);
	} 
}

