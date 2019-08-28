/******************************************************************************
*
* @brief  ACMP ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef _MY_ACMP_H_
#define _MY_ACMP_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/* ѡ��DAC��׼ */
enum
{
    DAC_REF_BANDGAP = 0,
    DAC_REF_VDDA
};

/******************************************************************************
 * ACMP ģ��������                                             
 ******************************************************************************/
#define     MAX_ACMP_NO             2

/******************************************************************************* 
*
* ����ACMP ������͸���������
*
*******************************************************************************/
#define     ACMP_INPUT_P_EXT0       (0<<4)  /*!< ACMP������ѡ���ⲿ��׼0 */
#define     ACMP_INPUT_P_EXT1       (1<<4)  /*!< ACMP������ѡ���ⲿ��׼1 */
#define     ACMP_INPUT_P_EXT2       (2<<4)  /*!< ACMP������ѡ���ⲿ��׼2 */
#define     ACMP_INPUT_P_DAC        (3<<4)  /*!< ACMP������ѡ��DAC��� */
#define     ACMP_INPUT_N_EXT0       0       /*!< ACMP������ѡ���ⲿ��׼0 */
#define     ACMP_INPUT_N_EXT1       1       /*!< ACMP������ѡ���ⲿ��׼1 */
#define     ACMP_INPUT_N_EXT2       2       /*!< ACMP������ѡ���ⲿ��׼2 */
#define     ACMP_INPUT_N_DAC        3       /*!< ACMP������ѡ��DAC��� */

/******************************************************************************
*
* ����ACMP�жϴ������Ĵ���ģʽ
*
*******************************************************************************/
#define     ACMP_SENSITIVITYMODE_FALLING   0    /*!< ACMP�ж�������½��ط���  */
#define     ACMP_SENSITIVITYMODE_RISING    1    /*!< ACMP�ж�����������ط���  */
#define     ACMP_SENSITIVITYMODE_ANY       3    /*!< ACMP�ж�����������ػ��½��ش��� */ 

/******************************************************************************
*
* ����ACMP ����
*
*******************************************************************************/
#define     ACMP_HYST_20MV                 (0<<6)  /*!< 20mv */
#define     ACMP_HYST_30MV                 (1<<6)  /*!< 30mv */

/******************************************************************************
*
* �����ڲ�DAC�ο���׼
*
*******************************************************************************/
#define     ACMP_DAC_REFERENCE_BANDGAP     (0<<6)  /*!< ѡ���϶Ϊ��׼ */
#define     ACMP_DAC_REFERENCE_VDDA        (1<<6)  /*!< ѡ��VDDAΪ��׼ */

/******************************************************************************
*
* ACMP�ص���������
*
******************************************************************************/
typedef void (*ACMP_CallbackPtr)(void);

/******************************************************************************
*
* ACMP ���ƺ�״̬�ּĴ����ṹ��
*
*******************************************************************************/
typedef union 
{
    uint8_t byte;    				 /*!<����������-�ֽ�*/
    struct 
    {
        uint8_t bMod        : 2;     /*!< �жϴ���ģʽ */
        uint8_t bOutEn      : 1;     /*!< ACMP��������ⲿ���� */
        uint8_t bOutState   : 1;     /*!< ģ��Ƚ�ǿ����ĵ�ǰֵ */
        uint8_t bIntEn      : 1;     /*!< ʹ��ACMP�ж� */
        uint8_t bIntFlag    : 1;     /*!< ACMP �жϱ�־λ */
        uint8_t bHyst       : 1;     /*!< ѡ��ACMP���� */
        uint8_t bEn         : 1;     /*!< ʹ��ACMPģ�� */
    }bits;      				     /*!< ����������-λ�� */
}ACMP_CtrlStatusType, *ACMP_CtrlStatusPtr;  /*!< ACMP ���ƺ�״̬�Ĵ����ṹ*/

/******************************************************************************
*
* ACMP �ⲿ�������ſ��ƽṹ��
*
*******************************************************************************/
typedef union 
{
    uint8_t byte;    				 /*!<����������-�ֽ�*/
    struct 
    {
        uint8_t bNegPin  : 2;        /*!< ������ѡ��*/
        uint8_t          : 2;
        uint8_t bPosPin  : 2;        /*!< ������ѡ�� */
        uint8_t          : 2;
    }bits;      				     /*!< ����������-λ�� */
}ACMP_PinSelType, *ACMP_PinSelPtr; 	     /*!< ACMP ����ѡ��ṹ�� */ 

/******************************************************************************
*
* ACMP �ڲ�DAC���ƽṹ��
*
*******************************************************************************/
typedef union 
{
    uint8_t byte;    					 /*!<����������-�ֽ�*/
    struct 
    {
        uint8_t bVal  : 6;           /*!< DAC�����ƽѡ��*/
        uint8_t bRef  : 1;           /*!< DAC��׼ѡ�� */
        uint8_t bEn   : 1;           /*!< DACʹ�� */
    }bits;      				     /*!< ����������-λ�� */
}ACMP_DACType, *ACMP_DACPtr;	     /*!< ACMP DAC ���ƽṹ�� */ 

/******************************************************************************
*
* ACMP �ⲿ��������ʹ�ܿ��ƽṹ��
*
*******************************************************************************/
typedef union 
{
    uint8_t byte;    				 /*!<����������-�ֽ�*/
	  struct 
    {
        uint8_t bEn   : 3;           /*!< ACMP �ⲿ��������ʹ�� */
        uint8_t bRsvd : 5;
    }bits;      				    /*!< ����������-λ�� */
}ACMP_PinEnType, *ACMP_PinEnPtr;	 /*!< ACMP ����ʹ�ܽṹ�� */ 

/******************************************************************************
*
* ACMP ģ�����ýṹ��
*
*******************************************************************************/
typedef struct 
{
    ACMP_CtrlStatusType  sCtrlStatus;     /*!< ACMP ���ƺ�״�� */
    ACMP_PinSelType      sPinSelect;      /*!< ACMP ����ѡ�� */
    ACMP_DACType         sDacSet;         /*!< ACMP �ڲ�DAC���� */
    ACMP_PinEnType       sPinEnable;      /*!< ACMP �ⲿ��������ʹ�ܿ��� */
}ACMP_ConfigType, *ACMP_ConfigPtr;

/*****************************************************************************//*!
*
* @brief  ʹ��ACMPģ��
*        
* @param[in]  pACMPx     ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_Disable.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_Enable(ACMP_Type *pACMPx)
{
    pACMPx->CS |= ACMP_CS_ACE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ����ACMPģ��
*        
* @param[in]  pACMPx     ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_Enable.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_Disable(ACMP_Type *pACMPx)
{
    pACMPx->CS &= ~ACMP_CS_ACE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ѡ��ACMP�жϴ������Ĵ���ģʽ.
*        
* @param[in]  pACMPx         ָ��ACMPģ��
* @param[in]  u8EdgeSelect   �����ػ��½���ѡ��, 0~3. 
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_SelectIntMode(ACMP_Type *pACMPx, uint8_t u8EdgeSelect)
{
    pACMPx->CS &= ~ACMP_CS_ACMOD_MASK;
    pACMPx->CS |= ACMP_CS_ACMOD(u8EdgeSelect & 0x3);
}

/*****************************************************************************//*!
*
* @brief  ʹ��ACMP��������ⲿ������
*        
* @param[in]  pACMPx     ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_DisablePinOut.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_EnablePinOut(ACMP_Type *pACMPx)
{
    pACMPx->CS |= ACMP_CS_ACOPE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ����ACMP��������ⲿ������
*        
* @param[in]  pACMPx    ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_EnablePinOut.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DisablePinOut(ACMP_Type *pACMPx)
{
    pACMPx->CS &= ~ACMP_CS_ACOPE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ѡ��ACMP���� 
*        
* @param[in]  pACMPx           ָ��ACMPģ��.
* @param[in]  u8HystSelect     2mv or 30mv.   
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_SelectHyst(ACMP_Type *pACMPx, uint8_t u8HystSelect)
{
    pACMPx->CS &= ~ACMP_CS_HYST_MASK;
    pACMPx->CS |= u8HystSelect;
}

/*****************************************************************************//*!
*
* @brief  ʹ��ACMPģ���ж�
*        
* @param[in]  pACMPx      ָ��ACMPģ��.
*              
* @return none.
*
*
* @see    ACMP_DisableInterrupt.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_EnableInterrupt(ACMP_Type *pACMPx)
{
    pACMPx->CS |= ACMP_CS_ACIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ����ACMPģ���ж�
*        
* @param[in]  pACMPx     ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_EnableInterrupt.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DisableInterrupt(ACMP_Type *pACMPx)
{
    pACMPx->CS &= ~ACMP_CS_ACIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ��ȡ�жϱ�־λ
*        
* @param[in]  pACMPx      ָ��ACMPģ��.
*              
* @return none.
*
* @see    ACMP_ClrFlag.
*
*****************************************************************************/
__STATIC_INLINE uint8_t ACMP_GetFlag(ACMP_Type *pACMPx)
{
    return (pACMPx->CS & ACMP_CS_ACF_MASK);
}

/*****************************************************************************//*!
*
* @brief  ����жϱ�־λ
*        
* @param[in]  pACMPx      ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_GetFlag.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_ClrFlag(ACMP_Type *pACMPx)
{
    pACMPx->CS &= ~ACMP_CS_ACF_MASK;
}

/*****************************************************************************//*!
*
* @brief  ACMP������ѡ��
*        
* @param[in]  pACMPx        ָ��ACMPģ��
* @param[in]  u8PosPinSel   ������ѡ��, �ⲿ��׼0~2 ��DAC���.     
*
* @return none.
*
* @see    ACMP_NegativeInputSelect.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_PositiveInputSelect(ACMP_Type *pACMPx, uint8_t u8PosPinSel)
{
    pACMPx->C0 &= ~ACMP_C0_ACPSEL_MASK;
    pACMPx->C0 |= u8PosPinSel;
}

/*****************************************************************************//*!
*
* @brief ACMP������ѡ��.
*         
* @param[in]  pACMPx       ָ��ACMPģ��
* @param[in]  u8NegPinSel   ������ѡ��,  �ⲿ��׼0~2 ��DAC���.     
*
* @return none.
*
* @see    ACMP_PositiveInputSelect.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_NegativeInputSelect(ACMP_Type *pACMPx, uint8_t u8NegPinSel)
{
    pACMPx->C0 &= ~ACMP_C0_ACNSEL_MASK;
    pACMPx->C0 |= u8NegPinSel;
}

/*****************************************************************************//*!
*
* @brief  ʹ���ڲ�DAC
*        
* @param[in]  pACMPx     ָ��ACMPģ��
*              
* @return none.
*
* @see    ACMP_DacDisable.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DacEnable(ACMP_Type *pACMPx)
{
    pACMPx->C1 |= ACMP_C1_DACEN_MASK;
}

/*****************************************************************************//*!
*
* @brief �����ڲ�DAC
*        
* @param[in]  pACMPx     ָ��ACMPģ��.
*              
* @return none.
*
* @see    ACMP_DacEnable.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DacDisable(ACMP_Type *pACMPx)
{
    pACMPx->C1 &= ~ACMP_C1_DACEN_MASK;
}

/*****************************************************************************//*!
*
* @brief  DAC��׼ѡ��
*        
* @param[in]  pACMPx        ָ��ACMPģ��.
* @param[in]  u8RefSelect   DAC�ο�ѡ��: ��϶ or VDDA.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DacReferenceSelect(ACMP_Type *pACMPx, uint8_t u8RefSelect)
{
    pACMPx->C1 &= ~ACMP_C1_DACREF_MASK;
    pACMPx->C1 |= u8RefSelect;
}

/*****************************************************************************//*!
*
* @brief  DAC�����ƽѡ��
*        
* @param[in]  pACMPx        ָ��ACMPģ��.
* @param[in]  u8DacValue    DAC���ѡ��, Voutput= (Vin/64)x(DACVAL[5:0]+1).
*
* @return none.
*
*
*****************************************************************************/
__STATIC_INLINE void ACMP_DacOutputSet(ACMP_Type *pACMPx, uint8_t u8DacValue)
{
    ASSERT(!(u8DacValue & (~ACMP_C1_DACVAL_MASK)));
    pACMPx->C1 &= ~ACMP_C1_DACVAL_MASK;
    pACMPx->C1 |= ACMP_C1_DACVAL(u8DacValue);
}

/*****************************************************************************//*!
*
* @brief  ʹ��ACMP�ⲿ��������.
*        
* @param[in]  pACMPx        ָ��ACMPģ��.
* @param[in]  u8InputPin    ACMP �ⲿ��������, 0~2.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_InputPinEnable(ACMP_Type *pACMPx, uint8_t u8InputPin)
{
    ASSERT(!(u8InputPin & (~ACMP_C2_ACIPE_MASK)));
    pACMPx->C2 |= ACMP_C2_ACIPE(u8InputPin);
}

/*****************************************************************************//*!
*
* @brief  ����ACMP�ⲿ��������.
*        
* @param[in]  pACMPx        ָ��ACMPģ��.
* @param[in]  u8InputPin    ACMP �ⲿ��������, 0~2.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ACMP_InputPinDisable(ACMP_Type *pACMPx, uint8_t u8InputPin)
{
    ASSERT(!(u8InputPin & (~ACMP_C2_ACIPE_MASK)));
    pACMPx->C2 &= ~ACMP_C2_ACIPE(u8InputPin);
}

/******************************************************************************/
void ACMP_Init(ACMP_Type *pACMPx, ACMP_ConfigType *pConfig);
void ACMP_DeInit(ACMP_Type *pACMPx); 
void ACMP_ConfigDAC(ACMP_Type *pACMPx, ACMP_DACType *pDACConfig);
void ACMP_SetCallback(ACMP_Type *pACMPx, ACMP_CallbackPtr pfnCallback);
#ifdef __cplusplus
}
#endif
#endif 
