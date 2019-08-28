/******************************************************************************
*
* @brief  KBI ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef _KBI_H_
#define _KBI_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
/********************************************************!
*
* @brief KBIģ���ж������źż��ģʽѡ��
*
***********************************************************/
typedef enum
{
    KBI_MODE_EDGE_ONLY = 0,             /*!< ѡ��  ���ؼ��  */         
    KBI_MODE_EDGE_LEVEL                 /*!< ѡ��  ���غ͵�ƽ���*/
}KBI_ModeType;


typedef enum
{
    KBI_FALLING_EDGE_LOW_LEVEL = 0,     /*!< ѡ�� �½��ػ�͵�ƽ */
    KBI_RISING_EDGE_HIGH_LEVEL          /*!< ѡ�� �����ػ�ߵ�ƽ */
}KBI_EdgeType;


/******************************************************************************
* 
* ����KBIģ��������ж��������Ÿ���
*
*******************************************************************************/
#define KBI_MAX_NO              2                  /*!< KBIģ����� */

#if defined(CPU_NV32)|| defined(CPU_NV32M3)
   #define KBI_MAX_PINS_PER_PORT   8                  /*!< KBI�ж��������Ÿ��� */
#elif defined(CPU_NV32M4)
   #define KBI_MAX_PINS_PER_PORT   32                  /*!< KBI�ж��������Ÿ��� */
#endif


/******************************************************************************
* KBI�ص���������
******************************************************************************/
typedef void (*KBI_CallbackType)(void);                     




/******************************************************************************
* 
* KBI�������ýṹ��
*
*******************************************************************************/

typedef struct
{
    uint8_t     bEdge   : 1;                                /*!< ����/��ƽѡ��Ϊ*/
    uint8_t     bEn     : 1;                                /*!< ����ʹ��λ*/
    uint8_t     bRsvd   : 6;                                /*!< ���� */
} KBI_PinConfigType;

/******************************************************************************
*
* KBI���ýṹ��
*
*******************************************************************************/
/*!
 * @brief KBI״̬�Ϳ��ƼĴ����ṹ��.
 *
 */

typedef struct
{
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
    struct
    {
        uint8_t     bMode   : 1;                            /*!< ѡ��KBI���ģʽ */
        uint8_t     bIntEn  : 1;                            /*!< ʹ��KBI�ж�λ */
        uint8_t     bRsvd   : 6;                            /*!< ���� */
    } sBits;
#elif  defined(CPU_NV32M4)
    struct
    {
        uint32_t     bMode   : 1;                          /*!< ѡ��KBI���ģʽ */
        uint32_t     bIntEn  : 1;                          /*!< ʹ��KBI�ж�λ */
        uint32_t     bRsvd2  : 2;                          /*!< ���� */
        uint32_t     bKbspEn : 1;							            /*!<Real KBI_SP register enable*/
		uint32_t     bRstKbsp: 1;                             /*!<Reset KBI_SP register*/
        uint32_t     bRsvd26 : 26;                        /*!< reserved */
    } sBits;
#endif
    KBI_PinConfigType   sPin[KBI_MAX_PINS_PER_PORT];
} KBI_ConfigType, *KBI_ConfigTypePtr;


/*****************************************************************************//*!
*
* @brief ���ý��½��ؼ��.
*        
* @param[in]   pKBI          ָ��KBIģ��.
* @param[in]   PinMasks      KBI�ж��������ź�.
*
* @return none.
*
* @see KBI_DetectRisingEdge.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_DetectFallingEdge(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_DetectFallingEdge(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->SC &= ~KBI_SC_KBMOD_MASK;
    pKBI->ES &= ~(PinMasks);
}

/*****************************************************************************//*!
*
* @brief ���ý��ߵ�ƽ���
*        
* @param[in]   pKBI          ָ��KBIģ��.
* @param[in]   PinMasks      KBI�ж��������ź�.
*
* @return none.
*
* @see KBI_DetectFallingEdge.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_DetectRisingEdge(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_DetectRisingEdge(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->SC &= ~KBI_SC_KBMOD_MASK;
    pKBI->ES |= (PinMasks);    
}

/*****************************************************************************//*!
*
* @brief ���������غ͸ߵ�ƽ���
*        
* @param[in]   pKBI          ָ��KBIģ��.
* @param[in]   PinMasks      KBI�ж��������ź�.
*
* @return none.
*
* @see KBI_DetectFallingEdgeLowLevel.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_DetectRisingEdgeHighLevel(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_DetectRisingEdgeHighLevel(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->SC |= KBI_SC_KBMOD_MASK;
    pKBI->ES |= (PinMasks);    
}

/*****************************************************************************//*!
*
* @brief �����½��غ͵͵�ƽ���
*        
* @param[in]   pKBI          ָ��KBIģ��.
* @param[in]   PinMasks      KBI�ж��������ź�.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_DetectRisingEdgeHighLevel.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_DetectFallingEdgeLowLevel(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_DetectFallingEdgeLowLevel(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->SC |= KBI_SC_KBMOD_MASK;
    pKBI->ES &= ~(PinMasks);        
}

/*****************************************************************************//*!
*
* @brief ʹ��KBI�ж���������
*        
* @param[in]   pKBI          ָ��KBIģ��..
* @param[in]   PinMasks      KBI�ж��������ź�.
*
* @return none.
*
* @see KBI_Disable.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_Enable(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_Enable(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->PE |= (PinMasks);        
}

/*****************************************************************************//*!
*
* @brief ����KBI�ж���������.
*        
* @param[in]   pKBI         ָ��KBIģ��..
* @param[in]   PinMasks     KBI�ж��������ź�.
*
* @return none.
*
* @see KBI_Enable.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  void KBI_Disable(KBI_Type *pKBI, uint8_t PinMasks)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  void KBI_Disable(KBI_Type *pKBI, uint32_t PinMasks)
#endif
{
    pKBI->PE &= ~(PinMasks);        
}

/*****************************************************************************//*!
*
* @brief ʹ��KBI�ж�
*        
* @param[in]   pKBI          ָ��KBIģ��..
*          
* @return none.
*
*
* @see KBI_DisableInt.
*
*****************************************************************************/
__STATIC_INLINE  void KBI_EnableInt(KBI_Type *pKBI)
{
    pKBI->SC |= KBI_SC_KBIE_MASK;        
}


/*****************************************************************************//*!
*
* @brief ����KBI�ж�
*        
* @param[in]  pKBI          ָ��KBIģ��.
*          
* @return none.
*
*
* @see KBI_EnableInt.
*
*****************************************************************************/
__STATIC_INLINE  void KBI_DisableInt(KBI_Type *pKBI)
{
    pKBI->SC &= ~KBI_SC_KBIE_MASK;        
}

/*****************************************************************************//*!
*
* @brief ��ȡ�жϱ�־λ
*        
* @param[in]   pKBI         ָ��KBIģ��.
*          
* @return uint8_t.
*
* @see KBI_ClrFlags.
*
*****************************************************************************/
#if defined(CPU_NV32)|| defined(CPU_NV32M3)
__STATIC_INLINE  uint8_t KBI_GetFlags(KBI_Type *pKBI)
#elif defined(CPU_NV32M4)
__STATIC_INLINE  uint32_t KBI_GetFlags(KBI_Type *pKBI)
#endif
{
    return (pKBI->SC & KBI_SC_KBF_MASK);        
}

/*****************************************************************************//*!
*
* @brief ����жϱ�־λ.
*        
* @param[in]   pKBI          ָ��KBIģ��.
*          
* @return none.
*
*
* @see KBI_GetFlags.
*
*****************************************************************************/
__STATIC_INLINE  void KBI_ClrFlags(KBI_Type *pKBI)
{
    pKBI->SC |= KBI_SC_KBACK_MASK;        
}

#if defined(CPU_NV32M4)
/*****************************************************************************//*!
*
* @brief Real KBI_SP register enable.
*        
* @param[in]   pKBI         pointer to KBI module
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see The real ETMe value of Keyboard source pin to be read.
*
*****************************************************************************/
__STATIC_INLINE  void KBI_SPEnable(KBI_Type *pKBI)
{
    pKBI->SC |= KBI_SC_KBSPEN_MASK;        
}

/*****************************************************************************//*!
*
* @brief Get KBI source pin register fields.
*        
* @param[in]   pKBI         pointer to KBI module.
*          
* @return uint32_t.
*
* @ Pass/ Fail criteria: none.
*
* @see KBI_GetSP.
*
*****************************************************************************/
__STATIC_INLINE  uint32_t KBI_GetSP(KBI_Type *pKBI)
{
    return (pKBI->SP & KBI_SP_SP_MASK);        
}

/*****************************************************************************//*!
*
* @brief Reset KBI_SP register.
*        
* @param[in]   pKBI         pointer to KBI module
*          
* @return none.
*
* @ Pass/ Fail criteria: none
*
* @see KBI_RstSP.
*
*****************************************************************************/
__STATIC_INLINE  void KBI_RstSP(KBI_Type *pKBI)
{
    pKBI->SC |= KBI_SC_RSTKBSP_MASK;        
}
#endif


/******************************************************************************
* Global functions
******************************************************************************/

void KBI_Init(KBI_Type *pKBI, KBI_ConfigType *pConfig);
void KBI_SetCallback(KBI_Type *pKBI, KBI_CallbackType pfnCallback);

#ifdef __cplusplus
}
#endif
#endif 
