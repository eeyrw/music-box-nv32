/******************************************************************************
*
* @brief  ETM ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef ETM_H_
#define ETM_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/******************************************************************************
* ETM ����ֵ����
*
*//*! 
* @{
*******************************************************************************/
#define ETM_ERR_SUCCESS         0               /*!< ���سɹ� */
#define ETM_ERR_INVALID_PARAM   1               /*!< ������Ч���� */
/*! @}                                                                        */


/******************************************************************************
* ETM ͨ���Ŷ���
*
*//*!
* @{
*******************************************************************************/
#define ETM_CHANNEL_CHANNEL0    0               /*!< ͨ��0 */
#define ETM_CHANNEL_CHANNEL1    1               /*!< ͨ��1 */
#define ETM_CHANNEL_CHANNEL2    2               /*!< ͨ��2 */
#define ETM_CHANNEL_CHANNEL3    3               /*!< ͨ��3 */
#define ETM_CHANNEL_CHANNEL4    4               /*!< ͨ��4 */
#define ETM_CHANNEL_CHANNEL5    5               /*!< ͨ��5 */

#define ETM_CHANNELPAIR0        0               /*!< ͨ����0:ch0 & ch1 */
#define ETM_CHANNELPAIR1        2               /*!< ͨ����1:ch2 & ch3 */
#define ETM_CHANNELPAIR2        4               /*!< ͨ����2:ch4 & ch5 */

/*! @}                                                                        */

/******************************************************************************
* ETM PWMģʽ����
*
*//*! 
* @{
*******************************************************************************/
#define ETM_PWMMODE_EDGEALLIGNED      1          /*!< ���ض���PWM */
#define ETM_PWMMODE_CENTERALLIGNED    2          /*!< �������PWM */
#define ETM_PWMMODE_COMBINE           3          /*!< ����PWM     */
/*! @}                                                   */

/******************************************************************************
* ETM ��ʼ��ֵ����
*
*//*!
* @{
*******************************************************************************/
#define ETM_MOD_INIT	(20000-1)                /*!< MOD ��ʼ��ֵ */
#define ETM_C0V_INIT	1000                     /*!< C0V ��ʼ��ֵ */
#define ETM_C1V_INIT	1000                     /*!< C1V ��ʼ��ֵ */
#define ETM_C2V_INIT	1000                     /*!< C2V ��ʼ��ֵ */    
#define ETM_C3V_INIT	1000                     /*!< C3V ��ʼ��ֵ */
#define ETM_C4V_INIT	1000                     /*!< C4V ��ʼ��ֵ */
#define ETM_C5V_INIT	1000                     /*!< C5V ��ʼ��ֵ */
/*! @}                                                                        */

/******************************************************************************
* ETM ����ģʽλ����
*
*//*!
* @{
*******************************************************************************/
#define ETM_COMBINE_FAULTEN_MASK  0x40       /*!< ���Ͽ���ʹ�� */
#define ETM_COMBINE_SYNCEN_MASK   0x20       /*!< ͬ��ʹ�� */
#define ETM_COMBINE_DTEN_MASK     0x10       /*!< ����ʱ��ʹ�� */
#define ETM_COMBINE_DECAP_MASK    0x08       /*!< ˫���ز���״̬λ */
#define ETM_COMBINE_DECAPEN_MASK  0x04       /*!< ˫���ز���ʹ��λ */
#define ETM_COMBINE_COMP_MASK     0x02       /*!< ͨ������ʹ�� */
#define ETM_COMBINE_COMBINE_MASK  0x01       /*!< ͨ�����ʹ�� */
/*! @}                                                                        */

/******************************************************************************
* ETM ʱ��Դ����Ƶϵ������
*
*//*!
* @{
*******************************************************************************/
#define ETM_CLOCK_NOCLOCK             0                 /*!< ���ü����� */
#define ETM_CLOCK_SYSTEMCLOCK         1                 /*!< ϵͳʱ�� */
#define ETM_CLOCK_FIXEDFREQCLOCK      2                 /*!< �̶�Ƶ��ʱ�� */
#define ETM_CLOCK_EXTERNALCLOCK       3                 /*!< ���ʱ�� */

/* ʱ�ӷ�Ƶϵ�� */
#define ETM_CLOCK_PS_DIV1             0                 /*!< 1��Ƶ */
#define ETM_CLOCK_PS_DIV2             1                 /*!< 2��Ƶ */
#define ETM_CLOCK_PS_DIV4             2                 /*!< 4��Ƶ */
#define ETM_CLOCK_PS_DIV8             3                 /*!< 8��Ƶ */
#define ETM_CLOCK_PS_DIV16            4                 /*!< 16��Ƶ */
#define ETM_CLOCK_PS_DIV32            5                 /*!< 32��Ƶ */
#define ETM_CLOCK_PS_DIV64            6                 /*!< 64��Ƶ */
#define ETM_CLOCK_PS_DIV128           7                 /*!< 128��Ƶ */
/*! @}                                                                        */

/******************************************************************************
* ETM ����ʱ��Ԥ��Ƶֵ
*
*//*! @����ʱ��Ԥ��Ƶֵ
* @{
*******************************************************************************/
/*  */
#define ETM_DEADETME_DTPS_DIV1        0                 /*!< 1��Ƶ */
#define ETM_DEADETME_DTPS_DIV4        2                 /*!< 4��Ƶ */
#define ETM_DEADETME_DTPS_DIV16       3                 /*!< 16��Ƶ */
/*! @}                                                                        */

/******************************************************************************
* ETM ���ģʽ����
*
*//*!
* @{
*******************************************************************************/
/* output mode */
#define ETM_OUTPUT_TOGGLE   1                    /*!< ƥ��ʱ�л���� */
#define ETM_OUTPUT_CLEAR    2                    /*!< ƥ��ʱ������� */
#define ETM_OUTPUT_SET      3                    /*!< ƥ��ʱ��λ��� */
/*! @}                                                                        */
      
/******************************************************************************
* ETM ���벶׽���ض���
*
*//*! 
* @{
*******************************************************************************/

#define ETM_INPUTCAPTURE_RISINGEDGE           1     /*!< ������ */
#define ETM_INPUTCAPTURE_FALLINGEDGE          2     /*!< �½��� */
#define ETM_INPUTCAPTURE_BOTHEDGE             3     /*!< ˫���� */

#define ETM_INPUTCAPTURE_DUALEDGE_NOEDGE      0     /*!< �� */
#define ETM_INPUTCAPTURE_DUALEDGE_RISINGEDGE  1     /*!< ������ */
#define ETM_INPUTCAPTURE_DUALEDGE_FALLInGEDGE 2     /*!< �½��� */
#define ETM_INPUTCAPTURE_DUALEDGE_BOTHEDGE    3     /*!< ˫���� */
/*! @}                                                                        */

/******************************************************************************
* ETM ˫�ز�׽ģʽ����
*
*//*! 
* @{
*******************************************************************************/
#define ETM_INPUTCAPTURE_DUALEDGE_ONESHOT     4  /*!< ���β�׽ģʽ */ 
#define ETM_INPUTCAPTURE_DUALEDGE_CONTINUOUS  5  /*!< ������׽ģʽ */
/*! @}                                                                        */

/******************************************************************************
* ETM ����PWMģʽ����
*
*//*! 
* @{
*******************************************************************************/
#define ETM_PWM_HIGHTRUEPULSE        1            /*!< �������� */
#define ETM_PWM_LOWTRUEPULSE         2            /*!< �������� */
/*! @}                                                                        */

/******************************************************************************
* ETM ͬ������Դ����
*
*//*!  
* @{
*******************************************************************************/
#define ETM_SYNC_TRIGGER_SOFTWARE    1        /*!< ���ͬ�� */
#define ETM_SYNC_TRIGGER_TRIGGER2    2        /*!< Ӳ������2ͬ��, SIM_SOPT[ETMSYNC] */
#define ETM_SYNC_TRIGGER_TRIGGER1    3        /*!< Ӳ������1ͬ��, ETM0CH0 */
#define ETM_SYNC_TRIGGER_TRIGGER0    4        /*!< Ӳ������0ͬ��, ACMP0 */
/*! @}                                                                        */

/******************************************************************************
* ETM ����������ֵ
*
*//*!  
* @{
*******************************************************************************/ 
#define ETM_SWOCTRL_HIGH    1                /*!< �������ߵ�ƽ */
#define ETM_SWOCTRL_LOW     0                /*!< �������͵�ƽ */
/*! @}                                                                        */

/******************************************************************************
* ETM  ͨ����������
*
*//*! 
* @{
*******************************************************************************/
#define ETM_POLARITY_HIGHACTIVE     0        /*!< ͨ�����Ըߵ�ƽ��Ч */
#define ETM_POLARITY_LOWACTIVE      1        /*!< ͨ�����Ե͵�ƽ��Ч */
/*! @}                                                                        */


/******************************************************************************
******************************************************************************/
/*! @brief ETM_CALLBACK ETM�ص����Ͷ���                                */
typedef void (*ETM_CallbackPtr)(void);
/*! @}                                                                        */

/******************************************************************************
* ETM �������ýṹ��.
*
*//*! 
* @{
*******************************************************************************/
/*!
* @brief ETM �������ýṹ��.
*
*/

typedef struct
{
  uint8_t   clk_source;       /*!< ʱ��Դ */
  uint8_t   prescaler;        /*!< ʱ�ӷ�Ƶϵ�� */
  uint8_t   sc;               /*!< ״̬�Ϳ��� */
  uint16_t  modulo;           /*!< ����ģֵ */
  uint16_t  cnt;              /*!< ����ֵ */
  uint16_t  cntin;            /*!< ������ʼֵ */
  uint8_t   mode;             /*!< ����ģʽѡ�� */
  uint8_t   sync;             /*!< ͬ��ģʽ */
  uint8_t   outinit;          /*!< ͨ�������ʼ����  */
  uint8_t   outmask;          /*!< ������� */
  uint32_t  combine;          /*!< ͨ������ */
  uint16_t  deadETMe;         /*!< ����ʱ�������� */
  uint8_t   exttrig;          /*!< �ⲿ���� */
  uint8_t   pol;              /*!< ͨ������ */
  uint16_t  filter;           /*!< �����˲����� */
  uint8_t   fms;              /*!< ����ģʽ״̬ */
  uint16_t  fltctrl;          /*!< ���Ͽ��� */
  uint8_t   fltpol;           /*!< �������뼫�� */
  uint16_t  conf;             /*!< ETM���� */
  uint32_t  synconf;          /*!< ͬ������ */
  uint8_t   invctrl;          /*!< ������� */
  uint16_t  swoctrl;          /*!< ���������� */
  uint16_t  pwmload;          /*!< PWMװ�� */
} ETM_ConfigType, *ETM_ConfigPtr;
/*! @}                                                                       */

/******************************************************************************
* ETM ͨ�����ýṹ��.
*
*//*!
* @{
*******************************************************************************/
/*!
* @brief ETM ͨ�����ýṹ��.
*
*/
typedef struct
{
  uint8_t         u8CnSC;                  /*!< ETM ͨ��״̬�Ϳ��� */
  uint16_t        u16CnV;                  /*!< ETM ͨ��ֵ���� */
  union
  {
    uint32_t      u32dw;
    struct 
    {
      uint32_t    bMode         : 3;        /*!< ETMģʽѡ��: GPIO, ���벶��, ����Ƚ�, ����PWM, �������PWM,
                                             * ����PWM, ˫���ز��� 
                                             */
      uint32_t    bEdge         : 2;        /*!< ����ѡ�� */
      uint32_t    bOutCmp       : 2;        /*!< ��ת, ����, ��λ */
      uint32_t    bPWMPol       : 1;        /*!< ��������, �������� */
      uint32_t    bDualCapMode  : 1;        /*!< ˫���ز�׽ģʽ: ����, ����ģʽ */
      uint32_t    bCHIE         : 1;        /*!< ʹ��ͨ���ж� */
    }bits;
  }ctrl;                                    /*!< ETM ͨ�����Կ��� */
} ETM_ChParamsType;

/*! @}                                                                        */

/*******************************************************************************/
/*!
 *  ��������
 */
/*******************************************************************************/

/*****************************************************************************//*!
*
* @brief  ʹ��ETM��ʱ������ж�.
*        
* @param[in]    pETM          ָ������ETM��ʱ������һ���Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_EnableOverflowInt(ETM_Type *pETM)
{
    if(pETM->SC & ETM_SC_TOF_MASK)
    {
        pETM->SC &= ~ETM_SC_TOF_MASK;
    }
    pETM->SC |= ETM_SC_TOIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ����ETM��ʱ������ж�.
*        
* @param[in]    pETM    ָ������ETM��ʱ������һ���Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_DisableOverflowInt(ETM_Type *pETM)
{
    pETM->SC &= ~ETM_SC_TOIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ʹ��ͨ���ж�.
*        
* @param[in]    pETM            ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    u8ETM_Channel   ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_EnableChannelInt(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    pETM->CONTROLS[u8ETM_Channel].CnSC |= ETM_CnSC_CHIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ��ֹͨ���ж�.
*        
* @param[in]    pETM            ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    u8ETM_Channel   ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_DisableChannelInt(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    pETM->CONTROLS[u8ETM_Channel].CnSC &= ~ETM_CnSC_CHIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ��ȡ�����־λ.
*        
* @param[in]    pETM           ָ������ETM��ʱ������һ���Ļ�ַ.
*    
* @return none.
*
*****************************************************************************/
__STATIC_INLINE uint8_t ETM_GetOverFlowFlag(ETM_Type *pETM)
{
    return (pETM->SC & ETM_SC_TOF_MASK);
}

/*****************************************************************************//*!
*
* @brief  ��������־λ.
*        
* @param[in]    pETM           ָ������ETM��ʱ������һ���Ļ�ַ.
*    
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_ClrOverFlowFlag(ETM_Type *pETM)
{
    if(pETM->SC & ETM_SC_TOF_MASK)
    {
         pETM->SC &= ~ETM_SC_TOF_MASK;
    }
}

/*****************************************************************************//*!
*
* @brief  ��ȡͨ���¼���־λ.
*        
* @param[in]    pETM            ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    u8ETM_Channel   ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE uint8_t ETM_GetChannelFlag(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    return (pETM->CONTROLS[u8ETM_Channel].CnSC & ETM_CnSC_CHF_MASK);
}

/*****************************************************************************//*!
*
* @brief  ���ͨ���¼���־λ.
*        
* @param[in]    pETM           ָ������ETM��ʱ������һ���Ļ�ַ.
*    
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_ClrChannelFlag(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    pETM->CONTROLS[u8ETM_Channel].CnSC &= ~ETM_CnSC_CHF_MASK;
}

/*********************************************************************************//*!
*
* @brief ʹ��д��������.��д��������λ����д��(ETM2). 
*        
* @param[in]    pETM             ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_WriteProtectionEnable(ETM_Type *pETM)
{
    pETM->FMS |= ETM_FMS_WPEN_MASK;
}

/*********************************************************************************//*!
*
* @brief ����д����λ.��д��������λ��д(ETM2). 
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_WriteProtectionDisable(ETM_Type *pETM)
{
    if (pETM->FMS & ETM_FMS_WPEN_MASK)
    {
        pETM->MODE |= ETM_MODE_WPDIS_MASK;
    }
}

/*****************************************************************************//*!
*
* @brief  ��λ ETMEN ȥʹ��ETM��һЩ���⹦�ܼĴ���(ETM2).
*        
* @param[in]    pETM             ETM2.
*    
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_SetETMEnhanced(ETM_Type *pETM)
{
    if(pETM->MODE & ETM_MODE_WPDIS_MASK)   /* ǰ����û�н���д���� */
    {
        pETM->MODE |= ETM_MODE_ETMEN_MASK;
    }
    else
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->MODE |= ETM_MODE_ETMEN_MASK;
        ETM_WriteProtectionEnable(pETM);
    }    
}

/*****************************************************************************//*!
*
* @brief  ��� ETMEN ȥ����һЩ���⹦�ܼĴ���, ֻ��һЩ����ģ�����ʹ��(ETM2).       
* @param[in]    pETM           ETM2.
*    
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_SetETMBasic(ETM_Type *pETM)
{
    if(pETM->MODE & ETM_MODE_WPDIS_MASK)    /* ǰ����Ҫû�н���д���� */
    {
        pETM->MODE &= ~ETM_MODE_ETMEN_MASK;
    }
    else
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->MODE &= ~ETM_MODE_ETMEN_MASK;
        ETM_WriteProtectionEnable(pETM);
    }  
}

/*****************************************************************************//*!
*
* @brief  ���� ETM ģֵ(ETM2).
*              
* @param[in]    pETM           (ETM2).
* @param[in]    u16ModValue    ��Ҫ���õ�16λģֵ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_SetModValue(ETM_Type *pETM, uint16_t u16ModValue)
{
    pETM->CNT = 0;
    pETM->MOD = u16ModValue;
    if(ETM2 == pETM)
    {
        if(pETM->MODE & ETM_MODE_ETMEN_MASK)
        {
            pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK;
        }
        else
        {
        }
    }
    else
    {
    }
}

/*****************************************************************************//*!
*
* @brief  ���� ETM ͨ��ֵ(ETM2).
*                
* @param[in]    pETM               ETM2.
* @param[in]    u16ChannelValue    ��Ҫ���õ�16λͨ��ֵ.
* @param[in]    u8ETM_Channel      ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_SetChannelValue(ETM_Type *pETM, uint8_t u8ETM_Channel, uint16_t u16ChannelValue)
{
    pETM->CONTROLS[u8ETM_Channel].CnV = u16ChannelValue;
    if(ETM2 == pETM)
    {
        if(pETM->MODE & ETM_MODE_ETMEN_MASK)
        {
            if(u8ETM_Channel < 2)
            {
                pETM->COMBINE |= ETM_COMBINE_SYNCEN0_MASK; 
            }
            else if (u8ETM_Channel < 4)
            {
                pETM->COMBINE |= ETM_COMBINE_SYNCEN1_MASK;
            }
            else
            {
                pETM->COMBINE |= ETM_COMBINE_SYNCEN2_MASK;
            }
            pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK;  
        }
        else
        {
        }
    }
    else
    {
    }
}

/*****************************************************************************//*!
*
* @brief  ����ETM�ļ�����ʼֵ(ETM2).
*               
* @param[in]    pETM               ETM2.
* @param[in]    u16CounterValue    ��Ҫ���õ�16λ������ʼֵ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_SetCounterInitValue(ETM_Type *pETM, uint16_t u16CounterValue)
{
    pETM->CNTIN = u16CounterValue;
    if(pETM->MODE & ETM_MODE_ETMEN_MASK)
    {
        pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK;
    }
    else
    {
    }
}

/*****************************************************************************//*!
*
* @brief  ����������μĴ�����ֵ(ETM2).
*               
* @param[in]    pETM            ETM2.
* @param[in]    u16ChMask       ���ε�ͨ��ֵ��6λ����ӦETM2��6��ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_MaskChannels(ETM_Type *pETM, uint16_t u16ChMask)
{
     pETM->OUTMASK |= u16ChMask;
}

/*****************************************************************************//*!
*
* @brief  ���������μĴ�����ֵ(ETM2).
*               
* @param[in]    pETM            ETM2.
* @param[in]    u16ChMask       ��Ҫ������ε�ͨ��ֵ��6λ����ӦETM2��6��ͨ����.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void ETM_UnMaskChannels(ETM_Type *pETM, uint16_t u16ChMask)
{
     pETM->OUTMASK &= ~u16ChMask;
}

/*********************************************************************************//*!
*
* @brief ���� ETM ͨ������(ETM2).
*        
* @param[in]    pETM                 ETM2.
* @param[in]    u8ChsPolValue        ��Ҫ���õ�ͨ������ֵ��6λ����ӦETM2��6��ͨ��.
*                                    0Ϊ�ߵ�ƽ��Ч��1Ϊ�͵�ƽ��Ч
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_SetChannelsPolarity(ETM_Type *pETM, uint8_t u8ChsPolValue)
{
    pETM->POL = u8ChsPolValue;
}

/*********************************************************************************//*!
*
* @brief ��ȡ ETM ��ͨ������ֵ(ETM2).
*        
* @param[in]    pETM                  ETM2.
*
* @return ͨ������ֵ.
*
*********************************************************************************/
__STATIC_INLINE uint8_t ETM_GetChannelsPolarity(ETM_Type *pETM)
{
    return (pETM->POL);
}

/*********************************************************************************//*!
*
* @brief ������ǿPWMͬ��(ETM2).
*        
* @param[in]    pETM                 ETM2.
* 
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_EnableEnhancedSYNCMode(ETM_Type *pETM)
{
    pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   
}

/*********************************************************************************//*!
*
* @brief ���ô�ͳPWMͬ��(ETM2).
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_DisableEnhancedSYNCMode(ETM_Type *pETM)
{
    pETM->SYNCONF   &= ~ETM_SYNCONF_SYNCMODE_MASK;   
}

/*********************************************************************************//*!
*
* @brief ����ͨ���ⲿ����(ETM2).
*        
* @param[in]    pETM               ETM2.
* @param[in]    u8TirggerSource    ����0-5��6��ͨ���Ĵ���
*                                  0����ֹͨ������������1��ʹ��ͨ����������
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_SetExternalTrigger(ETM_Type *pETM, uint8_t u8TirggerSource)
{
    pETM->EXTTRIG   = u8TirggerSource;
}

/*********************************************************************************//*!
*
* @brief ��ȡ�ⲿ������־.
*        
* @param[in]    pETM              ETM2.
*
* @return ͨ��������־.
*
*********************************************************************************/
__STATIC_INLINE uint8_t ETM_GetExternalTriggerFlag(ETM_Type *pETM)
{
    return (pETM->EXTTRIG & ETM_EXTTRIG_TRIGF_MASK);
}

/*********************************************************************************//*!
*
* @brief ���ü���ʹ��λ(ETM2).
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_SetLoadEnable(ETM_Type *pETM)
{
    pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK;
}

/*********************************************************************************//*!
*
* @brief ����ƥ������а�����ͨ��.
*        
* @param[in]    pETM               ETM2.
* @param[in]    u8Matchchannel     ƥ���������������ͨ��.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_SetLoadMatchChannel(ETM_Type *pETM, uint8_t u8Matchchannel)
{
    pETM->PWMLOAD |= u8Matchchannel;
}

/*********************************************************************************//*!
*
* @brief ����ͨ�������˲���(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8ETM_Channel     ͨ���ţ�ֻ��ͨ��0-ͨ��3������.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_InputCaptureFilterClr(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    pETM->FILTER &= ~(0x000F << (u8ETM_Channel << 2));
}

/*********************************************************************************//*!
*
* @brief ʹ��ͨ���˲���(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8ETM_Channel     ͨ���ţ�ֻ��ͨ��0-ͨ��3������.
* @param[in]    u8FilterValue     �˲�����ֵ:1~15, 0: ����ͨ���˲���.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_InputCaptureFilterSet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8FilterValue)
{
    if (u8FilterValue)
    {
        pETM->FILTER |= (u8FilterValue << (u8ETM_Channel << 2));
    }
    else
    {
        ETM_InputCaptureFilterClr(pETM, u8ETM_Channel);
    }
}


/*********************************************************************************//*!
*
* @brief ʹ�ܹ�������(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FaultPin        ��������ͨ����: 0~3.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinEnable(ETM_Type *pETM, uint8_t u8FaultPin)
{
    if (pETM->MODE & ETM_MODE_WPDIS_MASK) 
    {
        pETM->FLTCTRL |= (1 << u8FaultPin);
    }
    else                                 
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->FLTCTRL |= (1 << u8FaultPin);
        ETM_WriteProtectionEnable(pETM);
    }
}

/*********************************************************************************//*!
*
* @brief ���ù�������(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FaultPin        ��������ͨ����: 0~3.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinDisable(ETM_Type *pETM, uint8_t u8FaultPin)
{
    if (pETM->MODE & ETM_MODE_WPDIS_MASK) 
    {
        pETM->FLTCTRL &= ~(1 << u8FaultPin);
    }
    else                              
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->FLTCTRL &= ~(1 << u8FaultPin);
        ETM_WriteProtectionEnable(pETM);
    }
}

/*********************************************************************************//*!
*
* @brief ʹ�ܹ��������˲���(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FaultPin        ��������ͨ����: 0~3.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinFilterEnable(ETM_Type *pETM, uint8_t u8FaultPin)
{
    if (pETM->MODE & ETM_MODE_WPDIS_MASK) 
    {
        pETM->FLTCTRL |= (0x10 << u8FaultPin);
    }
    else                               
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->FLTCTRL |= (0x10 << u8FaultPin);
        ETM_WriteProtectionEnable(pETM);
    }
}

/*********************************************************************************//*!
*
* @brief ���ù��������˲���(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FaultPin        ��������ͨ����: 0~3.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinFilterDisable(ETM_Type *pETM, uint8_t u8FaultPin)
{
    if (pETM->MODE & ETM_MODE_WPDIS_MASK) 
    {
        pETM->FLTCTRL &= ~(0x10 << u8FaultPin);
    }
    else                           
    {
        ETM_WriteProtectionDisable(pETM);
        pETM->FLTCTRL &= ~(0x10 << u8FaultPin);
        ETM_WriteProtectionEnable(pETM);
    }
}

/*********************************************************************************//*!
*
* @brief �������еĹ��������˲���(ETM2). 
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinFilterCDisableAll(ETM_Type *pETM)
{
    pETM->FLTCTRL &= ~ETM_FLTCTRL_FFVAL_MASK;
}

/*********************************************************************************//*!
*
* @brief ���ù��������˲���ֵ. ���еĹ���ͨ����Ϊ���ֵ(ETM2).
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FilterValue     ���������˲���ֵ: 1~15, 0 �����˲���.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE void ETM_FaultPinFilterSet(ETM_Type *pETM, uint8_t u8FilterValue)
{
    if (u8FilterValue)
    {
        pETM->FLTCTRL |= ETM_FLTCTRL_FFVAL(u8FilterValue);
    }
    else
    {
        ETM_FaultPinFilterCDisableAll(pETM);
    }
}

/*********************************************************************************//*!
*
* @brief ��ȡ����ͨ�����ϼ���־(ETM2)
*        
* @param[in]    pETM               ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE uint8_t ETM_GetFaultDetectionLogicORFlag(ETM_Type *pETM)
{
    return (pETM->FMS & ETM_FMS_FAULTF_MASK);
}

/*********************************************************************************//*!
*
* @brief ��ȡԤ��ͨ�����ϼ���־(ETM2)
*        
* @param[in]    pETM              ETM2.
* @param[in]    u8FaultPin        ��������ͨ����: 0~3.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE uint8_t ETM_GetFaultDetectionFlag(ETM_Type *pETM, uint8_t u8FaultPin)
{
    return (pETM->FMS & (1 << u8FaultPin));
}

/*********************************************************************************//*!
*
* @brief ��ȡ�����߼��Ĺ�������(ETM2)
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
__STATIC_INLINE uint8_t ETM_GetFaultInputsLogicORValue(ETM_Type *pETM)
{
    return (pETM->FMS & ETM_FMS_FAULTIN_MASK);
}

/*! @}                                                                        */

/******************************************************************************
******************************************************************************/

void ETM_ClockSet(ETM_Type *pETM, uint8_t u8ClockSource, uint8_t u8ClockPrescale);
void ETM_PWMInit(ETM_Type *pETM, uint8_t u8PWMModeSelect, uint8_t u8PWMEdgeSelect);
void ETM_InputCaptureInit(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8CaptureMode);
void ETM_DualEdgeCaptureInit(ETM_Type *pETM, uint8_t u8ChannelPair, uint8_t u8CaptureMode, 
                             uint8_t u8Channel_N_Edge, uint8_t u8Channel_Np1_Edge);
void ETM_OutputCompareInit(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8CompareMode);
void ETM_SoftwareSync(ETM_Type *pETM);
void ETM_HardwareSync(ETM_Type *pETM, uint8_t u8TriggerN);
void ETM_HardwareSyncCombine(ETM_Type *pETM, uint8_t u8TriggerMask);
void ETM_GenerateTrig2(ETM_Type *pETM);
void ETM_PWMDeadETMeSet(ETM_Type *pETM, uint8_t u8PrescalerValue, uint8_t u8DeadETMeValue);
void ETM_OutputMaskSet(ETM_Type *pETM, uint8_t u8ETM_Channel);
void ETM_SWOutputControlSet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8ChannelValue);
void ETM_SetDebugModeBehavior(ETM_Type *pETM, uint8_t u8DebugMode);
void ETM_SetTOFFrequency(ETM_Type *pETM, uint8_t u8TOFNUM);
void ETM_PolaritySet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8ActiveValue);
void ETM_InvertChannel(ETM_Type *pETM, uint8_t u8ChannelPair);
void ETM_Init(ETM_Type *pETM, ETM_ConfigType *pConfig);
void ETM_DeInit(ETM_Type *pETM);
void ETM_ChannelInit(ETM_Type *pETM, uint8_t u8ETM_Channel, ETM_ChParamsType *pETM_ChParams);
void ETM_SetDutyCycleCombine(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8DutyCycle);
void ETM_SetCallback(ETM_Type *pETM, ETM_CallbackPtr pfnCallback);
void  ETM_SyncConfigActivate(ETM_Type *pETM, uint32_t u32ConfigValue);
void ETM_SyncConfigDeactivate(ETM_Type * pETM, uint32_t u32ConfigValue);
uint8_t ETM_GetFaultDetectionLogicORFlag(ETM_Type *pETM);
uint8_t ETM_GetFaultDetectionFlag(ETM_Type *pETM, uint8_t u8FaultPin);
uint8_t ETM_GetFaultInputsLogicORValue(ETM_Type *pETM);
void ETM_WriteProtectionEnable(ETM_Type *pETM);
void ETM_WriteProtectionDisable(ETM_Type *pETM);
void ETM_FaultPinFilterCDisableAll(ETM_Type *pETM);
void ETM_FaultPinFilterSet(ETM_Type *pETM, uint8_t u8FilterValue);
void ETM_FaultPinFilterDisable(ETM_Type *pETM, uint8_t u8FaultPin);
void ETM_FaultPinFilterEnable(ETM_Type *pETM, uint8_t u8FaultPin);
void ETM_FaultPinEnable(ETM_Type *pETM, uint8_t u8FaultPin);
void ETM_FaultPinDisable(ETM_Type *pETM, uint8_t u8FaultPin);
void ETM_InputCaptureFilterClr(ETM_Type *pETM, uint8_t u8ETM_Channel);
void ETM_InputCaptureFilterSet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8FilterValue);
void ETM_SetLoadMatchChannel(ETM_Type *pETM, uint8_t u8Matchchannel);
void ETM_SetLoadEnable(ETM_Type *pETM);
uint8_t ETM_GetExternalTriggerFlag(ETM_Type *pETM);
void ETM_DisableEnhancedSYNCMode(ETM_Type *pETM);
void ETM_EnableEnhancedSYNCMode(ETM_Type *pETM);
uint8_t ETM_GetChannelsPolarity(ETM_Type *pETM);
void ETM_SetChannelsPolarity(ETM_Type *pETM, uint8_t u8ChsPolValue);
void ETM_UnMaskChannels(ETM_Type *pETM, uint16_t u16ChMask);
void ETM_MaskChannels(ETM_Type *pETM, uint16_t u16ChMask);
void ETM_SetCounterInitValue(ETM_Type *pETM, uint16_t u16CounterValue);
void ETM_SetChannelValue(ETM_Type *pETM, uint8_t u8ETM_Channel, uint16_t u16ChannelValue);
void ETM_SetModValue(ETM_Type *pETM, uint16_t u16ModValue);
void ETM_SetETMBasic(ETM_Type *pETM);
void ETM_SetETMEnhanced(ETM_Type *pETM);
void ETM_ClrChannelFlag(ETM_Type *pETM, uint8_t u8ETM_Channel);
uint8_t ETM_GetChannelFlag(ETM_Type *pETM, uint8_t u8ETM_Channel);
void ETM_ClrOverFlowFlag(ETM_Type *pETM);
uint8_t ETM_GetOverFlowFlag(ETM_Type *pETM);
void ETM_DisableChannelInt(ETM_Type *pETM, uint8_t u8ETM_Channel);
void ETM_EnableChannelInt(ETM_Type *pETM, uint8_t u8ETM_Channel);
void ETM_DisableOverflowInt(ETM_Type *pETM);
void ETM_EnableOverflowInt(ETM_Type *pETM);
void ETM_disblechannel(ETM_Type *pETM, uint8_t u8ETM_Channel);

#ifdef __cplusplus
}
#endif
#endif /* ETM_H_ */
