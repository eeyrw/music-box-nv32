/******************************************************************************
*
* @brief UART数据收发驱动函数
*
******************************************************************************/
#include "UART_app.h"

static uint8_t *pUART_TxBuff[MAX_UART_NO] = {NULL};            /* 发送数组指针 */
static uint8_t *pUART_RxBuff[MAX_UART_NO] = {NULL};            /* 接收数组指针 */
static uint16_t gu16UART_TxBuffPos[MAX_UART_NO] = {0};         /* 发送数组元素变量 */
static uint16_t gu16UART_RxBuffPos[MAX_UART_NO] = {0};         /* 接收数组元素变量 */
static uint32_t gu32UART_BuffSize[MAX_UART_NO] = {0};          /* 数组大小*/

UART_TxDoneCallbackType UART_TxDoneCallback[MAX_UART_NO] = {NULL};
UART_RxDoneCallbackType UART_RxDoneCallback[MAX_UART_NO] = {NULL};

/*****************************************************************************//*!
*
* @brief 在中断模式下发送一串字符串.
*        
* @param[in] pUART      UART的基址
* @param[in] pSendBuff  指向所要发送的字符串头地址
* @param[in] u32Length  字符的数量
*
* @return       none
*
*****************************************************************************/
void UART_SendInt(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    /* 存放所要发送数据的指针以及大小 */
    pUART_TxBuff[u8Port]        = pSendBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_TxBuffPos[u8Port]  = 0;

    UART_EnableTxBuffEmptyInt(pUART);   /* 使能发送中断 */ 
}

/*****************************************************************************//*!
*
* @brief 在中断模式下接收一串字符串.
*        
* @param[in] pUART          UART的基址
* @param[in] pReceiveBuff   指向所要接收的字符串头地址
* @param[in] u32Length      字符的数量
*
* @return       none
*
*****************************************************************************/
void UART_ReceiveInt(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
     /* 存放所要接收数据的指针以及大小 */
    pUART_RxBuff[u8Port]        = pReceiveBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_RxBuffPos[u8Port]  = 0;

    UART_EnableRxBuffFullInt(pUART);   /* 使能接收中断 */ 
}
/*****************************************************************************//*!
*
* @brief UART中断处理.
*        
* @param[in] pUART          UART的基址
*
* @return       none
*
*****************************************************************************/
void UART_HandleInt(UART_Type *pUART)
{
    uint8_t   u8Port;
    uint8_t   *pRdBuff;
    uint8_t   *pWrBuff;  
    volatile uint8_t read_temp = 0;
    
    u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
    /* 检查接收是否过载 */
    if(UART_CheckFlag(pUART,UART_FlagOR))
    {
        read_temp = UART_ReadDataReg(pUART);     
    }
    
    /* 检测接收器是否满 */
    else if(UART_IsRxBuffFull(pUART))       
    {
        /* 接收数据的缓冲区 */
        pRdBuff = pUART_RxBuff[u8Port];                     /* 获取接收缓冲区 */
        read_temp = UART_ReadDataReg(pUART);
        pRdBuff[gu16UART_RxBuffPos[u8Port]++]= read_temp;   /* 存放接收到的数据 */
        //pRdBuff[gu16UART_RxBuffPos[u8Port]++] = UART_ReadDataReg(pUART);
        if(gu16UART_RxBuffPos[u8Port] == gu32UART_BuffSize[u8Port])
        {    
            /* 若接收区已满，则停止接收 */
            UART_DisableRxBuffFullInt(pUART);
            if (UART_RxDoneCallback[u8Port])
			{
			    /* 返回接收完成 */
			    UART_RxDoneCallback[u8Port]();
			}
        }    
    }
    /* 检测发送器 */
			else if(UART_IsTxBuffEmpty(pUART))
    {
        if(gu16UART_TxBuffPos[u8Port] != gu32UART_BuffSize[u8Port])
        {
            
            pWrBuff = pUART_TxBuff[u8Port];
            UART_WriteDataReg(pUART, pWrBuff[gu16UART_TxBuffPos[u8Port]++]);     
        }  
        else
        {
			UART_DisableTxBuffEmptyInt(pUART);
			if (UART_TxDoneCallback[u8Port])
			{
			    /* 返回发送完成 */
			    UART_TxDoneCallback[u8Port]();
			}
        }
    }
    else
    {
        
    }
}
/*****************************************************************************//*!
*
* @brief UART中断发送回调接口.
*        
* @param[in] pUART          UART的基址
* @param[in] pfnCallback    回调函数的地址
*
* @return       none
*
*****************************************************************************/
void UART_SetTxDoneCallback(UART_Type *pUART, UART_TxDoneCallbackType pfnCallback)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    UART_TxDoneCallback[u8Port] = pfnCallback;
}
/*****************************************************************************//*!
*
* @brief UART中断接收回调接口.
*        
* @param[in] pUART          UART的基址
* @param[in] pfnCallback    回调函数的地址
*
* @return       none
*
*****************************************************************************/
void UART_SetRxDoneCallback(UART_Type *pUART, UART_RxDoneCallbackType pfnCallback)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    UART_RxDoneCallback[u8Port] = pfnCallback;
}
