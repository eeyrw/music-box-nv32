/******************************************************************************
*
* @brief SPI�����շ���������
*
******************************************************************************/
#include "common.h"
#include "spi.h"
#include "spi_app.h"

/******************************************************************************/

static SPI_WidthType *pSPI_RxBuf[MAX_SPI_NO] = {NULL};           /* ��������ָ�� */
static SPI_WidthType *pSPI_TxBuf[MAX_SPI_NO] = {NULL};           /* ��������ָ�� */
static uint16_t u16SPI_RxBuffWrPointer[MAX_SPI_NO] = {0};        /* ��������Ԫ�ر��� */
static uint16_t u16SPI_TxBuffRdPointer[MAX_SPI_NO] = {0};        /* ��������Ԫ�ر��� */
static uint32_t gu32SPI_BuffSize[MAX_SPI_NO] = {0};              /* �����С*/
static uint8_t u8SPI_Status[MAX_SPI_NO];


/******************************************************************************/

void SPI0_MasterCallback(void);
void SPI1_MasterCallback(void);


/*****************************************************************************//*!
   *
   * @brief SPIģ����շ��ͷ������ݳ�ʼ������
   *        
   * @param[in]  pSPI     ָ��SPIģ��.
   * @param[in]  pConfig  ָ�����ýṹ�����.
   *
   * @return none.
   *
   *****************************************************************************/
void SPI_InitGlobalVariable( void )
{
    pSPI_RxBuf[0] = NULL;
    pSPI_TxBuf[0] = NULL;
    u16SPI_RxBuffWrPointer[0] = 0;
    u16SPI_TxBuffRdPointer[0] = 0;
    u8SPI_Status[0] = SPI_STATUS_IDLE;          //SPI����״̬
    SPI_SetCallback(SPI0,SPI0_MasterCallback); //ע��SPI0�ص�������ͨ��SPI0�жϷ���������

    pSPI_RxBuf[1] = NULL;
    pSPI_TxBuf[1] = NULL;
    u16SPI_RxBuffWrPointer[1] = 0;
    u16SPI_TxBuffRdPointer[1] = 0;
    u8SPI_Status[1] = SPI_STATUS_IDLE;
    SPI_SetCallback(SPI1,SPI1_MasterCallback); //ע��SPI1�ص�������ͨ��SPI1�жϷ���������
}

/*****************************************************************************//*!
   *
   * @brief  SPI�շ��������ã��ú��������ô��ݽ�������ͷ��������ָ������ݴ�С��
   *        
   * @param[in]   pSPI  ָ��SPIģ��
   * @param[in]   pWrBuff -- ��������ָ��. 
   * @param[in]   uiLength --���պͷ������ݴ�С
   * @param[out]   pRdBuff --��������ָ��. 
   * 
   * @return  if <0, means error, 0: success.
   *
   *****************************************************************************/
ResultType SPI_Transfer(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType * pWrBuff,uint32 u32Length)
{
    uint8_t  iSPI = ((uint32_t)pSPI-(uint32_t)SPI0)>>12;
    ResultType err = SPI_ERR_SUCCESS;
    
    if(!u32Length)
    {
        return (err);
    }
    /*���ݽ������ͷ��������ָ��������С*/
    gu32SPI_BuffSize[iSPI] = u32Length;  //��¼Ҫ���ͺͽ������ݴ�С
    pSPI_RxBuf[iSPI] = pRdBuff;          //���ݽ��������ָ��
    pSPI_TxBuf[iSPI] = pWrBuff;          //���ݽ��������ָ��
    u16SPI_RxBuffWrPointer[iSPI] = 0;    
    u16SPI_TxBuffRdPointer[iSPI] = 0;
    u8SPI_Status[iSPI] = SPI_STATUS_BUSY;
                
    SPI_TxIntEnable(pSPI);         /* ʹ�ܷ����ж�*/                
	SPI_IntEnable(pSPI); 
    return (err);        
}

/*****************************************************************************//*!
   *
   * @brief  SPI0 �ص�����ͨ��SPI0�жϷ���������
   *        
   * @param[in]   none.
   *
   * @return  none.
   *
   *****************************************************************************/

void SPI0_MasterCallback(void)
{
    /*�жϽ��ջ������Ƿ����������ȡ���յ�����*/
    if(SPI_IsSPRF(SPI0))       
    {
    	if( u16SPI_RxBuffWrPointer[0] < gu32SPI_BuffSize[0] )  /*�ж����������Ƿ����������Ƿ�ȫ�����գ�*/
    	{
       		pSPI_RxBuf[0][u16SPI_RxBuffWrPointer[0]++] = SPI_ReadDataReg(SPI0);  /*�����յ������ݵ�����������*/
        }
       	if (u16SPI_RxBuffWrPointer[0] >= gu32SPI_BuffSize[0]) /*����ȫ��������*/
       	{
       		SPI_ReadDataReg(SPI0);
       		u8SPI_Status[0] |= SPI_STATUS_RX_OVER;
       	}
    }
    /*�жϷ��ͻ����ݻ������Ƿ�Ϊ�գ�Ϊ���򽫷������������д�����ͻ�����*/
    if(SPI_IsSPTEF(SPI0))
    {
        if(u16SPI_TxBuffRdPointer[0] < gu32SPI_BuffSize[0])   /*�ж������Ƿ�ȫ��������ȫ��û�з��������д����*/
        {
            SPI_WriteDataReg(SPI0,pSPI_TxBuf[0][u16SPI_TxBuffRdPointer[0]++]);  /*д���ݵ����ͻ�����*/
        }  
        else
        {
			SPI_TxIntDisable(SPI0);
			u8SPI_Status[0] |= SPI_STATUS_TX_OVER;
        }
    }
    
}
/*****************************************************************************//*!
   *
   * @brief  SPI1 �ص�����ͨ���жϷ���������
   *        
   * @param[in]   none.
   *
   * @return  if <0, means error, 0: success.
   *

   *****************************************************************************/

void SPI1_MasterCallback(void)
{
    /*�жϽ��ջ������Ƿ����������ȡ���յ�����*/
    if(SPI_IsSPRF(SPI1))      
    {
    	if( u16SPI_RxBuffWrPointer[1] < gu32SPI_BuffSize[1] )  /*�ж����������Ƿ����������Ƿ�ȫ�����գ�*/
    	{
       		pSPI_RxBuf[1][u16SPI_RxBuffWrPointer[1]++] = SPI_ReadDataReg(SPI1); /*�����յ������ݵ�����������*/
       	}
       	if( u16SPI_RxBuffWrPointer[1] >= gu32SPI_BuffSize[1] )  /*����ȫ��������*/
       	{
       		SPI_ReadDataReg(SPI1);
      		u8SPI_Status[1] |= SPI_STATUS_RX_OVER;
       	}
    }
    /*�жϷ��ͻ����ݻ������Ƿ�Ϊ�գ�Ϊ���򽫷������������д�����ͻ�����*/
    if(SPI_IsSPTEF(SPI1))
    {
        if(u16SPI_TxBuffRdPointer[1] < gu32SPI_BuffSize[1])     /*�ж������Ƿ�ȫ��������ȫ��û�з��������д����*/
        {
            SPI_WriteDataReg(SPI1,pSPI_TxBuf[1][u16SPI_TxBuffRdPointer[1]++]);      
        }  
        else
        {
			SPI_TxIntDisable(SPI1);
			u8SPI_Status[1] |= SPI_STATUS_TX_OVER;
        }
    }
}
/*****************************************************************************//*!
   *
   * @brief  ��ȡ����/����״̬
   *        
   * @param[in]   pSPI  ָ��SPIģ��.
   *
   * @return  ����״̬.
   *
   *****************************************************************************/
uint8_t SPI_GetTransferStatus( SPI_Type *pSPI )
{
	if( pSPI == SPI0 )
    {
        return u8SPI_Status[0];
    }
    else
    {
        return u8SPI_Status[1];
    }
}

/*****************************************************************************//*!
   *
   * @brief  ��λ����/����״̬
   *        
   * @param[in]   pSPI  ָ��SPIģ��.
   *
   * @return  none.
   *
   *****************************************************************************/
void SPI_ResetTransferStatus( SPI_Type *pSPI )
{
    if( pSPI == SPI0 )
    {
        u8SPI_Status[0] = SPI_STATUS_IDLE;
    }
    else
    {
        u8SPI_Status[1] = SPI_STATUS_IDLE;
    }
}





