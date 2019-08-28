/**************************************************************************!
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       adc.c
 * @brief      ��ģת��ģ��(ADC)�⺯������
 * @author     Navota
 * @date       2018-3-1
 ****************************************************************************/
 
#include "common.h"
#include "adc.h"

/****************************************************************************!
 * @ ���ACMP�ص������ӿ�
 ****************************************************************************/
ADC_CallbackType ADC_Callback[1] = {NULL};

/*****************************************************************************//**
   *
   * @brief  ��ʼ��ADCģ��.
   *
   * @param[in]  pADC         ָ��ADCģ������.
   * @param[in]  pADC_Config  ָ��ADCģ�����ýṹ��
   *
   * @return none
   *
   *****************************************************************************/
void ADC_Init(ADC_Type *pADC, ADC_ConfigTypePtr pADC_Config)
{
    if( pADC == ADC)
    {
        SIM->SCGC |= SIM_SCGC_ADC_MASK;
    }
    /* ѡ��ADCģ��ʱ��Դ */
    ADC_SelectClock(pADC,pADC_Config->u8ClockSource);
    /* �趨ʱ�ӷ�Ƶ */
    ADC_SelectClockDivide(pADC,pADC_Config->u8ClockDiv);
    /* ����ADCת��ģʽ */
    ADC_SetMode(pADC,pADC_Config->u8Mode);
    /* ����FIFO��� */
    ADC_SetFifoLevel(pADC,pADC_Config->u8FiFoLevel);
    /* ADC�������ſ��� */
    pADC->APCTL1 = pADC_Config->u16PinControl;
    if( pADC_Config->sSetting.bCompareEn )
    {
        ADC_CompareEnable(pADC);     //ʹ��ADC�ȽϹ���
    }
    if( pADC_Config->sSetting.bCompareGreaterEn )
    {
        ADC_CompareGreaterFunction(pADC);  //������ڻ���ڱȽϵ�ƽʱ�Ƚϴ���
    }
    if( pADC_Config->sSetting.bContinuousEn )
    {
        ADC_ContinuousConversion(pADC);    // ʹ��ADC����ת��.
    }
    if( pADC_Config->sSetting.bCompareAndEn )
    {
        ADC_CompareFifoAnd(pADC);     //�����бȽϴ�����������
    }
    if( pADC_Config->sSetting.bFiFoScanModeEn )
    {
        ADC_FifoScanModeEnable(pADC);  //ʹ��FIFOɨ��ģʽ
    }
    if( pADC_Config->sSetting.bHardwareTriggerEn )
    {
        ADC_SetHardwareTrigger(pADC);  // ����ADCӲ������.
    }
    if( pADC_Config->sSetting.bIntEn )  //ʹ���ж�
    {
        ADC_IntEnable(pADC);
        NVIC_EnableIRQ( ADC0_IRQn );
    }
    if( pADC_Config->sSetting.bLongSampleEn )
    {
        ADC_SetLongSample(pADC);   // ����ADC������.
    }
    if( pADC_Config->sSetting.bLowPowerEn )
    {
        ADC_SetLowPower(pADC);   // ����ADCΪ�͹���ģʽ
    }
}

/*****************************************************************************//*!
   *
   * @brief ����ADCģ��
   *
   * @param[in]  pADC ָ��ADCģ������
   *
   * @return none.
   *
   *****************************************************************************/
void ADC_DeInit( ADC_Type *pADC )
{
    ADC_SetChannel(pADC,ADC_CHANNEL_DISABLE);
    SIM->SCGC &= ~SIM_SCGC_ADC_MASK;
}

/*****************************************************************************//*!
   *
   * @brief ��ʼת���Ͷ�ȡת�����
   *
   * @param[in]  pADC  ָ��ADCģ��
   * @param[in]  u8Channel ADCת��ͨ��
   *
   * @return ADC ת�����.
   *
   *****************************************************************************/
unsigned int ADC_PollRead( ADC_Type *pADC, uint8_t u8Channel )
{
    ADC_SetChannel(pADC,u8Channel);
    while( !ADC_IsCOCOFlag(pADC) )
    {
        ;
    }
    return ADC_ReadResultReg(pADC);
}

/*****************************************************************************//*!
   *
   * @brief ע�� ADC �ص�������ͨ���жϷ���������
   *
   * @param[in]  pADC_CallBack ָ��ADC�ص�������ַ.
   *
   * @return none.
   *
   *****************************************************************************/
void ADC_SetCallBack(ADC_CallbackType pADC_CallBack)
{
    ADC_Callback[0] = pADC_CallBack;
}

/*****************************************************************************//*!
   *
   * @brief ѡ��ADC����ͨ��.
   *
   * @param[in]  pADC  ָ��ADCģ��.
   * @param[in]  u8Channel ADCת��ͨ��
   *
   * @return none
   *
   *****************************************************************************/
void ADC_SetChannel( ADC_Type *pADC, uint8_t u8Channel )
{
    uint32_t u32temp;
    u32temp = pADC->SC1;
    u32temp &= ~ADC_SC1_ADCH_MASK;
    pADC->SC1 = u32temp|ADC_SC1_ADCH(u8Channel);
}

/*****************************************************************************//*!
   *
   * @brief ѡ��ADC��׼��ѹ.
   *
   * @param[in]  pADC    ָ��ADCģ��
   * @param[in]  u8Vref  ѡ��ADC��׼��ѹ.
   *
   * @return none
   *
   *****************************************************************************/
void ADC_VrefSelect( ADC_Type *pADC, uint8_t u8Vref )
{
    uint32_t u32Temp;
    u32Temp = pADC->SC2;
    u32Temp &= ~ADC_SC2_REFSEL_MASK;
    pADC->SC2 = u32Temp|ADC_SC2_REFSEL(u8Vref);
}

/*****************************************************************************//*!
   *
   * @brief ����ADCʱ��Դ��Ƶ
   *
   * @param[in]  pADC  ָ��ADCģ��.
   * @param[in]  u8Div ѡ���Ƶϵ��.
   *
   * @return none
   *
   *****************************************************************************/
void ADC_SelectClockDivide( ADC_Type *pADC, uint8_t u8Div )
{
    uint32_t u32Temp;
    u32Temp = pADC->SC3;
    u32Temp &= ~ADC_SC3_ADIV_MASK;
    pADC->SC3 = u32Temp|ADC_SC3_ADIV(u8Div);
}

/*****************************************************************************//*!
   *
   * @brief ����ADCת��ģʽ
   *
   * @param[in]  pADC ָ��ADCģ��
   * @param[in]  u8Mode ѡ��ADCת������.
   *
   * @return none
   *
   *****************************************************************************/
void ADC_SetMode( ADC_Type *pADC, uint8_t u8Mode )
{
    uint32_t u32Temp;
    u32Temp = pADC->SC3;
    u32Temp &= ~ADC_SC3_MODE_MASK;
    pADC->SC3 = u32Temp|ADC_SC3_MODE(u8Mode);
}

/*****************************************************************************//*!
   *
   * @brief  ����ADC����ʱ��.
   *
   * @param[in]  pADC    ָ��ADCģ��
   * @param[in]  u8Clock ѡ������ʱ��Դ.
   *
   * @return none
   *
   *****************************************************************************/
void ADC_SelectClock( ADC_Type *pADC, uint8_t u8Clock )
{
    uint32_t u32Temp;
    u32Temp = pADC->SC3;
    u32Temp &= ~ADC_SC3_ADICLK_MASK;
    pADC->SC3 = u32Temp|ADC_SC3_ADICLK(u8Clock);
}

/*****************************************************************************//*!
   *
   * @brief ����FIFO���
   *
   * @param[in]  pADC ָ��ADCģ��.
   * @param[in]  u8FifoLevel ѡ��FIFO���.
   *
   * @return none
   *
   *****************************************************************************/
void ADC_SetFifoLevel( ADC_Type *pADC, uint8_t u8FifoLevel )
{
    uint32_t u32Temp;
    u32Temp = pADC->SC4;
    u32Temp &= ~ADC_SC4_AFDEP_MASK;
    pADC->SC4 = u32Temp|ADC_SC4_AFDEP(u8FifoLevel);
}

/*****************************************************************************//*!
   *
   * @brief ADC �жϷ�����.
   *
   * @param  none.
   *
   * @return none.
   *
   *****************************************************************************/
void ADC_Isr(void)
{
    if( ADC_Callback[0] )
    {
        ADC_Callback[0]();
    }
}
