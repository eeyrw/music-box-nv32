/******************************************************************************
*
* @brief  PIT ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef PIT_H_
#define PIT_H_
#include"common.h"
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
* PIT ͨ�����б�
*
*//*!
*******************************************************************************/
enum
{
    PIT_CHANNEL0 = 0,       /*!< PIT ͨ�� 0 */
    PIT_CHANNEL1            /*!< PIT ͨ�� 1 */
};

/*!                                                                           */


/*!
 * @brief PIT �ص�����.
 *
 */

typedef void (*PIT_CallbackType)(void);            





/* PIT ���ýṹ����
 */  
/*!
 * @brief PIT ���ò���.
 *
 */
typedef struct
{   
    uint8_t bFreeze         : 1;    /*!< 1: �ڵ���ģʽʱ����, 0: �ڵ���ģʽʱ��Ȼ���� */
    uint8_t bModuleDis      : 1;    /*!< 1: ����PITģ��, 0: ʹ��PITģ�� */    
    uint8_t bReserved0      : 1;    /*!< ���� */ 
    uint8_t bReserved1      : 5;    /*!< ���� */ 
    uint8_t bETMerEn        : 1;    /*!< 1: ʹ��PITͨ��, 0: ��ֹPITͨ�� */ 
    uint8_t bInterruptEn    : 1;    /*!< 1: ʹ��PITͨ���ж�, 0: ��ֹPITͨ���ж� */ 
    uint8_t bChainMode      : 1;    /*!< 1: ʹ����ģʽ, 0: ��ֹ��ģʽ */ 
    uint8_t bReserved2      : 5;    /*!< ���� */ 
    uint8_t bFlag           : 1;    /*!< 1: PIT�жϱ�־λ��λ,д1��0, 0: PIT�жϱ�־λû����λ */
    uint8_t bReserved3      : 7;    /*!< ���� */   
    uint32_t u32LoadValue      ;    /*!< 32λͨ������ģֵ */  
} PIT_ConfigType, *PIT_ConfigPtr; 


/******************************************************************************
******************************************************************************/

/*!
 * ��������
 */
/*****************************************************************************//*!
*
* @brief ʹ��PITģ��.
*        
* @param none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_Enable(void)
{

    PIT->MCR &= ~PIT_MCR_MDIS_MASK; 
    
}

/*****************************************************************************//*!
*
* @brief ����PITģ��.
*        
* @param none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_Disable(void)
{

    PIT->MCR |= PIT_MCR_MDIS_MASK; 
}


/*****************************************************************************//*!
*
* @brief ����PIT�ڵ���ģʽ�½�ֹ����.
*        
* @param none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_SetDebugFreeze(void)
{

    PIT->MCR |= PIT_MCR_FRZ_MASK;
}


/*****************************************************************************//*!
*
* @brief ����PIT�ڵ���ģʽ�¼�������.
*        
* @param none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_SetDebugOn(void)
{

    PIT->MCR &= ~PIT_MCR_FRZ_MASK;
}


/*****************************************************************************//*!
*
* @brief PIT��ʱ��ͨ��ʹ��.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelEnable(uint8_t u8Channel)

{

    PIT->CHANNEL[u8Channel].TCTRL |= PIT_TCTRL_TEN_MASK; 
}


/*****************************************************************************//*!
*
* @brief ����PIT��ʱ��ͨ��.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelDisable(uint8_t u8Channel)
{

    PIT->CHANNEL[u8Channel].TCTRL &= ~PIT_TCTRL_TEN_MASK; 
}


/*****************************************************************************//*!
*
* @brief ʹ��PIT��ʱ��ͨ���ж�.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelEnableInt(uint8_t u8Channel)

{

    PIT->CHANNEL[u8Channel].TCTRL |= PIT_TCTRL_TIE_MASK; 
}


/*****************************************************************************//*!
*
* @brief ����PIT��ʱ��ͨ���ж�.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelDisableInt(uint8_t u8Channel)

{

    PIT->CHANNEL[u8Channel].TCTRL &= ~PIT_TCTRL_TIE_MASK; 
}


/*****************************************************************************//*!
*
* @brief ʹ��PIT��ʱ��ͨ����ģʽ.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelEnableChain(uint8_t u8Channel)
{
    PIT->CHANNEL[u8Channel].TCTRL |= PIT_TCTRL_CHN_MASK;
}


/*****************************************************************************//*!
*
* @brief ����PIT��ʱ��ͨ����ģʽ.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelDisableChain(uint8_t u8Channel)

{
    PIT->CHANNEL[u8Channel].TCTRL &= ~PIT_TCTRL_CHN_MASK;
}


/*****************************************************************************//*!
*
* @brief ��ȡPITͨ���жϱ�־λ.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return PITͨ���жϱ�־λ.
*
*****************************************************************************/
__STATIC_INLINE uint8_t PIT_ChannelGetFlags(uint8_t u8Channel)

{
    uint8_t bflag;  
    
    bflag = (PIT->CHANNEL[u8Channel].TFLG & PIT_TFLG_TIF_MASK);    
    
    return bflag;
    
}


/*****************************************************************************//*!
*
* @brief ���PITͨ���жϱ�־λ.
*        
* @param[in]   u8Channel   ͨ����.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void PIT_ChannelClrFlags(uint8_t u8Channel)
{
    PIT->CHANNEL[u8Channel].TFLG |= PIT_TFLG_TIF_MASK;       
}


/******************************************************************************
******************************************************************************/
void PIT_Init(uint8_t u8Channel_No, PIT_ConfigType *pConfig);
void PIT_SetLoadVal(uint8_t u8Channel, uint32_t u32loadvalue);
void PIT_SetCallback(uint8_t u8Channel_No, PIT_CallbackType pfnCallback);
void PIT_DeInit(void);
                                          
#ifdef __cplusplus
}
#endif
#endif /* PIT_H_ */
