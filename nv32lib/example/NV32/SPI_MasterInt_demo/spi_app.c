/******************************************************************************
*
* @brief SPI数据收发驱动函数
*
******************************************************************************/
#include "common.h"
#include "spi.h"
#include "spi_app.h"

/******************************************************************************/

static SPI_WidthType *pSPI_RxBuf[MAX_SPI_NO] = {NULL};           /* 接收数组指针 */
static SPI_WidthType *pSPI_TxBuf[MAX_SPI_NO] = {NULL};           /* 发送数组指针 */
static uint16_t u16SPI_RxBuffWrPointer[MAX_SPI_NO] = {0};        /* 接收数组元素变量 */
static uint16_t u16SPI_TxBuffRdPointer[MAX_SPI_NO] = {0};        /* 发送数组元素变量 */
static uint32_t gu32SPI_BuffSize[MAX_SPI_NO] = {0};              /* 数组大小*/
static uint8_t u8SPI_Status[MAX_SPI_NO];


/******************************************************************************/

void SPI0_MasterCallback(void);
void SPI1_MasterCallback(void);


/*****************************************************************************//*!
   *
   * @brief SPI模块接收发和发送数据初始化配置
   *        
   * @param[in]  pSPI     指向SPI模块.
   * @param[in]  pConfig  指向配置结构体参数.
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
    u8SPI_Status[0] = SPI_STATUS_IDLE;          //SPI空闲状态
    SPI_SetCallback(SPI0,SPI0_MasterCallback); //注册SPI0回调函数，通过SPI0中断服务函数调用

    pSPI_RxBuf[1] = NULL;
    pSPI_TxBuf[1] = NULL;
    u16SPI_RxBuffWrPointer[1] = 0;
    u16SPI_TxBuffRdPointer[1] = 0;
    u8SPI_Status[1] = SPI_STATUS_IDLE;
    SPI_SetCallback(SPI1,SPI1_MasterCallback); //注册SPI1回调函数，通过SPI1中断服务函数调用
}

/*****************************************************************************//*!
   *
   * @brief  SPI收发数据设置，该函数的作用传递接收数组和发送数组的指针和数据大小。
   *        
   * @param[in]   pSPI  指向SPI模块
   * @param[in]   pWrBuff -- 发送数组指针. 
   * @param[in]   uiLength --接收和发送数据大小
   * @param[out]   pRdBuff --接收数组指针. 
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
    /*传递接收数和发送数组的指针和数组大小*/
    gu32SPI_BuffSize[iSPI] = u32Length;  //记录要发送和接收数据大小
    pSPI_RxBuf[iSPI] = pRdBuff;          //传递接收数组的指针
    pSPI_TxBuf[iSPI] = pWrBuff;          //传递接收数组的指针
    u16SPI_RxBuffWrPointer[iSPI] = 0;    
    u16SPI_TxBuffRdPointer[iSPI] = 0;
    u8SPI_Status[iSPI] = SPI_STATUS_BUSY;
                
    SPI_TxIntEnable(pSPI);         /* 使能发送中断*/                
	SPI_IntEnable(pSPI); 
    return (err);        
}

/*****************************************************************************//*!
   *
   * @brief  SPI0 回调函数通过SPI0中断服务函数调用
   *        
   * @param[in]   none.
   *
   * @return  none.
   *
   *****************************************************************************/

void SPI0_MasterCallback(void)
{
    /*判断接收缓冲区是否满，满则读取接收的数据*/
    if(SPI_IsSPRF(SPI0))       
    {
    	if( u16SPI_RxBuffWrPointer[0] < gu32SPI_BuffSize[0] )  /*判读接收数组是否满（数据是否全部接收）*/
    	{
       		pSPI_RxBuf[0][u16SPI_RxBuffWrPointer[0]++] = SPI_ReadDataReg(SPI0);  /*读接收到的数据到接收数组中*/
        }
       	if (u16SPI_RxBuffWrPointer[0] >= gu32SPI_BuffSize[0]) /*数据全部接收完*/
       	{
       		SPI_ReadDataReg(SPI0);
       		u8SPI_Status[0] |= SPI_STATUS_RX_OVER;
       	}
    }
    /*判断发送缓数据缓冲区是否为空，为空则将发送数组的数据写到发送缓冲区*/
    if(SPI_IsSPTEF(SPI0))
    {
        if(u16SPI_TxBuffRdPointer[0] < gu32SPI_BuffSize[0])   /*判断数据是否全部发送完全，没有发送完继续写数据*/
        {
            SPI_WriteDataReg(SPI0,pSPI_TxBuf[0][u16SPI_TxBuffRdPointer[0]++]);  /*写数据到发送缓冲区*/
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
   * @brief  SPI1 回调函数通过中断服务函数调用
   *        
   * @param[in]   none.
   *
   * @return  if <0, means error, 0: success.
   *

   *****************************************************************************/

void SPI1_MasterCallback(void)
{
    /*判断接收缓冲区是否满，满则读取接收的数据*/
    if(SPI_IsSPRF(SPI1))      
    {
    	if( u16SPI_RxBuffWrPointer[1] < gu32SPI_BuffSize[1] )  /*判读接收数组是否满（数据是否全部接收）*/
    	{
       		pSPI_RxBuf[1][u16SPI_RxBuffWrPointer[1]++] = SPI_ReadDataReg(SPI1); /*读接收到的数据到接收数组中*/
       	}
       	if( u16SPI_RxBuffWrPointer[1] >= gu32SPI_BuffSize[1] )  /*数据全部接收完*/
       	{
       		SPI_ReadDataReg(SPI1);
      		u8SPI_Status[1] |= SPI_STATUS_RX_OVER;
       	}
    }
    /*判断发送缓数据缓冲区是否为空，为空则将发送数组的数据写到发送缓冲区*/
    if(SPI_IsSPTEF(SPI1))
    {
        if(u16SPI_TxBuffRdPointer[1] < gu32SPI_BuffSize[1])     /*判断数据是否全部发送完全，没有发送完继续写数据*/
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
   * @brief  获取发送/接收状态
   *        
   * @param[in]   pSPI  指向SPI模块.
   *
   * @return  发送状态.
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
   * @brief  复位发送/接收状态
   *        
   * @param[in]   pSPI  指向SPI模块.
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





