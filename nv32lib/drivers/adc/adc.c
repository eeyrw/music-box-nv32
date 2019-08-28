/**************************************************************************!
 *     技术讨论：QQ群  123763203
 *     官网    ：www.navota.com
 *
 * @file       adc.c
 * @brief      数模转换模块(ADC)库函数
 * @author     Navota
 * @date       2018-3-1
 ****************************************************************************/
 
#include "common.h"
#include "adc.h"

/****************************************************************************!
 * @ 存放ACMP回调函数接口
 ****************************************************************************/
ADC_CallbackType ADC_Callback[1] = {NULL};

/*****************************************************************************//**
   *
   * @brief  初始化ADC模块.
   *
   * @param[in]  pADC         指向ADC模块类型.
   * @param[in]  pADC_Config  指向ADC模块配置结构体
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
    /* 选择ADC模块时钟源 */
    ADC_SelectClock(pADC,pADC_Config->u8ClockSource);
    /* 设定时钟分频 */
    ADC_SelectClockDivide(pADC,pADC_Config->u8ClockDiv);
    /* 设置ADC转换模式 */
    ADC_SetMode(pADC,pADC_Config->u8Mode);
    /* 设置FIFO深度 */
    ADC_SetFifoLevel(pADC,pADC_Config->u8FiFoLevel);
    /* ADC输入引脚控制 */
    pADC->APCTL1 = pADC_Config->u16PinControl;
    if( pADC_Config->sSetting.bCompareEn )
    {
        ADC_CompareEnable(pADC);     //使能ADC比较功能
    }
    if( pADC_Config->sSetting.bCompareGreaterEn )
    {
        ADC_CompareGreaterFunction(pADC);  //输入大于或等于比较电平时比较触发
    }
    if( pADC_Config->sSetting.bContinuousEn )
    {
        ADC_ContinuousConversion(pADC);    // 使能ADC连续转换.
    }
    if( pADC_Config->sSetting.bCompareAndEn )
    {
        ADC_CompareFifoAnd(pADC);     //对所有比较触发做与运算
    }
    if( pADC_Config->sSetting.bFiFoScanModeEn )
    {
        ADC_FifoScanModeEnable(pADC);  //使能FIFO扫描模式
    }
    if( pADC_Config->sSetting.bHardwareTriggerEn )
    {
        ADC_SetHardwareTrigger(pADC);  // 设置ADC硬件触发.
    }
    if( pADC_Config->sSetting.bIntEn )  //使能中断
    {
        ADC_IntEnable(pADC);
        NVIC_EnableIRQ( ADC0_IRQn );
    }
    if( pADC_Config->sSetting.bLongSampleEn )
    {
        ADC_SetLongSample(pADC);   // 设置ADC长采样.
    }
    if( pADC_Config->sSetting.bLowPowerEn )
    {
        ADC_SetLowPower(pADC);   // 设置ADC为低功耗模式
    }
}

/*****************************************************************************//*!
   *
   * @brief 禁用ADC模块
   *
   * @param[in]  pADC 指向ADC模块类型
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
   * @brief 开始转换和读取转换结果
   *
   * @param[in]  pADC  指向ADC模块
   * @param[in]  u8Channel ADC转换通道
   *
   * @return ADC 转换结果.
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
   * @brief 注册 ADC 回调函数，通过中断服务函数调用
   *
   * @param[in]  pADC_CallBack 指向ADC回调函数地址.
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
   * @brief 选择ADC输入通道.
   *
   * @param[in]  pADC  指向ADC模块.
   * @param[in]  u8Channel ADC转换通道
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
   * @brief 选择ADC基准电压.
   *
   * @param[in]  pADC    指向ADC模块
   * @param[in]  u8Vref  选择ADC基准电压.
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
   * @brief 设置ADC时钟源分频
   *
   * @param[in]  pADC  指向ADC模块.
   * @param[in]  u8Div 选择分频系数.
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
   * @brief 设置ADC转换模式
   *
   * @param[in]  pADC 指向ADC模块
   * @param[in]  u8Mode 选择ADC转换参数.
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
   * @brief  设置ADC输入时钟.
   *
   * @param[in]  pADC    指向ADC模块
   * @param[in]  u8Clock 选择输入时钟源.
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
   * @brief 设置FIFO深度
   *
   * @param[in]  pADC 指向ADC模块.
   * @param[in]  u8FifoLevel 选择FIFO深度.
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
   * @brief ADC 中断服务函数.
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
