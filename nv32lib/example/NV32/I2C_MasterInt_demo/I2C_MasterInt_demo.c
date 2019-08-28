/**********************************************************************
 *
 * 实验名称：I2C主机收发数据实验(采用中断的方式收发数据)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：SPI发送的同时接收相同的数据，比较接收和发送的数据相同，判断
 *          SPI通信成功 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "i2c.h"
#include "i2C_app.h"
#include "sysinit.h"

#define I2C_READ_DATA_LENGTH    64
#define I2C_SLAVE_ADDRESS1      0x50

uint8_t u8I2C_SendBuff[64];
uint8_t u8I2C_ReceiveBuff[64];
uint32_t u32I2C_ReceiveLength;

int main (void);


/********************************************************************/
int main (void)
{
    uint8_t         u8I2C_ErrorStatus;
    I2C_ConfigType  sI2C_Config = {0};
    volatile uint32_t i;
    
    sysinit();
  	printf("\nRunning the I2C_MasterInt_demo project.\r\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    /* 初始化I2C接收数据大小以及中断回调入口 */
    I2C_InitGlobalVariable( );

    /* 对I2C结构体的各个成员参数进行配置 */
    
    sI2C_Config.u16Slt = 0;   //设置低超时scl低电平保持时间为 0
    sI2C_Config.u16F = 0x1F;  //设置分频系数
    sI2C_Config.sSetting.bIntEn = 1;//打开中断使能
    sI2C_Config.sSetting.bI2CEn = 1;//打开I2C使能

    I2C_Init(I2C0,&sI2C_Config );   //初始化I2C模块
   

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;  //给发送数据的数组里存放数据
    }

	while(1)
	{
          /*反馈接收字节的个数*/
         u32I2C_ReceiveLength = I2C_MasterCheckRead(&u8I2C_ReceiveBuff[0]);
         if( !I2C_IsBusy(I2C0) )
         {
             /* 若I2C总线不繁忙，则从从机获取数据 */
             printf("start to read data from slave!\r\n");
             I2C_MasterRead(I2C_SLAVE_ADDRESS1,I2C_READ_DATA_LENGTH);
         }
	    if( u32I2C_ReceiveLength == 0 )
	    {
            printf("don't receive any data from slave!\r\n");
	    }
	    else
	    {
            for(i=0;i<u32I2C_ReceiveLength;i++)
            {
                if( (i%8) == 0 )
                {
                    printf("\r\n");
                }
                printf("0x%x,",u8I2C_ReceiveBuff[i]);
               
             }
             if( u32I2C_ReceiveLength >= I2C_READ_DATA_LENGTH )
             {
                printf("\r\nreceived all required data!\r\n");
                printf("start to send data to slave!\r\n");
                u8I2C_SendBuff[0]++;
                for(i=1;i<64;i++)
                {
                    u8I2C_SendBuff[i] = i+u8I2C_SendBuff[0];
                }
            
                /* 等待I2C空闲空闲，并反馈失败的数目 */
                u8I2C_ErrorStatus = I2C_MasterSend(I2C_SLAVE_ADDRESS1,&u8I2C_SendBuff[0],64);
                if(  u8I2C_ErrorStatus != I2C_ERROR_NULL )
                {
                    printf("I2C transfer status:0x%x\r\n",u8I2C_ErrorStatus);
                }
             }
         }
         for(i=0;i<0xfffff;i++);
	} 

}


