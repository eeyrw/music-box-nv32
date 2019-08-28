#ifndef SPI_APP_H_
#define SPI_APP_H_

#include "spi.h"

/******************************************************************************
* ���� PTE3 ���� SPI0 CS
*
*******************************************************************************/
#define CS0_PIN_MASK ( 1 << 3)
#define CS0_PIN_INPUT() (GPIOB_PDDR &= ~CS0_PIN_MASK)
#define CS0_PIN_Init()	{GPIOB_PDDR |=  CS0_PIN_MASK; GPIOB_PSOR = CS0_PIN_MASK;}
#define CS0_HIGH()		(GPIOB_PSOR = CS0_PIN_MASK)
#define CS0_LOW()		(GPIOB_PCOR = CS0_PIN_MASK)

/******************************************************************************
* ����SPIģ����
******************************************************************************/

#define     MAX_SPI_NO              2

/******************************************************************************
* ����SPI���������С
*
*******************************************************************************/

#define     SPI0_TX_BUF_MAX     16
#define     SPI0_RX_BUF_MAX     16
#define     SPI1_TX_BUF_MAX     16
#define     SPI1_RX_BUF_MAX     16

/******************************************************************************
* ����SPI���պͷ���״̬
*
*******************************************************************************/
#define SPI_STATUS_IDLE			  0x00
#define SPI_STATUS_RX_OVER		0x01
#define SPI_STATUS_TX_OVER		0x02
#define SPI_STATUS_BUSY			  0x80

/******************************************************************************/

ResultType SPI_Transfer(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType *pWrBuff,uint32 uiLength);
void SPI_InitGlobalVariable( void );
uint8_t SPI_GetTransferStatus( SPI_Type *pSPI );
void SPI_ResetTransferStatus( SPI_Type *pSPI );

#endif 
