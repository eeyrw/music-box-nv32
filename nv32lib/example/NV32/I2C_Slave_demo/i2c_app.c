/******************************************************************************
*
* @brief  I2C�����շ���������
*
******************************************************************************/
#include "common.h"
#include "i2c.h"
#include "i2c_app.h"

static I2C_MasterSendType sI2C_MasterSendInfo = {0};
static I2C_MasterReceiveType sI2C_MasterReceiveInfo = {0};
static I2C_SlaveSendType sI2C_SlaveSendInfo = {0};
static I2C_SlaveReceiveType sI2C_SlaveReceiveInfo = {0};
//static uint8_t u8I2C_BusStatus = I2C_BUS_NORMAL;

/*****************************************************************************//*!
 *
 * @brief  I2Cģ��ص����ܺ���.
 *        
 * @param  none
 *
 * @return none
 *
*****************************************************************************/

void I2C_CallBack( void )
{
    I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);//����жϱ�־λ
    if( I2C_GetStatus(I2C0) & I2C_S_ARBL_MASK )
    {
        I2C_ClearStatus(I2C0,I2C_S_ARBL_MASK);
        if( !(I2C_GetStatus(I2C0) & I2C_S_IAAS_MASK) )
        {
            // IIAAS is 0
            return;
        }
    }
	if( I2C_IsSMB_SLTF(I2C0) )
	{
		/* clear this flag */
		I2C_ClearSLTF(I2C0);

		/* Scl holding low over specified counter, add your code in here */
		//u8I2C_BusStatus |= I2C_BUS_SLTF;
	}

	if( I2C_IsSMB_SHTF2(I2C0) )
	{
		/* clear this flag */
		I2C_ClearSHTF2(I2C0);

		/* Scl holding low over specified counter, add your code in here */
		//u8I2C_BusStatus |= I2C_BUS_SHTF2;
	}
	
    if( I2C_IsMasterMode(I2C0) )
    {
        I2C_MasterIntProcessing();
    }
    else
    {
        I2C_SlaveIntProcessing();
    }

}

/*****************************************************************************//*!
 *
 * @brief ��ʼ���ӻ������������Լ����ûص�����.
 *        
 * @param[in] none.
 *
 * @return    none.
 *
*****************************************************************************/

void I2C_InitGlobalVariable( void )
{
	
    sI2C_SlaveReceiveInfo.u32FreeLength = I2C_SLAVE_RECEIVE_BUFFER_LENGTH;

    I2C0_SetCallBack(I2C_CallBack);
}


/*****************************************************************************//*!
 *
 * @brief I2C�����жϷ�����.
 *        
 * @param  none.
 *
 * @return none.
 *
*****************************************************************************/
void I2C_MasterIntProcessing( void )
{
    if( I2C_IsTxMode(I2C0) )
    {
        if( sI2C_MasterSendInfo.u32Pointer < sI2C_MasterSendInfo.u32Length )
        {
            I2C_WriteDataReg(I2C0,sI2C_MasterSendInfo.u8SendBuff[sI2C_MasterSendInfo.u32Pointer++]);
        }
        else
        {
            I2C_Stop(I2C0);//I2C0->C1 &= ~I2C_C1_MST_MASK;
        }
    }
    else
    {
        /* �����Ӵӻ��������� */
        
        
        if( sI2C_MasterReceiveInfo.u32Pointer < ( sI2C_MasterReceiveInfo.u32Length -1) )
        {
            I2C_SendAck(I2C0);
            sI2C_MasterReceiveInfo.u8ReceiveBuff[sI2C_MasterReceiveInfo.u32Pointer++] =
                    I2C_ReadDataReg(I2C0);  
        }
        else if( sI2C_MasterReceiveInfo.u32Pointer == (sI2C_MasterReceiveInfo.u32Length -1) )
        {
            I2C_SendNack(I2C0);
            sI2C_MasterReceiveInfo.u8ReceiveBuff[sI2C_MasterReceiveInfo.u32Pointer++] =
                    I2C_ReadDataReg(I2C0);  
        }
        else
        {
            I2C_Stop(I2C0);
            I2C_ReadDataReg(I2C0); 
        }
         
    }
}

/*****************************************************************************//*!
 *
 * @brief I2C�ӻ��жϷ�����.
 *        
 * @param  none.
 *
 * @return none.
 *
*****************************************************************************/
void I2C_SlaveIntProcessing( void )
{
    if( I2C_GetStatus(I2C0) & I2C_S_IAAS_MASK )
    {
        I2C_SendAck(I2C0);

        if( I2C_GetStatus(I2C0)& I2C_S_SRW_MASK )
        {
            // �ӻ���������
            I2C_TxEnable(I2C0);
            sI2C_SlaveSendInfo.u32Pointer = 0;
            I2C_WriteDataReg(I2C0,sI2C_SlaveSendInfo.u8SendBuff[sI2C_SlaveSendInfo.u32Pointer++]);
           
        }
        else
        {
            I2C_RxEnable(I2C0);
            I2C_ReadDataReg(I2C0);
        }
    }
    else
    {
        if( I2C0->S & I2C_S_SRW_MASK )
        {
            // ����Ҫ��������ȡACK�ź�
            if(I2C_IsReceivedAck(I2C0))
            {
                //û�н��յ�ACK�ź�
                I2C_RxEnable(I2C0);
                I2C_ReadDataReg(I2C0);
                // ѡȡ����
            }
            else
            {
                if( sI2C_SlaveSendInfo.u32Pointer < sI2C_SlaveSendInfo.u32Length )
                {
                    I2C_WriteDataReg(I2C0,sI2C_SlaveSendInfo.u8SendBuff[sI2C_SlaveSendInfo.u32Pointer++]);
                }
                else
                {
               
                    I2C_WriteDataReg( I2C0, 0xff );
                }
            }
        }
        else
        {

            if( sI2C_SlaveReceiveInfo.u32FreeLength != 0 )
            {
                sI2C_SlaveReceiveInfo.u8ReceiveBuff[sI2C_SlaveReceiveInfo.u32Tail++] = I2C_ReadDataReg(I2C0);
                if( sI2C_SlaveReceiveInfo.u32Tail >= I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
                {
                    sI2C_SlaveReceiveInfo.u32Tail = 0;
                }
                sI2C_SlaveReceiveInfo.u32FreeLength --;

            }
            else
            {
                /* �������� */
                I2C_ReadDataReg(I2C0);
            }
       }
    }
}

/*****************************************************************************//*!
 *
 * @brief I2C�ӻ����ж�ģʽ�½�������. 
 *        
 * @param[out] pRdBuff    ָ��������ݵ��׵�ַ.  
 *
 * @return ���յ����ݵĳ���.
 *
*****************************************************************************/
uint32_t I2C_SlaveReceive( uint8_t *pRdBuff )
{
    uint32_t i;

    I2C_IntDisable(I2C0);
    i = 0;
    while( sI2C_SlaveReceiveInfo.u32FreeLength < I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
    {
        if( sI2C_SlaveReceiveInfo.u32Head != sI2C_SlaveReceiveInfo.u32Tail )
        {
            pRdBuff[i++] = sI2C_SlaveReceiveInfo.u8ReceiveBuff[sI2C_SlaveReceiveInfo.u32Head++];

            if( sI2C_SlaveReceiveInfo.u32Head >= I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
            {
                sI2C_SlaveReceiveInfo.u32Head = 0;
            }
            
            sI2C_SlaveReceiveInfo.u32FreeLength ++;
        }
    }
    I2C_IntEnable(I2C0);
    return i;
}

/*****************************************************************************//*!
 *
 * @brief I2C�ӻ����ж�ģʽ�·�������. 
 *        
 * @param[in] pRdBuff     ָ�������ݵ��׵�ַ.  
 * @param[in] uiLength    ��Ҫ�������ݵĳ���.   
 *
 * @return ���͵�״̬
 *
*****************************************************************************/
uint8_t I2C_SlaveSend( uint8_t *pWrBuff, uint32_t uiLength )
{
    if( uiLength > I2C_SLAVE_SEND_BUFFER_LENGTH )
    {
        return FALSE;
    }
    I2C_IntDisable(I2C0);
    sI2C_SlaveSendInfo.u32Length = uiLength;
    memcpy((void *)&sI2C_SlaveSendInfo.u8SendBuff[0],(void *)pWrBuff,uiLength);
    I2C_IntEnable(I2C0);

    return TRUE;
}

/*****************************************************************************//*!
 *
 * @brief ���I2Cģ����յ��˶������ݣ��ѽ��յ������ݷ��뻺����. 
 *        
 * @param[out] pRdBuff  ָ��������ݵ��׵�ַ. 
 *
 * @return ���ؽ��յ����ݵĳ���.
 *
*****************************************************************************/
uint32_t I2C_MasterCheckRead( uint8_t *pRdBuff )
{
    uint32_t uiPointer;
    
    if( sI2C_MasterReceiveInfo.u32Pointer !=0 )
    {
        memcpy((void *) pRdBuff,
        (void *)&sI2C_MasterReceiveInfo.u8ReceiveBuff[0],
        sI2C_MasterReceiveInfo.u32Pointer );
        uiPointer = sI2C_MasterReceiveInfo.u32Pointer;
        sI2C_MasterReceiveInfo.u32Pointer = 0;
        return uiPointer;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************//*!
 *
 * @brief  I2C�������ж�ģʽ�½�������. 
 *        
 * @param[in] u16Address   I2C�ӻ���ַ. 
 * @param[in] u32Length    �Ӵӻ��ж�ȡ���ݵĳ���.  
 *
 * @return ����״̬.
 *
*****************************************************************************/
uint8_t I2C_MasterRead( uint16_t u16Address,uint32_t u32Length )
{
    
    uint8_t u8Status;
    
   I2C_IntDisable(I2C0);
   
   /* ������ʼ�ź� */
   u8Status = I2C_Start(I2C0);

   if( u8Status == I2C_ERROR_NULL )
   {
       u8Status = I2C_WriteOneByte(I2C0,(uint8)((u16Address<<1)|0x01));
       if( u8Status == I2C_ERROR_NULL )
       {
           
     
            sI2C_MasterReceiveInfo.u32Length = u32Length;
            sI2C_MasterReceiveInfo.u32Pointer = 0;
            I2C_SendAck(I2C0);
            I2C_RxEnable(I2C0);
            I2C_ReadDataReg(I2C0);
       }
   }

   I2C_IntEnable(I2C0);
   
   return u8Status;
}

/*****************************************************************************//*!
 *
 * @brief I2C�������ж�ģʽ�·�������. 
 * @param[in]   u16Address   I2C�ӻ���ַ. 
 * @pWrBuff[in] pWrBuff      ָ����Ҫ�������ݵ��׵�ַ. 
 * @param[in]   u32Length    Ҫ��ӻ������ݳ���.  
 *
 * @return ����״̬
 *
*****************************************************************************/
uint8_t I2C_MasterSend( uint16_t Address,uint8_t *pWrBuff, uint32_t u32Length )
{
    uint8_t u8Status;

    if( I2C_IsBusy(I2C0) )
    {
        return I2C_ERROR_BUS_BUSY;
    }

    I2C_IntDisable(I2C0);
    
    /* ������ʼ�ź� */
    u8Status = I2C_Start(I2C0);
    I2C_TxEnable(I2C0);
    if( u8Status == I2C_ERROR_NULL )
    {
        u8Status = I2C_WriteOneByte(I2C0,(uint8)((Address<<1)&0xfe));
        if( u8Status == I2C_ERROR_NULL )
        {
            memcpy((void *)&sI2C_MasterSendInfo.u8SendBuff[0],
                    (void *)pWrBuff,u32Length); 
   
        }
    }
    sI2C_MasterSendInfo.u32Pointer = 0;
    sI2C_MasterSendInfo.u32Length = u32Length;
    I2C_IntEnable(I2C0);
    
    I2C_WriteDataReg(I2C0, sI2C_MasterSendInfo.u8SendBuff[sI2C_MasterSendInfo.u32Pointer++] );

    return u8Status;

}




