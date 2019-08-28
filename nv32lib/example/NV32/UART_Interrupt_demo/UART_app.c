/******************************************************************************
*
* @brief UART�����շ���������
*
******************************************************************************/
#include "UART_app.h"

static uint8_t *pUART_TxBuff[MAX_UART_NO] = {NULL};            /* ��������ָ�� */
static uint8_t *pUART_RxBuff[MAX_UART_NO] = {NULL};            /* ��������ָ�� */
static uint16_t gu16UART_TxBuffPos[MAX_UART_NO] = {0};         /* ��������Ԫ�ر��� */
static uint16_t gu16UART_RxBuffPos[MAX_UART_NO] = {0};         /* ��������Ԫ�ر��� */
static uint32_t gu32UART_BuffSize[MAX_UART_NO] = {0};          /* �����С*/

UART_TxDoneCallbackType UART_TxDoneCallback[MAX_UART_NO] = {NULL};
UART_RxDoneCallbackType UART_RxDoneCallback[MAX_UART_NO] = {NULL};

/*****************************************************************************//*!
*
* @brief ���ж�ģʽ�·���һ���ַ���.
*        
* @param[in] pUART      UART�Ļ�ַ
* @param[in] pSendBuff  ָ����Ҫ���͵��ַ���ͷ��ַ
* @param[in] u32Length  �ַ�������
*
* @return       none
*
*****************************************************************************/
void UART_SendInt(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    /* �����Ҫ�������ݵ�ָ���Լ���С */
    pUART_TxBuff[u8Port]        = pSendBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_TxBuffPos[u8Port]  = 0;

    UART_EnableTxBuffEmptyInt(pUART);   /* ʹ�ܷ����ж� */ 
}

/*****************************************************************************//*!
*
* @brief ���ж�ģʽ�½���һ���ַ���.
*        
* @param[in] pUART          UART�Ļ�ַ
* @param[in] pReceiveBuff   ָ����Ҫ���յ��ַ���ͷ��ַ
* @param[in] u32Length      �ַ�������
*
* @return       none
*
*****************************************************************************/
void UART_ReceiveInt(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
     /* �����Ҫ�������ݵ�ָ���Լ���С */
    pUART_RxBuff[u8Port]        = pReceiveBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_RxBuffPos[u8Port]  = 0;

    UART_EnableRxBuffFullInt(pUART);   /* ʹ�ܽ����ж� */ 
}
/*****************************************************************************//*!
*
* @brief UART�жϴ���.
*        
* @param[in] pUART          UART�Ļ�ַ
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
    
    /* �������Ƿ���� */
    if(UART_CheckFlag(pUART,UART_FlagOR))
    {
        read_temp = UART_ReadDataReg(pUART);     
    }
    
    /* ���������Ƿ��� */
    else if(UART_IsRxBuffFull(pUART))       
    {
        /* �������ݵĻ����� */
        pRdBuff = pUART_RxBuff[u8Port];                     /* ��ȡ���ջ����� */
        read_temp = UART_ReadDataReg(pUART);
        pRdBuff[gu16UART_RxBuffPos[u8Port]++]= read_temp;   /* ��Ž��յ������� */
        //pRdBuff[gu16UART_RxBuffPos[u8Port]++] = UART_ReadDataReg(pUART);
        if(gu16UART_RxBuffPos[u8Port] == gu32UART_BuffSize[u8Port])
        {    
            /* ����������������ֹͣ���� */
            UART_DisableRxBuffFullInt(pUART);
            if (UART_RxDoneCallback[u8Port])
			{
			    /* ���ؽ������ */
			    UART_RxDoneCallback[u8Port]();
			}
        }    
    }
    /* ��ⷢ���� */
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
			    /* ���ط������ */
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
* @brief UART�жϷ��ͻص��ӿ�.
*        
* @param[in] pUART          UART�Ļ�ַ
* @param[in] pfnCallback    �ص������ĵ�ַ
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
* @brief UART�жϽ��ջص��ӿ�.
*        
* @param[in] pUART          UART�Ļ�ַ
* @param[in] pfnCallback    �ص������ĵ�ַ
*
* @return       none
*
*****************************************************************************/
void UART_SetRxDoneCallback(UART_Type *pUART, UART_RxDoneCallbackType pfnCallback)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    UART_RxDoneCallback[u8Port] = pfnCallback;
}
