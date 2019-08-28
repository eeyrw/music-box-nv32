/**********************************************************************
 *
 * ʵ�����ƣ�I2C�����շ�����(���ò�ѯ�ķ�ʽ�շ�����)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч�����ṩI2C������ѯ���շ����ݵĿ��
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
    
    /* ��ʼ��I2Cģ��Ϊ��ѯģʽ */
    sI2C_Config.u16F = 0x1F;//���÷�Ƶϵ��
    sI2C_Config.sSetting.bIntEn = 0;//�����ж�
    sI2C_Config.sSetting.bI2CEn = 1;//ʹ��I2C

    I2C_Init(I2C0,&sI2C_Config );//���÷�Ƶϵ��

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;//���������ݵ�������������
    }
 
	while(1)
	{ 
          
          //�ȴ������շ�����ѯ��
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

