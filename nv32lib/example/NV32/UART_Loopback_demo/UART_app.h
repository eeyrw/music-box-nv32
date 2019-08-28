/******************************************************************************
* @brief provide commond UART utilities. 
*
*******************************************************************************/
#ifndef _UART_APP_H_
#define _UART_APP_H_


/******************************************************************************
* Includes
******************************************************************************/
#include "common.h"
#include "uart.h"

/* callback types */
typedef void (*UART_TxDoneCallbackType)(void);
typedef void (*UART_RxDoneCallbackType)(void);
/******************************************************************************
* Global functions declaration
******************************************************************************/
void UART_SendInt(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length);
void UART_ReceiveInt(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length);
void UART_HandleInt(UART_Type *pUART);
void UART_SetTxDoneCallback(UART_Type *pUART, UART_TxDoneCallbackType pfnCallback);
void UART_SetRxDoneCallback(UART_Type *pUART, UART_RxDoneCallbackType pfnCallback);


#endif /* #ifndef _UART_APP_H_ */


