/*************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       uart.c
 * @brief      uartͨѶ�ӿں�����
 * @author     Navota
 * @date       2017-1-1
 *************************************************************************/
#include "uart.h"


/*!
 * @brief ��Żص����
 *
 */
UART_CallbackType UART_Callback = NULL;

/******************************************************************************
* ����UART�Ľӿں���
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief ��ʼ��UART,���ж�,��Ӳ��������.
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
* @param[in] pConfig     ����UART�Ľṹ��
*
* @return none
*
*****************************************************************************/
void UART_Init(UART_Type *pUART, UART_ConfigType *pConfig)
{
    uint16_t u16Sbr;
    uint8_t u8Temp;
    uint32_t u32SysClk = pConfig->u32SysClkHz;//����ϵͳʱ��
    uint32_t u32Baud = pConfig->u32Baudrate;//���岨����

    /* �Ϸ��Լ�� */
    ASSERT((pUART == UART0) || (pUART == UART1) || (pUART == UART2));
  
	/* ����ʱ��ѡͨ��������ѡ����Ӧ�� UART �� */
  if (pUART == UART0)
	{
		SIM->SCGC |= SIM_SCGC_UART0_MASK;//ʹ����Ӧ����λ�� ѡͨ��Ӧ UART
	}
#if defined(CPU_NV32)  
	else if (pUART == UART1)
	{
        SIM->SCGC |= SIM_SCGC_UART1_MASK;
	}
    else
	{
        SIM->SCGC |= SIM_SCGC_UART2_MASK;
	}
#endif    
    /*ȷ�������ǽ�������ʱ�� ��ֹ���ͺͽ���*/
    pUART->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
    
    /* ���� UART Ϊ 8 λģʽ, ����żУ��λ */
    pUART->C1 = 0;
    
    /* �����ʼ��� */
    u16Sbr = (((u32SysClk)>>4) + (u32Baud>>1))/u32Baud;
    
    /*�ѵ�ǰ���ݴ���ڴ��ڲ����ʼĴ����У� ��SBRλ��0���������ʷ���������ֹ*/
    u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);
    
    pUART->BDH = u8Temp |  UART_BDH_SBR(u16Sbr >> 8);
    pUART->BDL = (uint8_t)(u16Sbr & UART_BDL_SBR_MASK);

    /*ʹ�� UART ���պͷ��� */
    pUART->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

/*****************************************************************************//*!
*
* @brief ����һ���ַ�.
*        
* @param[in] pUART        ָ������UART����һ���Ļ�ַ
* 
* @return ���յ����ַ�
*
*****************************************************************************/
uint8_t UART_GetChar(UART_Type *pUART)
{

    /* �Ϸ��Լ�� */
    ASSERT((pUART == UART0) || (pUART == UART1) || (pUART == UART2));
 
    /* �ȴ�ֱ��һ���ַ������� */
    while (!(pUART->S1 & UART_S1_RDRF_MASK));
    
    /* ���ؽ��յ���8λ���� */
    return pUART->D;
}
/*****************************************************************************//*!
*
* @brief ����һ���ַ�.
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
* @param[in] u8Char      Ҫ���͵��ַ�
*
* @return none
*
*****************************************************************************/
void UART_PutChar(UART_Type *pUART, uint8_t u8Char)
{    
    /* һֱ�ȴ��� ֱ��������Ϊ�� */
    while (!(pUART->S1 & UART_S1_TDRE_MASK));
    
    /* �����ַ������ݼĴ��� */
    pUART->D = (uint8_t)u8Char;
}

/*****************************************************************************//*!
*
* @brief ����������.
*        
* @param[in] pUART       ָ������UART����һ���Ļ�ַ
* @param[in] pConfig     ������������ýṹ��
*
* @return none
*
*****************************************************************************/
void UART_SetBaudrate(UART_Type *pUART, UART_ConfigBaudrateType *pConfig)
{
    uint8_t u8Temp;
    uint16_t u16Sbr;
    uint32_t u32SysClk    = pConfig->u32SysClkHz;
    uint32_t u32baud       = pConfig->u32Baudrate;
 
    /* �Ϸ��Լ�� */
    ASSERT((pUART == UART0) || (pUART == UART1) || (pUART == UART2));

    /*���㲨���ʣ�����������߾��� */
    u16Sbr = (((u32SysClk)>>4) + (u32baud>>1))/u32baud;

  
    u8Temp = pUART->BDH & ~(UART_BDH_SBR_MASK);
    
    pUART->BDH = u8Temp |  UART_BDH_SBR(u16Sbr >> 8);
    pUART->BDL = (uint8_t)(u16Sbr & UART_BDL_SBR_MASK);


    pUART->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );

}

/*****************************************************************************//*!
*
* @brief ʹ��UART�ж�.
*        
* @param[in] pUART               ָ������UART����һ���Ļ�ַ
* @param[in] InterruptType       �жϵ�����
*
* @return none
*
*****************************************************************************/
void UART_EnableInterrupt(UART_Type *pUART, UART_InterruptType InterruptType)
{

    /* ͨ���Ϸ��Լ�� */
    ASSERT((pUART == UART0) || (pUART == UART1) || (pUART == UART2));

    if (InterruptType == UART_TxBuffEmptyInt)      //�����ж�ʹ��
    {
        pUART->C2 |= UART_C2_TIE_MASK;
    }
    else if (InterruptType == UART_TxCompleteInt)  //��������ж�ʹ��
    {
        pUART->C2 |= UART_C2_TCIE_MASK;
    }
    else if (InterruptType == UART_RxBuffFullInt)  //�������ж�ʹ��
    {
        pUART->C2 |= UART_C2_RIE_MASK;
    }
    else if (InterruptType == UART_IdleLineInt)    //�������ж�ʹ��
    {
        pUART->C2 |= UART_C2_ILIE_MASK;
    }
    else if (InterruptType == UART_RxOverrunInt)   //�����ж�ʹ��
    {
        pUART->C3 |= UART_C3_ORIE_MASK;
    }
    else if (InterruptType == UART_NoiseErrorInt)  //���������ж�ʹ��
    {
        pUART->C3 |= UART_C3_NEIE_MASK;
    }
    else if (InterruptType == UART_FramingErrorInt) //֡�����ж�ʹ��
    {
        pUART->C3 |= UART_C3_FEIE_MASK;
    } 
    else if (InterruptType == UART_ParityErrorInt)  //��żУ���ж�ʹ��
    {
        pUART->C3 |= UART_C3_FEIE_MASK;
    } 
    else
    {
        //�����ݲ�֧�����͵��ж�
    }  
}

/*****************************************************************************//*!
*
* @brief ����UART�ж�.
*        
* @param[in]      ָ������UART����һ���Ļ�ַ
* @param[in]      �жϵ�����
*
* @return none
*
*****************************************************************************/
void UART_DisableInterrupt(UART_Type *pUART, UART_InterruptType InterruptType)
{
     /* ͨ���Ϸ��Լ�� */
    ASSERT((pUART == UART0) || (pUART == UART1) || (pUART == UART2));


    if (InterruptType == UART_TxBuffEmptyInt)       //�����жϽ���
    {
        pUART->C2 &= (~UART_C2_TIE_MASK);
    }
    else if (InterruptType == UART_TxCompleteInt)  //��������жϽ���
    {
        pUART->C2 &= (~UART_C2_TCIE_MASK);
    }
    else if (InterruptType == UART_RxBuffFullInt)  //�������жϽ���
    {
        pUART->C2 &= (~UART_C2_RIE_MASK);
    }
    else if (InterruptType == UART_IdleLineInt)    //�������жϽ���
    {
        pUART->C2 &= (~UART_C2_ILIE_MASK);
    }
    else if (InterruptType == UART_RxOverrunInt)    //�����жϽ���
    {
        pUART->C3 &= (~UART_C3_ORIE_MASK);
    }
    else if (InterruptType == UART_NoiseErrorInt)   //���������жϽ���
    {
        pUART->C3 &= (~UART_C3_NEIE_MASK);
    }
    else if (InterruptType == UART_FramingErrorInt)  //֡�����жϽ���
    {
        pUART->C3 &= (~UART_C3_FEIE_MASK);
    } 
    else if (InterruptType == UART_ParityErrorInt)   //��żУ���жϽ���
    {
        pUART->C3 &= (~UART_C3_FEIE_MASK);
    } 
    else
    {
       //�����ݲ�֧�����͵��ж�
    }  
}


/*****************************************************************************//*!
*
* @brief ������״̬�Ĵ�����ȡUART״̬
*        
* @param[in] pUART     ָ������UART����һ���Ļ�ַ
*
* @return              16λ��״̬
*
*****************************************************************************/
uint16_t UART_GetFlags(UART_Type *pUART)
{
    uint16_t u16StatusFlags = 0;//����ձ�־λ

    u16StatusFlags = pUART->S2; //��״̬�Ĵ��� 2 ��ֵ������־����
    u16StatusFlags = (u16StatusFlags<<8)| pUART->S1; //����״̬�Ĵ���ƴ�Ӹ�����־����

    return u16StatusFlags;//���ر�־������ֵ
}

/*****************************************************************************//*!
*
* @brief ����ض���λ�Ƿ���λ.
*        
* @param[in] pUART         ָ������UART����һ���Ļ�ַ
* @param[in] FlagType      λ������
*
* @return       
*               1, ��־��λ
*               0, ��־����
*
*****************************************************************************/
uint8_t UART_CheckFlag(UART_Type *pUART, UART_FlagType FlagType)
{
    uint16_t u16StatusFlags = 0;

    u16StatusFlags = UART_GetFlags(pUART);

    return (u16StatusFlags & (1<<FlagType));
}

/*****************************************************************************//*!
*
* @brief �ò�ѯģʽ�����ַ���.
*        
* @param[in] pUART          ָ������UART����һ���Ļ�ַ
* @param[in] pSendBuff      �ַ����׵�ַ
* @param[in] u32Length      �ַ����ĳ���
*
* @return       none
*
*****************************************************************************/
void UART_SendWait(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length)
{
    uint8_t u8TxChar;
    uint32_t  i;
    
    for (i = 0; i < u32Length; i++)
    {
        u8TxChar = pSendBuff[i];
        while (!UART_IsTxBuffEmpty(pUART))
        {
            #if defined(ENABLE_WDOG)
                WDOG_Feed();
            #endif        
        }
        UART_WriteDataReg(pUART, u8TxChar);        
    }
}

/*****************************************************************************//*!
*
* @brief �ò�ѯģʽ�����ַ���.
*        
* @param[in] pUART           ָ������UART����һ���Ļ�ַ
* @param[in] pReceiveBuff    ��������ַ������׵�ַ
* @param[in] u32Length       ��Ҫ�����ַ����ĳ���
*
* @return       none
*
*****************************************************************************/
void UART_ReceiveWait(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length)
{
    uint8_t u8RxChar;
    uint32_t i;
    
    for (i = 0; i < u32Length; i++)
    {
        while (!UART_IsRxBuffFull(pUART))
        {
            #if defined(ENABLE_WDOG)
                WDOG_Feed();
            #endif       
        }    
        u8RxChar = UART_ReadDataReg(pUART);
        pReceiveBuff[i] = u8RxChar;
    }
}

/*****************************************************************************//*!
*
* @brief �ȴ��������.
*        
* @param[in] pUART         ָ������UART����һ���Ļ�ַ
*
* @return       none
*
*****************************************************************************/
void UART_WaitTxComplete(UART_Type *pUART)
{
    while (!UART_IsTxComplete(pUART));//һֱ�ȴ�ֱ���������
}

/*****************************************************************************//*!
*
* @brief  ����UARTģ����жϻص�����.
*        
* @param[in]  pfnCallback      �ص������ĵ�ַ
*
* @return none
*
*****************************************************************************/
void UART_SetCallback(UART_CallbackType pfnCallback)
{
    UART_Callback = pfnCallback;
}


/*! @}                                                                        */


/*****************************************************************************//*!
*
* @brief uart0 �жϷ�����.
*        
* @param        none
*
* @return       none
*
*****************************************************************************/
void UART0_Isr(void)
{
    UART_Callback(UART0);
}


#if defined(CPU_NV32) 
/*****************************************************************************//*!
*
* @brief uart0 �жϷ�����.
*        
* @param        none
*
* @return       none
*
*****************************************************************************/
void UART1_Isr(void)
{
    UART_Callback(UART1);
}
/*****************************************************************************//*!
*
* @brief uart0 �жϷ�����.
*        
* @param        none
*
* @return       none
*
*****************************************************************************/
void UART2_Isr(void)
{
    UART_Callback(UART2);
}


#endif



