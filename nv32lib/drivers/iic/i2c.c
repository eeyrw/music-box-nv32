/*********************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       i2c.c
 * @brief      i2cͨѶ�ӿں�����
 * @author     Navota
 * @date       2017-1-1
 ************************************************************************/
#include "common.h"
#include "i2c.h"


/*!
 * @brief ��Żص����
 *
 */
static I2C_CallbackType I2C_Callback[2] = {(I2C_CallbackType)NULL};

void I2C0_Isr( void );

/******************************************************************************
* ����I2C�Ľӿں���
*******************************************************************************/
/*****************************************************************************//*!
*
* @brief ��ʼ��I2Cģ��.
*        
* @param[in] pI2Cx        I2C�Ļ�ַ.
* @param[in] pI2CConfig   ����I2C�Ľṹ��.
*
* @return none
*
*****************************************************************************/

void I2C_Init(I2C_Type *pI2Cx,I2C_ConfigPtr pI2CConfig)
{
    uint8_t u8Temp;

#if defined(CPU_NV32)
    SIM->SCGC |= SIM_SCGC_IIC_MASK;
#endif

	I2C_SetBaudRate(pI2Cx,pI2CConfig->u16F);
    I2C_SetSlaveAddress(pI2Cx,pI2CConfig->u16OwnA1);
    pI2Cx->FLT = (uint8_t)pI2CConfig->u16Filt;
    pI2Cx->RA = (uint8_t)pI2CConfig->u16RangeA & 0xfe;
    I2C_SetSCLLowETMeout(pI2Cx,pI2CConfig->u16Slt);

    /* ���ÿ��ƼĴ���C2 */
    u8Temp = 0;
    if( pI2CConfig->sSetting.bGCAEn )
    {
        u8Temp |= I2C_C2_GCAEN_MASK;
    }
    if( pI2CConfig->sSetting.bAddressExt )
    {
        u8Temp |= I2C_C2_ADEXT_MASK;
    }
    if( pI2CConfig->sSetting.bRangeAddEn )
    {
        u8Temp |= I2C_C2_RMEN_MASK;
    }
    pI2Cx->C2 |= u8Temp;

    /* ���üĴ���SMB */
    u8Temp = 0;
    if( pI2CConfig->sSetting.bFackEn )
    {
        u8Temp |= I2C_SMB_FACK_MASK;
    }
    if( pI2CConfig->sSetting.bSMB_AlertEn )
    {
        u8Temp |= I2C_SMB_ALERTEN_MASK;
    }
    if( pI2CConfig->sSetting.bSecondAddressEn )
    {
        u8Temp |= I2C_SMB_SIICAEN_MASK;
    }
    if( pI2CConfig->sSetting.bSHTF2IntEn )
    {
        u8Temp |= I2C_SMB_SHTF2IE_MASK;
    }
    pI2Cx->SMB = u8Temp;
    
    /* ���üĴ���C1 */
    u8Temp = 0;
    if( pI2CConfig->sSetting.bIntEn )
    {
        u8Temp |= I2C_C1_IICIE_MASK;
        if(pI2Cx == I2C0)
        {
            NVIC_EnableIRQ(I2C0_IRQn);
        }   
        else
        {
            //
        }
    }
    if( pI2CConfig->sSetting.bWakeUpEn )
    {
        u8Temp |= I2C_C1_WUEN_MASK;
    }
    if( pI2CConfig->sSetting.bI2CEn )
    {
        u8Temp |= I2C_C1_IICEN_MASK;
    }
    pI2Cx->C1 = u8Temp;


}
/*****************************************************************************//*!
*
* @brief ����I2C��ʼ�źţ��������䣩.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ����״̬
*
*****************************************************************************/
uint8_t I2C_Start(I2C_Type *pI2Cx)
{
    uint32_t u32ETMeout;
    uint8_t u8ErrorStatus;
    
    u32ETMeout = 0; 
    u8ErrorStatus = 0x00;

    I2C_TxEnable(pI2Cx);//�� I2C ���ó� TX ����ģʽ
    pI2Cx->C1 |= I2C_C1_MST_MASK;//�� I2C ���ó�����ģʽ
      
    //���������ʼλ��û�з��ͳɹ�
    while( (!I2C_IsBusy(pI2Cx)) && ( u32ETMeout < I2C_WAIT_STATUS_ETMEOUT))
     {
        u32ETMeout ++;
     }

     if( u32ETMeout == I2C_WAIT_STATUS_ETMEOUT )
     {
        u8ErrorStatus |= I2C_ERROR_START_NO_BUSY_FLAG;
     }

     return u8ErrorStatus;//���ش����־��Ϊ0x10����ʼ�ź�û�з��ͳɹ�
}

/*****************************************************************************//*!
*
* @brief ����ֹͣ�ź�.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ����״̬
*
*****************************************************************************/
uint8_t I2C_Stop(I2C_Type *pI2Cx)
{
    uint32_t u32ETMeout;
    uint8_t u8ErrorStatus;
    
    u32ETMeout = 0; 
    u8ErrorStatus = 0x00;
    
    pI2Cx->C1 &= ~I2C_C1_MST_MASK;
   //������� I2C ֹͣλ�Ƿ��ͳɹ�
    while( (I2C_IsBusy(pI2Cx) ) && ( u32ETMeout < I2C_WAIT_STATUS_ETMEOUT))
     {
        u32ETMeout ++;
     }

     if( u32ETMeout == I2C_WAIT_STATUS_ETMEOUT )
     {
        u8ErrorStatus |= I2C_ERROR_STOP_BUSY_FLAG;
     }

     return u8ErrorStatus;
}


/*****************************************************************************//*!
*
* @brief �����ظ���ʼλ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return ����״̬.
*
*****************************************************************************/
uint8_t I2C_RepeatStart(I2C_Type *pI2Cx)
{
    uint32_t u32ETMeout;
    uint8_t u8ErrorStatus;
    
    u32ETMeout = 0; 
    u8ErrorStatus = 0x00;
    
    pI2Cx->C1 |= I2C_C1_RSTA_MASK;
    //���������ʼλ�Ƿ��ͳɹ�
    while( (!I2C_IsBusy(I2C0) ) && ( u32ETMeout < I2C_WAIT_STATUS_ETMEOUT))
     {
        u32ETMeout ++;
     }
       
     if( u32ETMeout == I2C_WAIT_STATUS_ETMEOUT )
     {
        u8ErrorStatus |= I2C_ERROR_START_NO_BUSY_FLAG;
     }

     return u8ErrorStatus;
}

/*****************************************************************************//*!
*
* @brief ���ôӻ���ַ.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
* @param[in] u16SlaveAddress      �ӻ���ַ.
*
* @return none
*
*****************************************************************************/
void I2C_SetSlaveAddress(I2C_Type *pI2Cx,uint16_t u16SlaveAddress)
{
    /* д��8λ��ַ */
    pI2Cx->A1 = (uint8_t)u16SlaveAddress;

    /* ���֧��ʮλ�ӻ���ַ�� д�����λ��ַ */
    pI2Cx->C2 &= ~I2C_C2_AD_MASK;
    pI2Cx->C2 |= (uint8_t)(u16SlaveAddress>>8)&0x03;
}

/*****************************************************************************//*!
*
* @brief ����I2C�ж�.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
void I2C_IntDisable(I2C_Type *pI2Cx)
{
    pI2Cx->C1 &= ~I2C_C1_IICIE_MASK;
    if(pI2Cx == I2C0)
    {
        NVIC_DisableIRQ(I2C0_IRQn);
    }
    else
    {
        
    }
}
/*****************************************************************************//*!
*
* @brief ʹ��I2C�ж�.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
void I2C_IntEnable(I2C_Type *pI2Cx)
{
    pI2Cx->C1 |= I2C_C1_IICIE_MASK;
    if(pI2Cx == I2C0)
    {
        NVIC_EnableIRQ(I2C0_IRQn);
    }
    else
    {
        
    }
}

/*****************************************************************************//*!
*
* @brief ����SCL�ͳ�ʱ����.
*        
* @param[in] pI2Cx      I2C�Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
void I2C_SetSCLLowETMeout(I2C_Type *pI2Cx, uint16_t u16ETMeout)
{
    pI2Cx->SLTL = (uint8_t)u16ETMeout;
    pI2Cx->SLTH = (uint8_t)(u16ETMeout>>8);
}

/*****************************************************************************//*!
*
* @brief ��λI2Cģ��.
*        
* @param[in] pI2Cx    I2C�Ļ�ַ.
*
* @return none
*
*****************************************************************************/
void I2C_Deinit(I2C_Type *pI2Cx)
{
     pI2Cx->C1 &= ~I2C_C1_IICEN_MASK;
#if defined(CPU_NV32)
    SIM->SCGC &= ~SIM_SCGC_IIC_MASK;
#endif
}

/*****************************************************************************//*!
*
* @brief ���͵��ֽ�����.
*        
* @param[in] pI2Cx         I2Cģ��Ļ�ַ.
* @param[in] u8WrBuff      Ҫд�����ݻ�����. 
*
* @return ����״̬
*
*****************************************************************************/

uint8_t I2C_WriteOneByte(I2C_Type *pI2Cx, uint8_t u8WrBuff)
{
    uint32_t u32ETMeout;
    uint8_t u8ErrorStatus;
    
    u32ETMeout = 0; 
    u8ErrorStatus = 0x00;
    while (((I2C_GetStatus(pI2Cx)&I2C_S_TCF_MASK) !=  I2C_S_TCF_MASK)
            && (u32ETMeout<I2C_WAIT_STATUS_ETMEOUT))
    {
        u32ETMeout ++;
    }
    if (u32ETMeout >= I2C_WAIT_STATUS_ETMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_TCF_FLAG; 
        return u8ErrorStatus;
    }
    
    I2C_TxEnable(pI2Cx);  //�� I2C ���ó� TX ���ģʽ
    I2C_WriteDataReg(pI2Cx,u8WrBuff); //д���ݼĴ�����������

    u32ETMeout = 0;
    while (((I2C_GetStatus(pI2Cx)&I2C_S_IICIF_MASK) !=  I2C_S_IICIF_MASK)
            && (u32ETMeout<I2C_WAIT_STATUS_ETMEOUT))
    {
        u32ETMeout ++;
    }
    if (u32ETMeout >= I2C_WAIT_STATUS_ETMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_IICIF_FLAG; 
        return u8ErrorStatus;
    }

    /* ����жϱ�־λ */
    I2C_ClearStatus(pI2Cx,I2C_S_IICIF_MASK);
    if (I2C_GetStatus(pI2Cx) & I2C_S_RXAK_MASK) 
    {
        u8ErrorStatus |= I2C_ERROR_NO_GET_ACK;
    }
    return u8ErrorStatus;
}
/*****************************************************************************//*!
*
* @brief ��ȡ���ֽ�����.
*        
* @param[in] pI2Cx        I2C�Ļ�ַ.
* @param[out] pRdBuff     ��Ҫ�Ӵӻ����ĵ�ַ.
* @param[out] u8Ack       ���� ack or nack.
*
* @return ����״̬
*
*****************************************************************************/

uint8_t I2C_ReadOneByte(I2C_Type *pI2Cx, uint8_t *pRdBuff, uint8_t u8Ack)
{
    uint32_t u32ETMeout;
    uint8_t u8ErrorStatus;
    
    u32ETMeout = 0; 
    u8ErrorStatus = 0x00;
    while (((I2C_GetStatus(pI2Cx)&I2C_S_TCF_MASK) !=  I2C_S_TCF_MASK)
            && (u32ETMeout<I2C_WAIT_STATUS_ETMEOUT))
    {
        u32ETMeout ++;
    }
    if (u32ETMeout >= I2C_WAIT_STATUS_ETMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_TCF_FLAG; 
        return u8ErrorStatus;
    }
    
    I2C_RxEnable(pI2Cx); //�� I2C ����Ϊ����ģʽ

    if( u8Ack )
    {
        /*  ���� nack  */
        I2C_SendNack(pI2Cx);
       
    }
    else
    {
         /*  ���� ack  */
        I2C_SendAck(pI2Cx);
    }
    *pRdBuff = I2C_ReadDataReg(pI2Cx);   //�����������ݴ�ŵ�������ͷ

    u32ETMeout = 0;
     //��������жϱ�־
    while (((I2C_GetStatus(pI2Cx)&I2C_S_IICIF_MASK) !=  I2C_S_IICIF_MASK)
            && (u32ETMeout<I2C_WAIT_STATUS_ETMEOUT))
    {
        u32ETMeout ++;
    }
    if (u32ETMeout >= I2C_WAIT_STATUS_ETMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_IICIF_FLAG; 
        return u8ErrorStatus;
    }

    /*��� IIC �жϱ�־λ */
    I2C_ClearStatus(pI2Cx,I2C_S_IICIF_MASK);

    return u8ErrorStatus;
}
/*****************************************************************************//*!
*
* @brief �������ݵ�I2C,Ȼ��ȴ����ݴ������.
*   
* @param[in]  pI2Cx                   I2C�Ļ�ַ.
* @param[in]  u16SlaveAddress         16λ�ӻ���ַ. 
* @param[in]  pWrBuff                 ��Ҫ�������ݵĻ�������. 
* @param[in]  u32Length               �����ֽڵ���Ŀ.
*
* @return ����״̬
*
*****************************************************************************/

uint8_t I2C_MasterSendWait(I2C_Type *pI2Cx,uint16_t u16SlaveAddress,uint8_t *pWrBuff,uint32_t u32Length)
{
    uint32_t i;
    uint8_t u8ErrorStatus;

    /* ������ʼ�ź� */
    u8ErrorStatus = I2C_Start(pI2Cx);

    /* ���ӻ����ʹӻ���ַ */
    u8ErrorStatus = I2C_WriteOneByte(pI2Cx,((uint8_t)u16SlaveAddress<<1) | I2C_WRITE);

    /* ���û�д������� ����������ֽ�*/
    if( u8ErrorStatus == I2C_ERROR_NULL )
    {
        for(i=0;i<u32Length;i++)
        {
            u8ErrorStatus = I2C_WriteOneByte(pI2Cx,pWrBuff[i]);
            if( u8ErrorStatus != I2C_ERROR_NULL )
            {
                return u8ErrorStatus;
            }
        }
     }

    /*���� I2C ֹͣλ */
     u8ErrorStatus = I2C_Stop(pI2Cx);

     return u8ErrorStatus;
            
}
/*****************************************************************************//*!
*
* @brief ��ȡ����ֽڲ��ȴ����.
*        
* @param[in] pI2Cx                 I2C�Ļ�ַ.
* @param[in]  u16SlaveAddress      �ӻ���ַ. 
* @param[in]  pRdBuff              ���ڽ������ݵĻ�����. 
* @param[in]  u32Length            �������ݵ��ֽ���.           
*
* @return ����״̬
*
*****************************************************************************/

uint8_t I2C_MasterReadWait(I2C_Type *pI2Cx,uint16_t u16SlaveAddress,uint8_t *pRdBuff,uint32_t u32Length)
{
    uint32_t i;
    uint8_t u8ErrorStatus;

    /* ������ʼ�źŵ����� */
    u8ErrorStatus = I2C_Start(pI2Cx);

    /* ���ӻ�����������ַ */
    u8ErrorStatus = I2C_WriteOneByte(pI2Cx,((uint8_t)u16SlaveAddress<<1) | I2C_READ);

    /* ���û�д������� ����������ֽ�*/
    I2C_ReadOneByte(pI2Cx,&pRdBuff[0],I2C_SEND_ACK);
    
    if( u8ErrorStatus == I2C_ERROR_NULL )
    {
        for(i=0;i<u32Length-1;i++)
        {
            u8ErrorStatus = I2C_ReadOneByte(pI2Cx,&pRdBuff[i],I2C_SEND_ACK);
            if( u8ErrorStatus != I2C_ERROR_NULL )
            {
                return u8ErrorStatus;
            }
        }
        u8ErrorStatus = I2C_ReadOneByte(pI2Cx,&pRdBuff[i],I2C_SEND_NACK);
     }
    /*����ֹͣ�ź� */
     u8ErrorStatus = I2C_Stop(pI2Cx);
     
     return u8ErrorStatus;
            
}

/*****************************************************************************//*!
*
* @brief ����I2Cģ����жϻص�����
*        
* @param[in]   pfnCallback   �ص������ĵ�ַ
*
* @return none 
*
*****************************************************************************/

void I2C1_SetCallBack( I2C_CallbackType pCallBack )
{
    I2C_Callback[1] = pCallBack;
}

/*****************************************************************************//*!
*
* @brief ����RTCģ����жϻص�����
*        
* @param[in]   pfnCallback   �ص������ĵ�ַ
*
* @return none 
*
*****************************************************************************/

void I2C0_SetCallBack( I2C_CallbackType pCallBack )
{
    I2C_Callback[0] = pCallBack;
}
/*! @}                          						*/


/*****************************************************************************//*!
*
* @brief I2C0 �жϷ�����.
*        
* @param  
*
* @return none
*
*****************************************************************************/
void I2C0_Isr( void )
{
    if( I2C_Callback[0] )
    {
        I2C_Callback[0]();
    }
}

/*****************************************************************************//*!
*
* @brief I2C1 �жϷ�����.
*        
* @param  
*
* @return none
*
*****************************************************************************/
void I2C1_Isr( void )
{
    if( I2C_Callback[1] )
    {
        I2C_Callback[1]();
    }
}



