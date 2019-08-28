/**********************************************************************
 *
 * ʵ�����ƣ�SPI�����շ�����ʵ��(�����жϵķ�ʽ�շ�����)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����SPI���͵�ͬʱ������ͬ�����ݣ��ȽϽ��պͷ��͵�������ͬ���ж�
 *           SPIͨ�ųɹ� 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "spi_app.h"
#include "sysinit.h"


/******************************************************************************
* �����շ����ݴ�С��SPI������
******************************************************************************/
#define SPI0_TX_DATA_SIZE			128
#define SPI_BIT_RATE                1000000     /* ~1Mbps */


static SPI_WidthType gu8SPI0_RxBuff[SPI0_TX_DATA_SIZE];
static SPI_WidthType gu8SPI0_TxBuff[SPI0_TX_DATA_SIZE];
static  uint32_t  gu32ErrorCount    = 0; 
static  uint8_t   gu8Pattern      = 0;
static uint32_t   gu32Loop = 0;

int main (void);

/********************************************************************/
int main (void)
{
	static uint32_t i;
	SPI_ConfigType sSPIConfig = {0};
	PORT->HDRVE = 0x30;
	
		sysinit();
  	printf("\nRunning the SPI_MasterInt_demo project.\r\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    SPI_InitGlobalVariable(); /*SPIģ����շ��ͷ������ݳ�ʼ������*/
    
    SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;  

    /* ��ʼ��SPI0λ����ģʽ*/
    sSPIConfig.u32BitRate = SPI_BIT_RATE;  /*���ò�����*/
    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
    sSPIConfig.sSettings.bModuleEn             = 1; /*!<ʹ��SPIģ��*/
    sSPIConfig.sSettings.bMasterMode           = 1; /*����SPI����ģ*/
    sSPIConfig.sSettings.bClkPhase1            = 0; /*����ʱ����λ*/
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1; /*!< �ӻ�ѡ�����ʹ�� */
   
 
    SPI_Init(SPI0, &sSPIConfig);
    
    gu8Pattern = 0x55;                                 
    /*��ʼ��Ҫ���͵�����*/
    for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
    {
        gu8SPI0_TxBuff[i] = i+ gu8Pattern;
    }
    
    NVIC_EnableIRQ(SPI0_IRQn);
	while(1)
	{
         /*��ʼ���ͺͽ�������*/
        SPI_Transfer(SPI0,gu8SPI0_RxBuff, gu8SPI0_TxBuff, SPI0_TX_DATA_SIZE);  
        
        /*�ȴ�����ȫ������*/
        while(!(SPI_GetTransferStatus(SPI0) & SPI_STATUS_RX_OVER) );
    
        SPI_ResetTransferStatus(SPI0);

        /*�˶Խ��յ������Ƿ���ȷ*/
        for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
        {
            if(gu8SPI0_RxBuff[i] != gu8SPI0_TxBuff[i])
            {                
                gu32ErrorCount++;
                RED_Init();                     /*���� */
                break;
            }
        } 

        printf("Error counter is %d\r\n",gu32ErrorCount);

        gu32Loop ++;

        printf("SPI communication counter %d\r\n",gu32Loop);

        for(i=0;i<0xffff;i++);
        
	} 

}
