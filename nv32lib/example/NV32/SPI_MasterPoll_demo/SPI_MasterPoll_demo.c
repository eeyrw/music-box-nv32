/**********************************************************************
 *
 * 实验名称：SPI主机收发数据实验(采用轮训的方式收发数据)
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：SPI发送的同时接收相同的数据，比较接收和发送的数据相同，判断
 *           SPI通信成功 
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "spi.h"
#include "sysinit.h"

/******************************************************************************
* 定义收发数据大小和SPI波特率
******************************************************************************/
#define SPI0_TX_DATA_SIZE		    128
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
	uint32_t i;
	SPI_ConfigType sSPIConfig = {0};
	
		sysinit();
  	printf("\nRunning the SPI_MasterPoll_demo project.\r\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;
      
     /* 初始化SPI0位主机模式*/
    sSPIConfig.u32BitRate = SPI_BIT_RATE;   /*设置波特率*/
    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
    sSPIConfig.sSettings.bModuleEn             = 1; /*!<使能SPI模块*/
    sSPIConfig.sSettings.bMasterMode           = 1; /*设置SPI主机模*/
    sSPIConfig.sSettings.bClkPhase1            = 1; /*设置时钟相位*/
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1; /*!< 从机选择输出使能 */
 
    SPI_Init(SPI0, &sSPIConfig);
    
    gu8Pattern = 0x55;                                  
      /*初始化要发送的数据*/
    for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
    {
        gu8SPI0_TxBuff[i] = i+ gu8Pattern;
    }
    
   
	while(1)
	{
        SPI_TransferWait(SPI0,gu8SPI0_RxBuff,gu8SPI0_TxBuff,SPI0_TX_DATA_SIZE);

        /*核对接收到的数据*/
        for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
        {
            if(gu8SPI0_RxBuff[i] != gu8SPI0_TxBuff[i])
            {                
                gu32ErrorCount++;
                RED_Init();                     /* 亮灯 */
                break;
            }
        } 

        printf("Error counter is %d\r\n",gu32ErrorCount);

        gu32Loop ++;

        printf("SPI communication counter %d\r\n",gu32Loop);

        for(i=0;i<0xffff;i++);
        
	} 

}
