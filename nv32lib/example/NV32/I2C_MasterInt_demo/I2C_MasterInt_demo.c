/**********************************************************************
 *
 * ʵ�����ƣ�I2C�����շ�����ʵ��(�����жϵķ�ʽ�շ�����)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����SPI���͵�ͬʱ������ͬ�����ݣ��ȽϽ��պͷ��͵�������ͬ���ж�
 *          SPIͨ�ųɹ� 
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
    
    /* ��ʼ��I2C�������ݴ�С�Լ��жϻص���� */
    I2C_InitGlobalVariable( );

    /* ��I2C�ṹ��ĸ�����Ա������������ */
    
    sI2C_Config.u16Slt = 0;   //���õͳ�ʱscl�͵�ƽ����ʱ��Ϊ 0
    sI2C_Config.u16F = 0x1F;  //���÷�Ƶϵ��
    sI2C_Config.sSetting.bIntEn = 1;//���ж�ʹ��
    sI2C_Config.sSetting.bI2CEn = 1;//��I2Cʹ��

    I2C_Init(I2C0,&sI2C_Config );   //��ʼ��I2Cģ��
   

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;  //���������ݵ�������������
    }

	while(1)
	{
          /*���������ֽڵĸ���*/
         u32I2C_ReceiveLength = I2C_MasterCheckRead(&u8I2C_ReceiveBuff[0]);
         if( !I2C_IsBusy(I2C0) )
         {
             /* ��I2C���߲���æ����Ӵӻ���ȡ���� */
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
            
                /* �ȴ�I2C���п��У�������ʧ�ܵ���Ŀ */
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


