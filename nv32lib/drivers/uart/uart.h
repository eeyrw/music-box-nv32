/******************************************************************************
*
* @brief  UART ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "wdog.h"

#define MAX_UART_NO             3


/*!
* @brief UART ��������.
*
*/ 

typedef struct
{
    uint32_t bEnable      : 1;                /*!< 1: ʹ��, 0: ���� */
    uint32_t resvd        : 31;               /*!< 1: ����λ�� */
} UART_SettingType;
/*! @}                                                                        */

 /*!
 * @brief UART ���ýṹ��.
 *
 */   
typedef struct 
{
    UART_SettingType    sSettings;  /*!< UART ���� */
    uint32_t    				u32SysClkHz;        /*!< ϵͳʱ�� */
    uint32_t    				u32Baudrate;        /*!< UART ������ */
} UART_ConfigType;
/*! @}                                                                        */

 /*!
 * @brief UART ���������ýṹ��.
 *
 */   
typedef struct
{
    uint32_t    u32SysClkHz;        /*!< ϵͳʱ�� */
    uint32_t    u32Baudrate;        /*!< UART ������ */
} UART_ConfigBaudrateType;
/*! @}                                                                        */

/******************************************************************************
*define uart����ģʽ�б�
*
*//*! @uart����ģʽ�б�
* @{
******************************************************************************/
typedef enum
{
    UART_Mode8Bit,                  /*!< 8λģʽ */
    UART_Mode9Bit,                  /*!< 9λģʽ */
    UART_ModeEnableLoopback,        /*!< ʹ�ܻ���ģʽ */
    UART_ModeDisableLoopback,       /*!< ���û���ģʽ*/
    UART_ModeEnableSingleWire,      /*!< ʹ��UART����ģʽ */
    UART_ModeDisableSingleWire,     /*!< ����UART����ģʽ */
} UART_ModeType;
/*! @}                                                                        */

/******************************************************************************
*define uart�ж�����
*
*//*! @uart�ж������б�
* @{
******************************************************************************/

typedef enum
{
    UART_TxBuffEmptyInt,            /*!< �����ж� */
    UART_TxCompleteInt,             /*!< ��������ж� */
    UART_RxBuffFullInt,             /*!< �������ж� */

    UART_IdleLineInt,               /*!< �������ж� */

    UART_RxOverrunInt,              /*!< �����ж� */
    UART_NoiseErrorInt,             /*!< ���������ж� */
    UART_FramingErrorInt,           /*!< /֡�����ж� */
    UART_ParityErrorInt,            /*!< ��żУ���ж� */
} UART_InterruptType;
/*! @}                                                                        */

/******************************************************************************
*define uart��־���Ͷ���
*
*//*! @uart��־���Ͷ����б�
* @{
******************************************************************************/
typedef enum
{
    UART_FlagPF = 0,        /*!< ���� */
    UART_FlagFE,            /*!< ֡�����־ */
    UART_FlagNF,            /*!< ������־ */
    UART_FlagOR,            /*!< ���չ��� */
    UART_FlagIDLE,          /*!< �����߱�־ */
    UART_FlagRDRF,          /*!< ������������־ */
    UART_FlagTC,            /*!< ������ɱ�־ */
    UART_FlagTDRE,          /*!< ���ͼĴ����ձ�־ */

    UART_FlagRAF,           /*!< ��Ч��������־ */
    UART_FlagLBKDE,         /*!< LIN������ʹ�� */
    UART_FlagBRK13,         /*!< ����ַ����� */ 
    UART_FlagRWUID,         /*!< ���ջ��ѿ��м�� */
    UART_FlagRXINV,         /*!< �������ݷ�ת */
    UART_FlagRev1,          /*!< ���� */
    UART_FlagRXEDGIF,       /*!< RXD���ŵ���Ч�����жϱ�־ */
    UART_FlagLBKDIF,        /*!< LIN��ֹ����жϱ�־ */
} UART_FlagType;
/*! @}                                                                        */

/*!
 * @brief UART �ص�����.
 *
 */
typedef void (*UART_CallbackType)(UART_Type *pUART);

/******************************************************************************
******************************************************************************/

/*!
 * ��������
 */
/******************************************************************************
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief ��ȡ��������
*        
* @param[in] pUART        ָ������UART����һ���Ļ�ַ
*
* @return ���յ����ַ�
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_ReadDataReg(UART_Type *pUART)
{
    return pUART->D;
}

/*****************************************************************************//*!
*
* @brief д�����ַ�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
* @param[in] u8Char      ��Ҫ���͵��ַ�
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_WriteDataReg(UART_Type *pUART, uint8_t u8Char)
{
    pUART->D = (uint8_t)u8Char;
}

/*****************************************************************************//*!
*
* @brief ���������ݼĴ����Ƿ���
*
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return 0, û���ַ�����; ��0, ���ַ����յ�
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_CharPresent(UART_Type *pUART)
{  
    return (pUART->S1 & UART_S1_RDRF_MASK);
}
/*****************************************************************************//*!
*
* @brief ʹ�ܷ�����
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableTx(UART_Type *pUART)
{
    
    pUART->C2 |= UART_C2_TE_MASK;
}
/*****************************************************************************//*!
*
* @brief ���÷�����
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableTx(UART_Type *pUART)
{    
    pUART->C2 &= (~UART_C2_TE_MASK);
}

/*****************************************************************************//*!
*
* @brief ʹ�ܽ�����
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableRx(UART_Type *pUART)
{    
    pUART->C2 |= UART_C2_RE_MASK;
}

/*****************************************************************************//*!
*
* @brief ���ý�����
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableRx(UART_Type *pUART)
{    
    pUART->C2 &= (~UART_C2_RE_MASK);
}

/*****************************************************************************//*!
*
* @brief ʹ��ѭ��ģʽ
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableLoopback(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_LOOPS_MASK;
    pUART->C1 &= (~UART_C1_RSRC_MASK);
}

/*****************************************************************************//*!
*
* @brief ʹ�ܵ���ģʽ
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableSingleWire(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_LOOPS_MASK;
    pUART->C1 |= UART_C1_RSRC_MASK;
}

/*****************************************************************************//*!
*
* @brief ����8λģʽ
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_Set8BitMode(UART_Type *pUART)
{
    pUART->C1 &= (~UART_C1_M_MASK);
}

/*****************************************************************************//*!
*
* @brief ����9λģʽ
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_Set9BitMode(UART_Type *pUART)
{
    pUART->C1 |= UART_C1_M_MASK;
}

/*****************************************************************************//*!
*
* @brief ʹ�ܷ��Ϳ��ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableTxBuffEmptyInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_TIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ʹ�ܷ�������ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableTxCompleteInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_TCIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ʹ�ܽ������ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_EnableRxBuffFullInt(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_RIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ���÷��Ϳ��ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableTxBuffEmptyInt(UART_Type *pUART)
{
        pUART->C2 &= (~UART_C2_TIE_MASK);    
}

/*****************************************************************************//*!
*
* @brief ���ô�������ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableTxCompleteInt(UART_Type *pUART)
{
    pUART->C2 &= (~UART_C2_TCIE_MASK);   
}

/*****************************************************************************//*!
*
* @brief ���ý������ж�
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void UART_DisableRxBuffFullInt(UART_Type *pUART)
{
    pUART->C2 &= (~UART_C2_RIE_MASK);  
}

/*****************************************************************************//*!
*
* @brief ���ͼ���ַ�
*        
* @param[in] pUART     ָ������UART����һ���Ļ�ַ
*
* @return       none
*
*****************************************************************************/
__STATIC_INLINE void UART_PutBreak(UART_Type *pUART)
{
    pUART->C2 |= UART_C2_SBK_MASK; 
    pUART->C2 &= (~UART_C2_SBK_MASK);
}

/*****************************************************************************//*!
*
* @brief ��ⷢ����ɱ�־.
*        
* @param[in] pUART    ָ������UART����һ���Ļ�ַ
*
* @return       
*               1, ���ͱ�־��λ
*               0, ���ͱ�־���
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsTxComplete(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_TC_MASK);
}
/*****************************************************************************//*!
*
* @brief ��ⷢ�ͼĴ����Ƿ��
*        
* @param[in] pUART    ָ������UART����һ���Ļ�ַ
* 
* @return       
*               1, ���ͼĴ�����
*               0, ���ͼĴ�����Ϊ��
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsTxBuffEmpty(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_TDRE_MASK);
}
/*****************************************************************************//*!
*
* @brief ���������ݼĴ����Ƿ���
*        
* @param[in] pUART    ָ������UART����һ���Ļ�ַ
*
* @return       
*               1, �������ݼĴ�����
*               0, �������ݼĴ�����Ϊ��
*
*****************************************************************************/
__STATIC_INLINE uint8_t UART_IsRxBuffFull(UART_Type *pUART)
{
    return (pUART->S1 & UART_S1_RDRF_MASK);
}
/*! @}                                                                        */


/******************************************************************************
******************************************************************************/
void UART_Init(UART_Type *pUART, UART_ConfigType *pConfig);
uint8_t UART_GetChar(UART_Type *pUART);
void UART_PutChar(UART_Type *pUART, uint8_t u8Char);
void UART_SetBaudrate(UART_Type *pUART, UART_ConfigBaudrateType *pConfig);
void UART_EnableInterrupt(UART_Type *pUART, UART_InterruptType InterruptType);
void UART_DisableInterrupt(UART_Type *pUART, UART_InterruptType InterruptType);
uint16_t UART_GetFlags(UART_Type *pUART);
uint8_t UART_CheckFlag(UART_Type *pUART, UART_FlagType FlagType);
void UART_SendWait(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length);
void UART_ReceiveWait(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length);
void UART_WaitTxComplete(UART_Type *pUART);
void UART_SetCallback(UART_CallbackType pfnCallback);
void UART0_Isr(void);
void UART1_Isr(void);
void UART2_Isr(void);


#ifdef __cplusplus
}
#endif
#endif /* #ifndef _UART_H_ */
