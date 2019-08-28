/**********************************************************************
 *
 * ʵ�����ƣ�I2C�ӻ�ͨѶ
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч��������ΪI2CͨѶ�дӻ����趨
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
    
    UART_WaitTxComplete(TERM_PORT);//�ȴ����ڷ������   
    
    /* ��ʼ��I2C�������ݴ�С�Լ��жϻص���� */
    I2C_InitGlobalVariable(  );
 
    
    sI2C_Config.u16Slt = 0;//���õͳ�ʱscl�͵�ƽ����ʱ��Ϊ 0
    sI2C_Config.u16F = 0x1F;//���÷�Ƶϵ��
    sI2C_Config.u16OwnA1 = I2C_SLAVE_ADDRESS;//����I2C�ӻ���ַ
    sI2C_Config.sSetting.bIntEn = 1;//���ж�ʹ��
    sI2C_Config.sSetting.bI2CEn = 1;//��I2Cʹ��

    I2C_Init(I2C0,&sI2C_Config);//��ʼ��I2C

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;//���������ݵ�������������
    }
    u8I2C_SendBuff[0] = 0xa0;
    I2C_SlaveSend(u8I2C_SendBuff,64);
    
	while(1)
	{
        u8I2C_ReceiveLength = I2C_SlaveReceive(&u8I2C_ReceiveBuff[0]);
        if( u8I2C_ReceiveLength < 64 )
        {
            /* ���;����ݸ����� */
            I2C_SlaveSend(&u8I2C_SendBuff[0],64);
        }
        else
        {
            /* ���ʹ��������յ��������� */
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

