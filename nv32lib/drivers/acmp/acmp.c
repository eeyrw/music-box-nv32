/**************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       acmp.c
 * @brief      ģ��Ƚ���(ACMP)������
 * @author     Navota
 * @date       2018-3-1
 ****************************************************************************/
 
#include "common.h"
#include "acmp.h"

/****************************************************************************!
 * @ ���ACMP�ص������ӿ�
 ****************************************************************************/
ACMP_CallbackPtr ACMP_Callback[2] = {(ACMP_CallbackPtr)NULL};

/**************************************************************************/
void ACMP0_Isr(void);
void ACMP1_Isr(void);

/*****************************************************************************//*!
*
* @brief ��ʼ��ACMPģ��
*        
* @param   pACMPx        ָ��ACMP�Ĵ�����ַ
* @param   pConfig       ���Ʋ���  
*
* @return none.
*
* @see   ACMP_DeInit.  
*
*****************************************************************************/
void ACMP_Init(ACMP_Type *pACMPx, ACMP_ConfigType *pConfig) 
{
    if(ACMP0 == pACMPx)
    {    
        /* ʹ��ACMPģ�������ʱ�� */
        SIM->SCGC |= SIM_SCGC_ACMP0_MASK;
 
        /* ʹ��ACMP�ж� */
        if(pConfig->sCtrlStatus.bits.bIntEn)
            NVIC_EnableIRQ(ACMP0_IRQn);
    }
    else
    {
        SIM->SCGC |= SIM_SCGC_ACMP1_MASK;
        if(pConfig->sCtrlStatus.bits.bIntEn)
            NVIC_EnableIRQ(ACMP1_IRQn);            
    }
    /*ѡ��ACMP������͸����룬������͸��������Ų�ͬ*/
    pACMPx->C0 = pConfig->sPinSelect.byte;
    ACMP_ConfigDAC(pACMPx, &pConfig->sDacSet );
    //pACMPx->C1 = pConfig->sDacSet.byte;
    pACMPx->C2 = pConfig->sPinEnable.byte;
    pACMPx->CS = pConfig->sCtrlStatus.byte;
}

/*****************************************************************************//*!
*
* @brief ����ACMP���ƼĴ���1.
*        
* @param   pACMPx      ָ��ACMPģ��Ĵ�������ַ
* @param   pDACConfig   ָ�� ACMP DAC ���ƽṹ��.
*
* @return none.
*
*****************************************************************************/
void ACMP_ConfigDAC(ACMP_Type *pACMPx, ACMP_DACType *pDACConfig) 
{
    pACMPx->C1 = pDACConfig->byte;  
}

/*****************************************************************************//*!
*
* @brief  ��λACMPģ��.
*        
* @param   pACMPx     ָ��ACMPģ��Ĵ�������ַ
*
* @return none.
*
* @see   ACMP_Init.  
*
*****************************************************************************/
void ACMP_DeInit(ACMP_Type *pACMPx) 
{
    if(ACMP0 == pACMPx)
    {    
        if(pACMPx->CS & ACMP_CS_ACIE_MASK)
            NVIC_DisableIRQ(ACMP0_IRQn);
    }
    else
    {
        if(pACMPx->CS & ACMP_CS_ACIE_MASK)
            NVIC_DisableIRQ(ACMP1_IRQn);            
    }
    
    pACMPx->CS = 0;
    pACMPx->C0 = 0;
    pACMPx->C1 = 0;
    pACMPx->C2 = 0;
    
    if(ACMP0 == pACMPx)
    {    
        SIM->SCGC &= ~SIM_SCGC_ACMP0_MASK;
    }
    else
    {
        SIM->SCGC &= ~SIM_SCGC_ACMP1_MASK;           
    } 
}

/*****************************************************************************//*!
*
* @brief  ����ACMP�ص�������ͨ���жϷ��������ûص�����.
*        
* @param  pACMPx       ָ��ACMPģ��Ĵ�������ַ. 
* @param   pfnCallback  �ص�����.
*
* @return none.
*
*
*****************************************************************************/
void ACMP_SetCallback(ACMP_Type *pACMPx, ACMP_CallbackPtr pfnCallback)
{
    if(ACMP0 == pACMPx)
    {
        ACMP_Callback[0] = pfnCallback;
    }
    else
    {
        ACMP_Callback[1] = pfnCallback;
    }
}

/*****************************************************************************//*!
*
* @brief  ACMP0 �жϷ�����.
*        
* @param  none. 
*
* @return none.
*
*****************************************************************************/
void ACMP0_Isr(void)
{
    if(ACMP_Callback[0])
    {
        ACMP_Callback[0]();             /*���ûص�����*/
    }
}

/*****************************************************************************//*!
*
* @brief  ACMP1 �жϷ�����
*        
* @param  none. 
*
* @return none.
*
*****************************************************************************/
void ACMP1_Isr(void)
{
    if(ACMP_Callback[1])
    {
        ACMP_Callback[1]();             /* ���ûص�����*/
    }
}
