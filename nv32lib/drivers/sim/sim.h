/******************************************************************************
*
* @brief  SIM ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef SIM_H_
#define SIM_H_

typedef enum {
    ID_TYPE_FAMID,                  /*!< NV32F100xϵ��   ID */
    ID_TYPE_SUBFAMID,               /*!< NV32F100x��ϵ�� ID */ 
    ID_TYPE_REVID,                  /*!< �����汾 ID */
    ID_TYPE_PINID                   /*!< �������� ID */
} IDType;
                                                    				

#if defined(CPU_NV32)
typedef struct{
    struct{
        uint32_t bEnableCLKOUT  : 1;        /*!< 1: ʹ�� , 0: ���� */
        uint32_t bTXDME         : 1;        /*!< 1: ʹ�� TXDME, 0: ���� */
        uint32_t bETMSYNC       : 1;        /*!< 1: ʹ�� ETM SYNC, 0: δ�����κ�ͬ�� */
        uint32_t bRXDFE         : 1;        /*!< 1: ʹ�� RXD �˲�, 0: ���˲� */
        uint32_t bRXDCE         : 1;        /*!< 1: ʹ�� RXD ��׽, 0: �޲�׽ */
        uint32_t bACIC          : 1;        /*!< 1: ACMP0�����ͨ�����ӵ�ETM1������ͨ��0, 0: ������ */
        uint32_t bRTCC          : 1;        /*!< 1: RTC������ӵ�ETM1����ͨ��1, 0: ������ */
        uint32_t u8ADHWT        : 2;        /*!<    ѡ��ADCת��Ӳ������Դ */
        uint32_t bDisableSWD    : 1;        /*!< 1: ����SWD, 0: ʹ�� */
        uint32_t bDisableRESET  : 1;        /*!< 1: ���ø�λ����, 0: ʹ�� */
        uint32_t bDisableNMI    : 1;        /*!< 1: ����NMI�ж���������, 0:ʹ�� */
        uint32_t bBusDiv        : 1;        /*!<    ���߷�Ƶϵ�� */
    } sBits;
    uint8_t     u8Delay;                		/*!< ETM����ADCת����ʱֵ */
    uint8_t     u8BusRef;               		/*!< ���߲ο� */
    uint32_t    u32PinSel;              		/*!< ����ѡ��Ĵ�����ֵ*/
    uint32_t    u32SCGC;                		/*!< ʱ��ѡͨ�Ĵ�����ֵ */   
} SIM_ConfigType, *SIM_ConfigPtr; /*!< SIMģ�����ýṹ������ */
#endif
                                                 				
  
#if defined(CPU_NV32)
/*****************************************************************************//*!
*
* @brief ����ETM2����ADCת������ʱʱ��
*        
* @param[in]   u8Delay     ��ʱʱ��ģ��ֵ���ο�ʱ�Ӳ�������ʱ�ӷ�Ƶ
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void SIM_DelayETM2Trig2ADC(uint8_t u8Delay)
{
    SIM->SOPT = (SIM->SOPT & ~(SIM_SOPT_DELAY_MASK)) | SIM_SOPT_DELAY(u8Delay);    
}
/*****************************************************************************//*!
*
* @brief ʹ������ʱ����PH2��
*
* @param   none
*
* @return none 
*
* @ �ο�  SIM_DisableClockOutput
*****************************************************************************/
__STATIC_INLINE void SIM_EnableClockOutput(void)
{
    SIM->SOPT |= (SIM_SOPT_CLKOE_MASK);    
}

/*****************************************************************************//*!
*
* @brief ��������ʱ�������PH2��
*        
* @param   none
*
* @return none
*
* @�ο�  SIM_EnableClockOutput
*****************************************************************************/
__STATIC_INLINE void SIM_DisableClockOutput(void)
{
    SIM->SOPT &= ~(SIM_SOPT_CLKOE_MASK);    
}
/*****************************************************************************//*!
*
* @brief ��������ʱ�������Ƶ. 
*        
* @param[in]  u8Divide     ��Ƶϵ�� (3-bits)
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_SetClockOutputDivide(uint8_t u8Divide)
{
    SIM->SOPT = (SIM->SOPT & ~(SIM_SOPT_BUSREF_MASK)) | SIM_SOPT_BUSREF(u8Divide & 0x07);    
}
/*****************************************************************************//*!
*
* @brief UART0_RX�����ź����ӵ�UART0ģ���ETM0ͨ��1
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_EnableUART0RXDConnectETMOCH1(void)
{
    SIM->SOPT |= (SIM_SOPT_RXDCE_MASK);    
}
/*****************************************************************************//*!
*
* @brief  UART0_TX�����ӳ�䵽������ǰ��ETM0ͨ��0����
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_EnableUART0Modulation(void)
{
    SIM->SOPT |= (SIM_SOPT_TXDME_MASK);    
}

/*****************************************************************************//*!
*
* @brief  UART0_TX���ֱ�����ӵ���������
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_DisableUART0Modulation(void)
{
    SIM->SOPT &= ~(SIM_SOPT_TXDME_MASK);    
}
/*****************************************************************************//*!
*
* @brief ����ETM2ģ���PWMͬ������
*        
* @param  none       
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void SIM_GenerateSoftwareTrig2ETM2(void)
{
    SIM->SOPT |= (SIM_SOPT_ETMSYNC_MASK);    
}
/*****************************************************************************//*!
*
* @brief ETM2_CH3ͨ��ӳ�䵽PD1��  
*        
* @param   none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM2CH3Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM2PS3_MASK;    
}

/*****************************************************************************//*!
*
* @brief  ETM2_CH2ͨ��ӳ�䵽PD0��
*        
* @param   none
*
* @return  none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM2CH2Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM2PS2_MASK;    
}
/*****************************************************************************//*!
*
* @brief  ETM0_CH1ͨ��ӳ�䵽PB3��  
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM0CH1Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM0PS1_MASK;    
}
/*****************************************************************************//*!
*
* @brief   ETM0_CH0ͨ��ӳ�䵽PB2�� 
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM0CH0Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM0PS0_MASK;    
}
/*****************************************************************************//*!
*
* @brief  UART0_RX��UART0_TXӳ�䵽PA2��PA3�� 
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapUART0Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_UART0PS_MASK;    
}

/*****************************************************************************//*!
*
* @brief SPI0_SCK SPI0_MOSI SPI0_MISO��SPI0_PCSӳ�䵽PE0��PE1��PE2��PE3�� 
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapSPI0Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;    
}

/*****************************************************************************//*!
*
* @brief  I2C0_SCL��I2C_SDA�ֱ�ӳ�䵽PB7��PB6��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapI2CPin(void)
{
    SIM->PINSEL |= SIM_PINSEL_IICPS_MASK;    
}
/*****************************************************************************//*!
*
* @brief RXD0�����ź���ACPM0�˲���Ȼ��ע��UART0 
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_EnableUART0Filter(void)
{
    SIM->SOPT |= (SIM_SOPT_RXDFE_MASK);    
}
/******************************************************************************!
*
* @brief RXD0�����ź�ֱ�����ӵ�UARTOģ��
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_DisableUART0Filter(void)
{
    SIM->SOPT &= ~(SIM_SOPT_RXDFE_MASK);    
}
/*****************************************************************************//*!
*
* @brief ѡ��RTC�����ΪADCӲ������Դ
*        
* @param  none        
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_TriggerADCByRTC(void)
{
    SIM->SOPT &= ~(SIM_SOPT_ADHWT_MASK);    
}
/*****************************************************************************//*!
*
* @brief ѡ��PIT�����ΪADCӲ������Դ 
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_TriggerADCByPIT(void)
{
    SIM->SOPT = (SIM->SOPT & ~(SIM_SOPT_ADHWT_MASK)) | SIM_SOPT_ADHWT(1);    
}
/*****************************************************************************//*!
*
* @brief ����ETM2��ʼ����ΪADCӲ������Դ.ETM2��ʼ���󾭹�һ��ʱ����ʱ����ADCת��
*        
* @param  none      
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_TriggerADCByETM2Init(void)
{
    SIM->SOPT = (SIM->SOPT & ~(SIM_SOPT_ADHWT_MASK)) | SIM_SOPT_ADHWT(2);    
}

/*****************************************************************************//*!
*
* @brief ����ETM2ƥ����ΪADCӲ������Դ��ETM2ƥ��󾭹�һ��ʱ�����ʱ����ADCת��
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_TriggerADCByETM2Match(void)
{
    SIM->SOPT = (SIM->SOPT & ~(SIM_SOPT_ADHWT_MASK)) | SIM_SOPT_ADHWT(3);    
}
/*****************************************************************************//*!
*
* @brief RTC������ӵ�ETM1����ͨ��1
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_EnableRTCCapture(void)
{
    SIM->SOPT |= (SIM_SOPT_RTCC_MASK);    
}
/*****************************************************************************//*!
*
* @brief RTC���δ���ӵ�ETM1����ͨ��1
*        
* @param  none       
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_DisableRTCCapture(void)
{
    SIM->SOPT &= ~(SIM_SOPT_RTCC_MASK);    
}
/*****************************************************************************//*!
*
* @brief ACMP0������ӵ�ETM1����ͨ��0
*        
* @param  none      
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_EnableACMP0InputCapture(void)
{
    SIM->SOPT |= (SIM_SOPT_ACIC_MASK);    
}
/*****************************************************************************//*!
*
* @brief ACMP0���δ���ӵ�ETM1����ͨ��0 
*        
* @param  none      
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_DisableACMP0InputCapture(void)
{
    SIM->SOPT &= ~(SIM_SOPT_ACIC_MASK);    
}
/*****************************************************************************//*!
*
* @brief RTC0ӳ�䵽PC5��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapRTCPin(void)
{
    SIM->PINSEL |= SIM_PINSEL_RTCPS_MASK;    
}
/*****************************************************************************//*!
*
* @brief ��������ʱ��Ƶ��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_SetBusDivide(uint8_t u8Divide)
{
    SIM->BUSDIV = u8Divide;    
}
/*****************************************************************************//*!
*
* @brief ETM2_CH1ͨ��ӳ�䵽PH1��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM2CH1Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM2PS1_MASK;    
}

/*****************************************************************************//*!
*
* @brief  ETM2_CH0ͨ��ӳ�䵽PH0��  
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM2CH0Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM2PS0_MASK;    
}

/*****************************************************************************//*!
*
* @brief  ETM1_CH1ͨ��ӳ�䵽PE7��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM1CH1Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM1PS1_MASK;    
}

/*****************************************************************************//*!
*
* @brief  ETM1_CH0ӳ�䵽PH2��
*        
* @param   none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void SIM_RemapETM1CH0Pin(void)
{
    SIM->PINSEL |= SIM_PINSEL_ETM1PS0_MASK;    
}

#endif

/****************************************************************************/

void SIM_Init(SIM_ConfigType *pConfig);
void SIM_SetClockGating(uint32_t u32PeripheralMask, uint8_t u8GateOn);
uint32_t SIM_GetStatus(uint32_t u32StatusMask);
uint8_t SIM_ReadID(IDType sID);

#endif 


