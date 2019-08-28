/******************************************************************************
*
* @brief  I2C数据收发驱动函数
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
 * @brief  I2C模块回调功能函数.
 *        
 * @param  none
 *
 * @return none
 *
*****************************************************************************/

void I2C_CallBack( void )
{
    I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);//清除中断标志位
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
 * @brief 初始化从机接收区长度以及设置回调函数.
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
 * @brief I2C主机中断服务框架.
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
        /* 主机从从机接收数据 */
        
        
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
 * @brief I2C从机中断服务框架.
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
            // 从机发送数据
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
            // 若需要从主机获取ACK信号
            if(I2C_IsReceivedAck(I2C0))
            {
                //没有接收到ACK信号
                I2C_RxEnable(I2C0);
                I2C_ReadDataReg(I2C0);
                // 选取接收
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
                /* 接收器满 */
                I2C_ReadDataReg(I2C0);
            }
       }
    }
}

/*****************************************************************************//*!
 *
 * @brief I2C从机在中断模式下接收数据. 
 *        
 * @param[out] pRdBuff    指向接收数据的首地址.  
 *
 * @return 接收到数据的长度.
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
 * @brief I2C从机在中断模式下发送数据. 
 *        
 * @param[in] pRdBuff     指向发送数据的首地址.  
 * @param[in] uiLength    所要发送数据的长度.   
 *
 * @return 发送的状态
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
 * @brief 检测I2C模块接收到了多少数据，把接收到的数据放入缓冲区. 
 *        
 * @param[out] pRdBuff  指向接收数据的首地址. 
 *
 * @return 返回接收到数据的长度.
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
 * @brief  I2C主机在中断模式下接收数据. 
 *        
 * @param[in] u16Address   I2C从机地址. 
 * @param[in] u32Length    从从机中读取数据的长度.  
 *
 * @return 错误状态.
 *
*****************************************************************************/
uint8_t I2C_MasterRead( uint16_t u16Address,uint32_t u32Length )
{
    
    uint8_t u8Status;
    
   I2C_IntDisable(I2C0);
   
   /* 发送起始信号 */
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
 * @brief I2C主机在中断模式下发送数据. 
 * @param[in]   u16Address   I2C从机地址. 
 * @pWrBuff[in] pWrBuff      指向所要发送数据的首地址. 
 * @param[in]   u32Length    要向从机的数据长度.  
 *
 * @return 错误状态
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
    
    /* 发送起始信号 */
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




