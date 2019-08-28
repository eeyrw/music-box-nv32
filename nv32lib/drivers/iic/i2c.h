/******************************************************************************
*
* @brief  I2C ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef _I2C_H__
#define _I2C_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
/******************************************************************************
* ����I2C�Ķ���д
*
*//*! 
* @{
*******************************************************************************/
#define I2C_READ                        0x01				/*!< I2C �� */
#define I2C_WRITE                       0x0  				/*!< I2C д */
/*! @}                                            */

#define I2C_SEND_ACK                    0				/*!< I2C ���� ACK */
#define I2C_SEND_NACK                   1				/*!< I2C ���� NACK */

#define I2C_WAIT_STATUS_ETMEOUT         200000

/******************************************************************************
* ����I2C����״̬
*
*//*!
* @{
*******************************************************************************/
#define I2C_ERROR_NULL                  0x00			/*!< I2C �����ɹ�*/
#define I2C_ERROR_NO_WAIT_TCF_FLAG      0x01			/*!< I2C �ȴ�������ɳ�ʱ*/
#define I2C_ERROR_NO_WAIT_IICIF_FLAG    0x02			/*!< I2C �ȴ��жϳ�ʱ */
#define I2C_ERROR_NO_GET_ACK            0x04			/*!< I2C û�еõ� ACK */
#define I2C_ERROR_START_NO_BUSY_FLAG    0x10			/*!< I2C û�гɹ�������ʼ�ź� */
#define I2C_ERROR_STOP_BUSY_FLAG        0x20			/*!< I2C û�гɹ�����ֹͣ�ź� */
#define I2C_ERROR_BUS_BUSY              0x80			/*!< I2C ���߷�æ���� */
/*! @} End of i2c_error_state_list                                            */

/******************************************************************************
* ����I2C����״̬
*
*//*! 
* @{
*******************************************************************************/
#define I2C_BUS_NORMAL				        0x00								/*!< I2C �������� */
#define I2C_BUS_SLTF					0x01								/*!< I2C ����ƫ��һ��FLAG */
#define I2C_BUS_SHTF2					0x02								/*!< I2C ����ƫ������FLAG */
/*! @}                                                                        */


#define I2C_MODE_MASTER             1
#define I2C_MODE_SLAVE              0
#define I2C_ADDRESS_7BIT            0
#define I2C_ADDRESS_10BIT           1
#define I2C_ETMEOUT_BUS_CLOCK_DIV64 0
#define I2C_ETMEOUT_BUS_CLOCK       1

/******************************************************************************
*
*//*! @I2C���Ʋ����ṹ��
* @{
*******************************************************************************/
/*!
 * @I2C���Ʋ����ṹ��.
 *
 */
typedef struct
{
	uint16_t bI2CEn         :1;     			/*!< ʹ��I2Cģ�� */
	uint16_t bIntEn         :1;    			 	/*!< ʹ��I2C�ж� */
	uint16_t bWakeUpEn      :1;     			/*!< I2C����ʹ�� */
	uint16_t bGCAEn         :1;     			/*!< I2Cͨ��������ַʹ�� */
	uint16_t bAddressExt    :1;     			/*!< I2C��ַ��չѡ�� */
	uint16_t bRangeAddEn    :1;     			/*!< ʹ�ܷ�Χ��ַƥ�� */
	uint16_t bFackEn        :1;     			/*!< ʹ�ܿ���ack */
	uint16_t bSMB_AlertEn   :1;     			/*!< SMB ������Ӧ��ַʹ�� */
	uint16_t bSecondAddressEn:1;    			/*!< ���õڶ�I2C��ַ */
	uint16_t bETMeoutCountClockSelect:1;                    /*!< ��ʱ������ʱ��ѡ�� */
    uint16_t bSHTF2IntEn    :1;					/*!< SHTF2 �ж�ʹ�� */
    uint16_t Reserve        :5;
}I2C_SettingType;
/*! @}                                      						*/

/******************************************************************************
*
*//*! @I2C���ýṹ��
* @{
*******************************************************************************/
/*!
 * @I2C���ýṹ��.
 *
 */
typedef struct
{
    I2C_SettingType sSetting;
    uint16_t u16F;              /*!< ����I2Cģ��Ĳ����� */
    uint16_t u16OwnA1;          /*!< �ӻ���ַ */
    uint16_t u16OwnA2;          /*!< SMBusʹ�õĴӻ���ַ */
    uint16_t u16RangeA;         /*!< ��Χ��ַ */
    uint16_t u16Filt;           /*!< I2C��������˲���   */
    uint16_t u16Slt;            /*!< SCL�ͳ�ʱ���� */
    
}I2C_ConfigType, *I2C_ConfigPtr;
/*! @}                                           				*/

/*!
 * @brief I2C �ص�����.
 *
 */

typedef void (*I2C_CallbackType)(void);		
/*! @}                            						*/

/******************************************************************************
******************************************************************************/

/*!
 * ��������
 */

/*****************************************************************************//*!
*
* @brief ����I2C����ģʽ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_TxEnable(I2C_Type *pI2Cx)
{
    pI2Cx->C1 |= I2C_C1_TX_MASK;
}

/*****************************************************************************//*!
*
* @brief ����I2C����ģʽ.
*      
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_RxEnable(I2C_Type *pI2Cx)
{
    pI2Cx->C1 &= ~I2C_C1_TX_MASK;
}

/*****************************************************************************//*!
*
* @brief ����I2C������. 
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_SetBaudRate(I2C_Type *pI2Cx,uint32_t u32Bps)
{
    pI2Cx->F = (uint8_t)u32Bps;
}

/*****************************************************************************//*!
*
* @brief ʹ��ͨ�õ�ַ����.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_GeneralCallEnable(I2C_Type *pI2Cx)
{
     pI2Cx->C2 |= I2C_C2_GCAEN_MASK;
}

/*****************************************************************************//*!
*
* @brief SMBus ������Ӧ��ַʹ��. 
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_SMBusAlertEnable(I2C_Type *pI2Cx)
{
     pI2Cx->SMB|= I2C_SMB_ALERTEN_MASK;
}

/*****************************************************************************//*!
*
* @brief ��Χ��ַƥ��ʹ��.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_RangeAddressEnable(I2C_Type *pI2Cx)
{
     pI2Cx->C2 |= I2C_C2_RMEN_MASK;
}

/*****************************************************************************//*!
*
* @brief SHTF2 �ж�ʹ��.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_SHTF2IntEnable(I2C_Type *pI2Cx)
{
     pI2Cx->SMB |= I2C_SMB_SHTF2IE_MASK;
}

/*****************************************************************************//*!
*
* @brief ��ʱ������ʱ��ѡ��.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_ETMeoutCounterClockSelect(I2C_Type *pI2Cx, uint8_t u8Clock)
{
    if( u8Clock )
    {
        pI2Cx->SMB |= I2C_SMB_TCKSEL_MASK;
    }
    else
    {
        pI2Cx->SMB &= ~I2C_SMB_TCKSEL_MASK;
    }
}

/*****************************************************************************//*!
*
* @brief ��ȡI2C��״̬.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return I2C��״̬
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_GetStatus(I2C_Type *pI2Cx)
{
    return pI2Cx->S;
}

/*****************************************************************************//*!
*
* @brief ����ض���״̬.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_ClearStatus(I2C_Type *pI2Cx, uint8_t u8ClearFlag)
{
    pI2Cx->S |= u8ClearFlag;
}

/*****************************************************************************//*!
*
* @brief д���ݵ����ݼĴ���.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_WriteDataReg(I2C_Type *pI2Cx, uint8_t u8DataBuff)
{
    pI2Cx->D = u8DataBuff;
}

/*****************************************************************************//*!
*
* @brief �����ݼĴ�����ȡ����.
*        
* @param[in] pI2Cx       I2C�Ļ�ַ.
*
* @return I2C���ݼĴ���ֵ
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_ReadDataReg(I2C_Type *pI2Cx )
{
    return pI2Cx->D;
}

/*****************************************************************************//*!
*
* @brief ���I2Cģʽ�Ƿ�Ϊ����.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsTxMode(I2C_Type *pI2Cx )
{
    return(pI2Cx->C1 & I2C_C1_TX_MASK);
}

/*****************************************************************************//*!
*
* @brief ��������Ƿ�æ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsBusy(I2C_Type *pI2Cx )
{
    return (pI2Cx->S & I2C_S_BUSY_MASK);
}

/*****************************************************************************//*!
*
* @brief ȷ���ź��Ƿ񱻽���.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsReceivedAck(I2C_Type *pI2Cx )
{
    return (pI2Cx->S & I2C_S_RXAK_MASK);
}

/*****************************************************************************//*!
*
* @brief ���I2C�Ƿ�Ϊ����ģʽ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��.
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsMasterMode(I2C_Type *pI2Cx )
{
    return(pI2Cx->C1 & I2C_C1_MST_MASK);
}

/*****************************************************************************//*!
*
* @brief ������޵ͳ�ʱ����.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��.
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsSMB_SLTF(I2C_Type *pI2Cx )
{
    return (pI2Cx->SMB & I2C_SMB_SLTF_MASK);
}

/*****************************************************************************//*!
*
* @brief ������޸߳�ʱ����.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ��/��.
*
*****************************************************************************/
__STATIC_INLINE uint8_t I2C_IsSMB_SHTF2(I2C_Type *pI2Cx )
{
    return(pI2Cx->SMB & I2C_SMB_SHTF2_MASK);
}

/*****************************************************************************//*!
*
* @brief ���SCL�ͳ�ʱ��־.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_ClearSLTF(I2C_Type *pI2Cx )
{
    pI2Cx->SMB |= I2C_SMB_SLTF_MASK;
}

/*****************************************************************************//*!
*
* @brief ����߳�ʱ��־λ2.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void I2C_ClearSHTF2(I2C_Type *pI2Cx )
{
    pI2Cx->SMB |= I2C_SMB_SHTF2_MASK;
}

/*****************************************************************************//*!
*
* @brief �����ź�ACK.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_SendAck(I2C_Type *pI2Cx )
{
    pI2Cx->C1 &= ~I2C_C1_TXAK_MASK;
}

/*****************************************************************************//*!
*
* @brief �����ź�NACK.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_SendNack(I2C_Type *pI2Cx )
{
    pI2Cx->C1 |= I2C_C1_TXAK_MASK;
}

/*****************************************************************************//*!
*
* @brief ʹ�ܵڶ�I2C��ַ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
__STATIC_INLINE void I2C_SecondAddressEnable(I2C_Type *pI2Cx)
{
     pI2Cx->SMB |= I2C_SMB_SIICAEN_MASK;
}

/******************************************************************************
******************************************************************************/
void I2C_Init(I2C_Type *pI2Cx,I2C_ConfigPtr pI2CConfig);
uint8_t I2C_Start(I2C_Type *pI2Cx);
uint8_t I2C_Stop(I2C_Type *pI2Cx);
uint8_t I2C_RepeatStart(I2C_Type *pI2Cx);
uint8_t I2C_IsTxMode(I2C_Type *pI2Cx );
uint8_t I2C_IsBusy(I2C_Type *pI2Cx );
uint8_t I2C_IsReceivedAck(I2C_Type *pI2Cx );
uint8_t I2C_IsMasterMode(I2C_Type *pI2Cx );
void I2C_ClearSHTF2(I2C_Type *pI2Cx );
void I2C_ClearSLTF(I2C_Type *pI2Cx );
uint8_t I2C_IsSMB_SHTF2(I2C_Type *pI2Cx );
uint8_t I2C_IsSMB_SLTF(I2C_Type *pI2Cx );
void I2C_TxEnable(I2C_Type *pI2Cx);
void I2C_RxEnable(I2C_Type *pI2Cx);
void I2C_IntEnable(I2C_Type *pI2Cx);
void I2C_IntDisable(I2C_Type *pI2Cx);
void I2C_SetBaudRate(I2C_Type *pI2Cx,uint32_t u32Bps);
void I2C_SetSlaveAddress(I2C_Type *pI2Cx,uint16_t u16SlaveAddress);
void I2C_GeneralCallEnable(I2C_Type *pI2Cx);
void I2C_SMBusAlertEnable(I2C_Type *pI2Cx);
void I2C_RangeAddressEnable(I2C_Type *pI2Cx);
void I2C_SHTF2IntEnable(I2C_Type *pI2Cx);
void I2C_ETMeoutCounterClockSelect(I2C_Type *pI2Cx, uint8_t u8Clock);
void I2C_SetSCLLowETMeout(I2C_Type *pI2Cx, uint16_t u16ETMeout);
uint8_t I2C_GetStatus(I2C_Type *pI2Cx);
void I2C_ClearStatus(I2C_Type *pI2Cx, uint8_t u8ClearFlag);
void I2C_SendAck(I2C_Type *pI2Cx );
void I2C_SendNack(I2C_Type *pI2Cx );
void I2C_SecondAddressEnable(I2C_Type *pI2Cx);
void I2C_ClearStatus(I2C_Type *pI2Cx, uint8_t u8ClearFlag);
void I2C_WriteDataReg(I2C_Type *pI2Cx, uint8_t u8DataBuff);
uint8_t I2C_ReadDataReg(I2C_Type *pI2Cx );
void I2C_Deinit(I2C_Type *pI2Cx);
uint8_t I2C_WriteOneByte(I2C_Type *pI2Cx, uint8_t u8WrBuff);
uint8_t I2C_ReadOneByte(I2C_Type *pI2Cx, uint8_t *pRdBuff, uint8_t u8Ack);
uint8_t I2C_MasterSendWait(I2C_Type *pI2Cx,uint16_t u16SlaveAddress,uint8_t *pWrBuff,uint32_t u32Length);
uint8_t I2C_MasterReadWait(I2C_Type *pI2Cx,uint16_t u16SlaveAddress,uint8_t *pRdBuff,uint32_t u32Length);
void I2C0_SetCallBack( I2C_CallbackType pCallBack );
void I2C1_SetCallBack( I2C_CallbackType pCallBack );

/*! @}                              						*/

#ifdef __cplusplus
}
#endif
#endif //


