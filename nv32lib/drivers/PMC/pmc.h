/******************************************************************************
*
* @brief  PMC ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef PMC_H_
#define PMC_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
/******************************************************************************
* PCMϵͳ��MCU����ģʽ��ģʽ����
*
*******************************************************************************/
#define PmcModeRun     0                     /*!< ����ģʽ */
#define PmcModeWait    1                     /*!< �ȵ�ģʽ */
#define PmcModeStop4   2                     /*!< ֹͣģʽ4 */
#define PmcModeStop3   3                     /*!< ֹͣģʽ3 */


/******************************************************************************
* PMC ��ѹ���͵�ѹ������ѹ����
*
*******************************************************************************/
#define PmcLVDTrip_Low   0                     /*!< LVD �͵�ƽ����� */
#define PmcLVDTrip_High  1                     /*!< LVD �ߵ�ƽ����� */

#define PmcLVWTrip_Low   0                     /*!< LVW �͵�ƽ����� */
#define PmcLVWTrip_Mid1  1                     /*!< LVW �м��ƽ1����� */
#define PmcLVWTrip_Mid2  2                     /*!< LVW �м��ƽ2�����*/
#define PmcLVWTrip_High  3                     /*!< LVW �ߵ�ƽ�����*/

/******************************************************************************
* PMC ���ƽṹ��
*******************************************************************************/
/*!
 * @brief PMC���ƽṹ������
 *
 */

typedef union 
{
   uint8_t byte;						  /*!< �����������-�ֽ�*/
   struct 
   {     
       uint8_t bBandgapEn        :1;      /*!< ʹ�ܴ�϶������ */
       uint8_t bBandgapDrv       :1;      /*!< ѡ���϶����*/
       uint8_t bLvdEn            :1;      /*!< ʹ�ܵ�ѹ���*/
       uint8_t bLvdStopEn        :1;      /*!< ��ѹ�����ֹͣģʽ��ʹ��*/
       uint8_t bLvdRstEn         :1;      /*!< ʹ�ܵ�ѹ��⸴λ */
       uint8_t bLvwIrqEn         :1;      /*!< ʹ�ܵ�ѹ�����ж� */
       uint8_t bLvwAck           :1;      /*!< ��ѹ����Ӧ�� */
       uint8_t bLvwFlag          :1;      /*!< ��ѹ������־ */
   }bits; 								                /*!< ����������-λ */
}PMC_Ctrl1Type, *PMC_Ctrl1Ptr;		  /*!< PMC ���ƼĴ���1�ṹ�� */


/******************************************************************************
* PMC ����-- ��ѹѡ��.
*******************************************************************************/
/*!
 * @brief PMC ����-- ��ѹ����ѡ��.
 *
 */
typedef union 
{
   uint8_t byte;						   /*!< ����������-�ֽ� */
   struct 
   {     
       uint8_t           :4;               /*!< none */  
       uint8_t bLVWV     :2;               /*!< ��ѹ������ѹѡ�� */  
       uint8_t bLVDV     :1;               /*!< ��ѹ��λ��ѹѡ�� */  
       uint8_t           :1;               /*!< none */                 
   }bits;  								   /*!< �ṹ�����͡���λ */
}PMC_Ctrl2Type, *PMC_Ctrl2Ptr;		   /*!< PMC ���ƼĴ���2�ṹ��*/

/******************************************************************************
* PMC ���ýṹ��
*******************************************************************************/

typedef struct 
{
    PMC_Ctrl1Type    sCtrlstatus;          /*!< PMC ���ƺ�״̬ */  
    PMC_Ctrl2Type    sDetectVoltSelect;    /*!< ��ѹ������ѹ�͵�ѹ��λ��ѹѡ��*/  
}PMC_ConfigType, *PMC_ConfigPtr;		   /*!< PMC ���ýṹ�� */

/*****************************************************************************//*!
*
* @brief  ��ֹͣģʽ��ʹ�ܵ�ѹ���.
* 
* @param[in]  pPMC              ָ��PMCģ��
*
* @return none.
*
* @see PMC_DisableLVDInStopMode.
*
*****************************************************************************/
__STATIC_INLINE void PMC_EnableLVDInStopMode(PMC_Type *pPMC)
{
    pPMC->SPMSC1 |= PMC_SPMSC1_LVDSE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ��ֹͣģʽ�½��õ�ѹ���
* 
* @param[in]  pPMC           ָ��PMCģ��
*
* @return none.
*
* @see PMC_EnableLVDInStopMode.
*
*****************************************************************************/
__STATIC_INLINE void PMC_DisableLVDInStopMode(PMC_Type *pPMC)
{
    pPMC->SPMSC1 &= ~PMC_SPMSC1_LVDSE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ʹ�ܵ�ѹ��⸴λ  ע: ��λ����ֶν���дһ��
* 
* @param[in]  pPMC           ָ��PMCģ��.
*
* @return none.
*
* @see PMC_DisableLVDRst.
*
*****************************************************************************/
__STATIC_INLINE void PMC_EnableLVDRst(PMC_Type *pPMC)
{
    pPMC->SPMSC1 |= PMC_SPMSC1_LVDRE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ���õ�ѹ��⸴λ   ע: ��λ����ֶν���дһ��
* 
* @param[in]  pPMC             ָ��PMCģ��
*
* @return none.
*
* @see PMC_EnableLVDRst.
*
*****************************************************************************/
__STATIC_INLINE void PMC_DisableLVDRst(PMC_Type *pPMC)
{
    pPMC->SPMSC1 &= ~PMC_SPMSC1_LVDRE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ʹ�ܵ�ѹ���,  ע: ��λ����ֶν���дһ��
* 
* @param[in]  pPMC            ָ��PMCģ��
*
* @return none.
*
* @see PMC_DisableLVD.
*
*****************************************************************************/
__STATIC_INLINE void PMC_EnableLVD(PMC_Type *pPMC)
{
    pPMC->SPMSC1 |= PMC_SPMSC1_LVDE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ���õ�ѹ���,   ע: ��λ����ֶν���дһ��
* 
* @param[in]  pPMC           ָ��PMCģ��
*
* @return none.
*
* @see PMC_EnableLVD.
*
*****************************************************************************/
__STATIC_INLINE void PMC_DisableLVD(PMC_Type *pPMC)
{
    pPMC->SPMSC1 &= ~PMC_SPMSC1_LVDE_MASK;
}

/*****************************************************************************//*!
*
* @brief   ���õ�ѹ�������㣬
* 
* @param[in]  pPMC              ָ��PMCģ��
* @param[in]  Trippoint         ��ѹ��������ѡ��1-�ߵ�ƽ���� 0-�͵�ƽ����
*
* @return none.
*
* @see PMC_SetLVWTripVolt.
*
*****************************************************************************/
__STATIC_INLINE void PMC_SetLVDTripVolt(PMC_Type *pPMC, uint8_t Trippoint)
{
    if(Trippoint)
        pPMC->SPMSC2 |= PMC_SPMSC2_LVDV_MASK;
    else
        pPMC->SPMSC2 &= ~PMC_SPMSC2_LVDV_MASK;
}

/*****************************************************************************//*!
*
* @brief  ѡ���ѹ����������ѹ
* 
* @param[in]  pPMC              ָ��PMCģ��
* @param[in]  Trippoint         ��ѹ����������ѹ  0 �͵�ƽ����     1 �м��ƽ1���� 
*                                                   2 �м��ƽ2����  3 �ߵ�ƽ����
*
* @return none.
*
* @see PMC_SetLVDTripVolt.
*
*****************************************************************************/
__STATIC_INLINE void PMC_SetLVWTripVolt(PMC_Type *pPMC, uint8_t Trippoint)
{
    pPMC->SPMSC2 &= ~PMC_SPMSC2_LVWV_MASK;
    pPMC->SPMSC2 |= PMC_SPMSC2_LVWV(Trippoint);
}

/*****************************************************************************//*!
*
* @brief  ʹ�ܵ�ѹ�����ж�
* 
* @param[in]  pPMC        ָ��PMCģ��
*
* @return none.
*
* @see PMC_DisableLVWInterrupt.
*
*****************************************************************************/
__STATIC_INLINE void PMC_EnableLVWInterrupt(PMC_Type *pPMC)
{
   pPMC->SPMSC1 |= PMC_SPMSC1_LVWIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ���õ�ѹ�����ж�
* 
* @param[in]  pPMC              ָ��PMCģ��.
*
* @return none.
*
*
* @see PMC_EnableLVWInterrupt.
*
*****************************************************************************/
__STATIC_INLINE void PMC_DisableLVWInterrupt(PMC_Type *pPMC)
{
   pPMC->SPMSC1 &= ~PMC_SPMSC1_LVWIE_MASK;
}

/*****************************************************************************//*!
*
* @brief  ��ȡ��ѹ������־λ.
* 
* @param[in]  pPMC           ָ��PMCģ��.
*
* @return  ��ѹ������־λֵ
*
* @see PMC_ClrLVWFlag.
*
*****************************************************************************/
__STATIC_INLINE uint8_t PMC_GetLVWFlag(PMC_Type *pPMC)
{
   return (pPMC->SPMSC1 & PMC_SPMSC1_LVWF_MASK);
}

/*****************************************************************************//*!
*
* @brief  �����ѹ������־λ
* 
* @param[in]  pPMC            ָ��PMCģ��.
*
* @return none.
*
* @see PMC_GetLVWFlag.
*
*****************************************************************************/
__STATIC_INLINE void PMC_ClrLVWFlag(PMC_Type *pPMC)
{
   pPMC->SPMSC1 |= PMC_SPMSC1_LVWACK_MASK;
}  

/**************************************************************************/

void PMC_Init(PMC_Type *pPMC, PMC_ConfigType *pPMC_Config);
void PMC_DeInit(PMC_Type *pPMC);
void PMC_SetMode(PMC_Type *pPMC,uint8_t u8PmcMode);

#ifdef __cplusplus
}
#endif
#endif 
