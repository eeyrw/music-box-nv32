/******************************************************************************
*
* @brief  ADC ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef ADC_H_
#define ADC_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/******************************************************************************
 *
 *����ADC�ο���ѹ
 *
*******************************************************************************/
#define ADC_VREF_VREFH                 0x00         /*!< ADC �ο���ѹ VREFH*/
#define ADC_VREF_VDDA                  0x01         /*!< ADC �ο���ѹ VDDA*/

/******************************************************************************
 *
 * ����ADCʱ��Դ
 *
*******************************************************************************/
#define CLOCK_SOURCE_BUS_CLOCK                          0x00    /*!< ADCʱ��Դѡ������ʱ��*/
#define CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2                 0x01    /*!< ADCʱ��Դѡ������ʱ��2��Ƶ*/
#define CLOCK_SOURCE_ALTCLK                               0x02  /*!< ADCʱ��Դѡ����ʱ��*/
#define CLOCK_SOURCE_ADACK                                0x03  /*!< ADCʱ��Դѡ���첽ʱ��*/

/******************************************************************************
 *
 * ����ADCԴ��Ƶϵ��
 *
*******************************************************************************/
#define ADC_ADIV_DIVIDE_1                               0x00        /*!< ADCʱ��Դ��Ƶϵ��Ϊ1*/
#define ADC_ADIV_DIVIDE_2                               0x01        /*!< ADCʱ��Դ��Ƶϵ��Ϊ2*/
#define ADC_ADIV_DIVIDE_4                               0x02        /*!< ADCʱ��Դ��Ƶϵ��Ϊ4*/
#define ADC_ADIV_DIVIDE_8                               0x03        /*!< ADCʱ��Դ��Ƶϵ��Ϊ8*/

/******************************************************************************
 *
 * ����ADCת��ģʽ
 *
*******************************************************************************/
#define ADC_MODE_8BIT                                0x00       /*!< 8λת��*/
#define ADC_MODE_10BIT                               0x01       /*!< 10λת��*/
#define ADC_MODE_12BIT                               0x02       /*!< 12λת��*/

/*****************************************************************************
 *
 * ����ADC����ͨ��
 *
*******************************************************************************/
#define ADC_CHANNEL_AD0                                 0x0  /*!< ADC����ͨ��0*/
#define ADC_CHANNEL_AD1                                 0x1  /*!< ADC����ͨ��1*/
#define ADC_CHANNEL_AD2                                 0x2  /*!< ADC����ͨ��2*/
#define ADC_CHANNEL_AD3                                 0x3  /*!< ADC����ͨ��3*/
#define ADC_CHANNEL_AD4                                 0x4  /*!< ADC����ͨ��4*/
#define ADC_CHANNEL_AD5                                 0x5  /*!< ADC����ͨ��5*/
#define ADC_CHANNEL_AD6                                 0x6  /*!< ADC����ͨ��6*/
#define ADC_CHANNEL_AD7                                 0x7  /*!< ADC����ͨ��7*/
#define ADC_CHANNEL_AD8                                 0x8  /*!< ADC����ͨ��8*/
#define ADC_CHANNEL_AD9                                 0x9  /*!< ADC����ͨ��9*/
#define ADC_CHANNEL_AD10                                0xa  /*!< ADC����ͨ��10*/
#define ADC_CHANNEL_AD11                                0xb  /*!< ADC����ͨ��11*/
#define ADC_CHANNEL_AD12                                0xc  /*!< ADC����ͨ��12*/
#define ADC_CHANNEL_AD13                                0xd  /*!< ADC����ͨ��13*/
#define ADC_CHANNEL_AD14                                0xe  /*!< ADC����ͨ��14*/
#define ADC_CHANNEL_AD15                                0xf  /*!< ADC����ͨ��15*/
#define ADC_CHANNEL_AD18_VSS                            0x12 /*!< ADC����ͨ�� VSS */
#define ADC_CHANNEL_AD22_TEMPSENSOR                     0x16 /*!< ADC����ͨ���¶ȴ����� */  //Modify
#define ADC_CHANNEL_AD23_BANDGAP                        0x17 /*!< ADC����ͨ����϶ */
#define ADC_CHANNEL_AD29_VREFH                          0x1D /*!< ADC����ͨ�� Vrefh */
#define ADC_CHANNEL_AD30_VREFL                          0x1E /*!< ADC����ͨ�� Vrefl */
#define ADC_CHANNEL_DISABLE                             0x1F /*!< ADC����ͨ������ */

/******************************************************************************
 *
 * ���� ADC FIFO ���
 *
*******************************************************************************/
#define ADC_FIFO_DISABLE                                0 /*!< FIFO����*/
#define ADC_FIFO_LEVEL2                                 1 /*!< 2��FIFO */
#define ADC_FIFO_LEVEL3                                 2 /*!< 3��FIFO */
#define ADC_FIFO_LEVEL4                                 3 /*!< 4��FIFO */
#define ADC_FIFO_LEVEL5                                 4 /*!< 5��FIFO */
#define ADC_FIFO_LEVEL6                                 5 /*!< 6��FIFO */
#define ADC_FIFO_LEVEL7                                 6 /*!< 7��FIFO */
#define ADC_FIFO_LEVEL8                                 7 /*!< 8��FIFO */

/******************************************************************************
 *
 * ����ADCת������Դ
 *
*******************************************************************************/
#define ADC_HARDWARE_TRIGGER                            0x01  /*!< Ӳ������ */
#define ADC_SOFTWARE_TRIGGER                            0x00  /*!< ������� */
#define ADC_TRIGGER_RTC                                 0x00  /*!< ѡ��RTC�����ΪӲ������Դ*/
#define ADC_TRIGGER_PIT                                 0x01  /*!< ѡ��PIT0�����ΪӲ������Դ*/
#define ADC_TRIGGER_ETM2INIT                            0x10  /*!< ѡ��ETM2��ʼ������ΪӲ������Դ */
#define ADC_TRIGGER_ETM2MATCH                           0x11  /*!< ѡ��ETM2ƥ����ΪӲ������Դ */

/******************************************************************************
 *
 * ����ADC�Ƚϴ���ģʽ
 *
*******************************************************************************/
#define ADC_COMPARE_LESS                                0x00 /*!< ����С�ڱȽϵ�ƽʱ�Ƚϴ���*/
#define ADC_COMPARE_GREATER                             0x01 /*!< ������ڱȼ۵�ƽʱ�Ƚϴ���*/

/******************************************************************************
 *
 * ADC�ص���������
 *
******************************************************************************/
    typedef void (*ADC_CallbackType)(void);                         /*!< ADC�ص����� */

/******************************************************************************
 *
 * ����ADC�ṹ�����
 *
*******************************************************************************/
    typedef struct
    {
        uint16_t bIntEn                 :1;     /*!< 1: �ж�ʹ��, 0: �����ж� */
        uint16_t bContinuousEn          :1;     /*!< 1: ʹ������ת��ģʽ, 0: ��������ת��ģʽ */
        uint16_t bHardwareTriggerEn     :1;     /*!< 1: Ӳ������, 0: ������� */
        uint16_t bCompareEn             :1;     /*!< 1: ʹ�ܱȽ�ģʽ, 0: ���ñȽ�ģʽ */
        uint16_t bCompareGreaterEn      :1;     /*!< 1: ������ڻ���ڱȽϵ�ƽʱ�Ƚϴ���, 0: ����С�ڱȽϵ�ƽʱ�Ƚϴ��� */
        uint16_t bLowPowerEn            :1;     /*!< 1: �͹���ģʽ, 0: ����ģʽ */
        uint16_t bLongSampleEn          :1;     /*!< 1: ������ģʽ, 0: �̲���ģʽ */
        uint16_t bFiFoScanModeEn        :1;     /*!< 1: ʹ��FIFOɨ��ģʽ, 0: ����FIFOɨ��ģʽ */
        uint16_t bCompareAndEn          :1;     /*!< 1: �����бȽϴ�����������, 0: �����бȽϴ�����������*/
#ifdef CPU_NV32
        uint16_t bReverse               :7;
#else
        uint16_t bHTRGMEn               :1;     /*!< one hardware trigger pulse trigger multiple conversions in fifo mode */
        uint16_t bHTRGMASKEn            :1;     /*!< Hardware trigger mask enable. */
        uint16_t bHTRGMASKSEL           :1;     /*!< This field selects hardware trigger mask mode. */
        uint16_t Reserve                :4;
#endif
    } ADC_SettingType;

/******************************************************************************
 *
 * ADC���ýṹ��
 *
*******************************************************************************/
    typedef struct
    {
        ADC_SettingType sSetting;               /*!< ADC���ýṹ��*/
        uint16_t u16PinControl;                 /*!< ���ſ��� */
        uint8_t u8ClockSource;                  /*!< ѡ��ʱ��Դ */
        uint8_t u8ClockDiv;                     /*!< ����ʱ�ӷ�Ƶ */
        uint8_t u8Mode;                         /*!< ����ת��ģʽ(8/10/12 bit mode) */
        uint8_t u8FiFoLevel;                    /*!< ����FIFO��� */
    } ADC_ConfigType,*ADC_ConfigTypePtr;
		
/*****************************************************************************//*!
 *
 * @brief ʹ��ADC�ж�.
 *
 * @param[in]  pADC ָ��ADCģ��
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_IntEnable( ADC_Type *pADC )
 {
    pADC->SC1 |= ADC_SC1_AIEN_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADC�ж�
 *
 * @param[in]  pADC ָ��ADCģ��
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_IntDisable( ADC_Type *pADC )
  {
    pADC->SC1 &= ~ADC_SC1_AIEN_MASK;
  }
	
/*****************************************************************************//*!
 *
 * @brief ʹ��ADC����ת��.
 *
 * @param[in]  pADC ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_ContinuousConversion( ADC_Type *pADC )
 {
   pADC->SC1 |= ADC_SC1_ADCO_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ʹ��ADC����ת��
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SingleConversion( ADC_Type *pADC )
 {
    pADC->SC1 &= ~ADC_SC1_ADCO_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADCӲ������.
 *
 * @param[in]  pADC  ָ��ADCģ�� .
 *
 * @return none.
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetHardwareTrigger( ADC_Type *pADC )
 {
   pADC->SC2 |= ADC_SC2_ADTRG_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADC�������.
 *
 * @param[in]  pADC  ָ��ADCģ�� .
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetSoftwareTrigger( ADC_Type *pADC )
 {
   pADC->SC2 &= ~ADC_SC2_ADTRG_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ʹ��ADC�ȽϹ���
 *
 * @param[in]  pADC ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_CompareEnable( ADC_Type *pADC )
 {
   pADC->SC2 |= ADC_SC2_ACFE_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADC�ȽϹ���.
 *
 * @param[in]  pADC  ָ��ADCģ��
 *
 * @return none
 *
*****************************************************************************/
 __STATIC_INLINE void ADC_CompareDisable( ADC_Type *pADC )
  {
    pADC->SC2 &= ~ADC_SC2_ACFE_MASK;
  }
	
/*****************************************************************************//*!
 *
 * @brief ������ڻ���ڱȽϵ�ƽʱ�Ƚϴ���
 *
 * @param[in]  pADC   ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_CompareGreaterFunction( ADC_Type *pADC )
  {
    pADC->SC2 |= ADC_SC2_ACFGT_MASK;
  }
	
/*****************************************************************************//*!
 *
 * @brief ����С�ڱȼ۵�ƽʱ�Ƚϴ���
 *
 * @param[in]  pADC   ָ��ADCģ��
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_CompareLessFunction( ADC_Type *pADC )
 {
   pADC->SC2 &= ~ADC_SC2_ACFGT_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADCΪ�͹���ģʽ
 *
 * @param[in]   pADC ָ��ADCģ��
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetLowPower( ADC_Type *pADC )
 {
   pADC->SC3 |= ADC_SC3_ADLPC_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADCΪ����ģʽ.
 *
 * @param[in]  pADC ָ��ADCģ��..
 *
 * @return none
 *
*****************************************************************************/
 __STATIC_INLINE void ADC_SetHighSpeed( ADC_Type *pADC )
  {
    pADC->SC3 &= ~ADC_SC3_ADLPC_MASK;
  }
	
/*****************************************************************************//*!
 *
 * @brief ����ADC������.
 *
 * @param[in]  pADC   ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetLongSample( ADC_Type *pADC )
 {
   pADC->SC3 |= ADC_SC3_ADLSMP_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ����ADC�̲���
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetShortSample( ADC_Type *pADC )
 {
   pADC->SC3 &= ~ADC_SC3_ADLSMP_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief ʹ��FIFOɨ��ģʽ
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none.
 *
*****************************************************************************/
__STATIC_INLINE void ADC_FifoScanModeEnable( ADC_Type *pADC )
  {
    pADC->SC4 |= ADC_SC4_ASCANE_MASK;
  }
	
/*****************************************************************************//*!
 *
 * @brief ����FIFOɨ��ģʽ.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_FifoScanModeDisable( ADC_Type *pADC )
 {
  pADC->SC4 &= ~ADC_SC4_ASCANE_MASK;
 }
/*****************************************************************************//*!
 *
 * @brief �����бȽϴ�����������.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none
 *
 *****************************************************************************/
__STATIC_INLINE void ADC_CompareFifoOr( ADC_Type *pADC )
 {
   pADC->SC4 &= ~ADC_SC4_ACFSEL_MASK;
 }
 
/*****************************************************************************//*!
 *
 * @brief �����бȽϴ�����������
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_CompareFifoAnd( ADC_Type *pADC )
 {
   pADC->SC4 |= ADC_SC4_ACFSEL_MASK;
 }
/*****************************************************************************//*!
 *
 * @brief ��ADCת������Ĵ���.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return ADC result value.
 *
*****************************************************************************/
__STATIC_INLINE uint16_t ADC_ReadResultReg( ADC_Type *pADC )
 {
   return (uint16_t)pADC->R;
 }
 
/*****************************************************************************//*!
 *
 * @brief ���ñȽ�ֵ.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 * @param[in]  u16Compare  �Ƚ�ֵ
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_SetCompareValue( ADC_Type *pADC, uint16_t u16Compare )
  {
    pADC->CV = u16Compare;
  }
	
/*****************************************************************************//*!
 *
 * @brief ʹ��ADC��������.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 * @param[in]  u16PinNumber    ʹ�ܵ�ADC����
 *
 * @return none
 *
 * @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void ADC_PinControlEnable( ADC_Type *pADC, uint16_t u16PinNumber)
  {
     ASSERT((u16PinNumber<16));
     pADC->APCTL1 &= ~(0x01<<u16PinNumber);
  }
	
/*****************************************************************************//*!
 *
 * @brief ����ADC��������.
 *
 * @param[in]  pADC  ָ��ADCģ��.
 * @param[in]  u16PinNumber  ���õ�ADC����.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_PinControlDisable( ADC_Type *pADC, uint16_t u16PinNumber)
 {
   ASSERT((u16PinNumber<16));
   pADC->APCTL1 |= (0x01<<u16PinNumber);
 }
 
/*****************************************************************************//*!
 *
 * @brief �鿴ADCת��״̬
 *
 * @param[in]  pADC  ָ��ADCģ��..
 *
 * @return 1 or 0
 *
*****************************************************************************/
__STATIC_INLINE uint8_t ADC_IsConversionActiveFlag( ADC_Type *pADC )
  {
    return(pADC->SC2 & ADC_SC2_ADACT_MASK);
  }
/*****************************************************************************//*!
 *
 * @brief ��ȡת����ɱ�׼λ
 *
 * @param[in]  pADC  ָ��ADCģ��..
 *
 * @return 1 or 0
 *
*****************************************************************************/
__STATIC_INLINE uint8_t ADC_IsCOCOFlag( ADC_Type *pADC )
 {
   return(pADC->SC1 & ADC_SC1_COCO_MASK);
 }
 
/*****************************************************************************//*!
 *
 * @brief ��ȡ���FIFO���Ƿ�����Ч�����ݱ�־λ
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return 1 or 0
 *
*****************************************************************************/
__STATIC_INLINE uint8_t ADC_IsFIFOEmptyFlag( ADC_Type *pADC )
 {
   return(pADC->SC2 & ADC_SC2_FEMPTY_MASK);
 }
 
/*****************************************************************************//*!
 *
 * @brief ��ȡ���FIFO�Ƿ�����־λ
 *
 * @param[in]  pADC  ָ��ADCģ��.
 *
 * @return  1 or 0
 *
*****************************************************************************/
__STATIC_INLINE uint8_t ADC_IsFIFOFullFlag( ADC_Type *pADC )
 {
   return(pADC->SC2 & ADC_SC2_FFULL_MASK);
 }
#ifndef CPU_NV32
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Multiple Conversion Enable
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerMultiple( ADC_Type *pADC )
  {
    pADC->SC4 |= ADC_SC4_HTRGME_MASK;
  }
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Single Conversion
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerSingle( ADC_Type *pADC )
{
   pADC->SC4 &= ~ADC_SC4_HTRGME_MASK;
}
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Mask Enable
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerMaskEnable( ADC_Type *pADC )
  {
    pADC->SC5 |= ADC_SC5_HTRGMASKE_MASK;
  }
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Mask Disable
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerMaskDisable( ADC_Type *pADC )
 {
   pADC->SC5 &= ~ADC_SC5_HTRGMASKE_MASK;
 }
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Mask Mode Select Automatic Mode
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerMaskAuto( ADC_Type *pADC )
  {
    pADC->SC5 |= ADC_SC5_HTRGMASKSEL_MASK;
  }
/*****************************************************************************//*!
 *
 * @brief Hardware Trigger Mask Mode Select to be with HTRGMASKE
 *
 * @param[in]  pADC point to ADC module type.
 *
 * @return none
 *
 * @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void ADC_HardwareTriggerMaskNonAuto( ADC_Type *pADC )
  {
    pADC->SC5 &= ~ADC_SC5_HTRGMASKSEL_MASK;
  }
#endif
/******************************************************************************
 * Global function
 ******************************************************************************/
void ADC_SetChannel( ADC_Type *pADC, uint8_t u8Channel );
void ADC_IntEnable( ADC_Type *pADC );
void ADC_IntDisable( ADC_Type *pADC );
void ADC_ContinuousConversion( ADC_Type *pADC );
void ADC_SingleConversion( ADC_Type *pADC );
void ADC_SetSoftwareTrigger( ADC_Type *pADC );
void ADC_SetHardwareTrigger( ADC_Type *pADC );
void ADC_VrefSelect( ADC_Type *pADC, uint8_t u8Vref );
void ADC_CompareEnable( ADC_Type *pADC );
void ADC_CompareDisable( ADC_Type *pADC );
void ADC_CompareGreaterFunction( ADC_Type *pADC );
void ADC_CompareLessFunction( ADC_Type *pADC );
void ADC_SetLowPower( ADC_Type *pADC );
void ADC_SetHighSpeed( ADC_Type *pADC );
void ADC_SelectClockDivide( ADC_Type *pADC, uint8_t u8Div);
void ADC_SetLongSample(ADC_Type *pADC);
void ADC_SetShortSample(ADC_Type *pADC);
void ADC_SetMode(ADC_Type *pADC, uint8_t u8Mode);
void ADC_SelectClock(ADC_Type *pADC, uint8_t u8Clock);
void ADC_FifoScanModeEnable(ADC_Type *pADC);
void ADC_FifoScanModeDisable(ADC_Type *pADC);
void ADC_CompareFifoOr(ADC_Type *pADC);
void ADC_CompareFifoAnd(ADC_Type *pADC);
void ADC_SetFifoLevel(ADC_Type *pADC, uint8_t u8FifoLevel);
uint16_t ADC_ReadResultReg(ADC_Type *pADC );
void ADC_SetCompareValue(ADC_Type *pADC, uint16_t u16Compare );
void ADC_PinControlEnable(ADC_Type *pADC, uint16_t u16PinNumber);
void ADC_PinControlDisable(ADC_Type *pADC, uint16_t u16PinNumber);
uint8_t ADC_IsConversionActiveFlag(ADC_Type *pADC);
uint8_t ADC_IsCOCOFlag(ADC_Type *pADC);
uint8_t ADC_IsFIFOEmptyFlag(ADC_Type *pADC);
uint8_t ADC_IsFIFOFullFlag(ADC_Type *pADC);
void ADC_HardwareTriggerMaskNonAuto(ADC_Type *pADC);
void ADC_HardwareTriggerMaskAuto(ADC_Type *pADC);
void ADC_HardwareTriggerMaskDisable( ADC_Type *pADC );
void ADC_HardwareTriggerMaskEnable( ADC_Type *pADC );
void ADC_HardwareTriggerSingle( ADC_Type *pADC );
void ADC_HardwareTriggerMultiple( ADC_Type *pADC );
unsigned int ADC_PollRead( ADC_Type *pADC, uint8_t u8Channel);
void ADC_SetCallBack(ADC_CallbackType pADC_CallBack);
void ADC_DeInit(ADC_Type *pADC);
void ADC_Init(ADC_Type *pADC, ADC_ConfigTypePtr pADC_Config);
#ifdef __cplusplus
}
#endif
#endif
