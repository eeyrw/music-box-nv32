/******************************************************************************
*
* @brief I2C�����շ����������ӿ�. 
*
*******************************************************************************/
#ifndef _I2C_APP_H__
#define _I2C_APP_H__

#define I2C_MASTER_SEND_BUFFER_LENGTH           64
#define I2C_MASTER_RECEIVE_BUFFER_LENGTH        64
#define I2C_SLAVE_SEND_BUFFER_LENGTH            64
#define I2C_SLAVE_RECEIVE_BUFFER_LENGTH         65

/*!
 * @brief ����I2C����������������.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8SendBuff[I2C_MASTER_SEND_BUFFER_LENGTH];
}I2C_MasterSendType;

/*!
 * @brief ����I2C����������������.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8ReceiveBuff[I2C_MASTER_RECEIVE_BUFFER_LENGTH];
}I2C_MasterReceiveType;

/*!
 * @brief ����I2C�ӻ�������������.
 *
 */
typedef struct
{
    uint32_t u32Head;
    uint32_t u32Tail;
    uint32_t u32FreeLength;
    uint8_t u8ReceiveBuff[I2C_SLAVE_RECEIVE_BUFFER_LENGTH];
}I2C_SlaveReceiveType;
/*!
 * @brief ����I2C�ӻ�������������.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8SendBuff[I2C_SLAVE_SEND_BUFFER_LENGTH];
}I2C_SlaveSendType;

/******************************************************************************
******************************************************************************/
void I2C_CallBack( void );
void I2C_InitGlobalVariable( void );
void I2C_MasterIntProcessing( void );
void I2C_SlaveIntProcessing( void );
uint32_t I2C_SlaveReceive( uint8_t *pRdBuff );
uint8_t I2C_SlaveSend( uint8_t *pWrBuff, uint32_t uiLength );
uint32_t I2C_MasterCheckRead( uint8_t *pRdBuff );
uint8_t I2C_MasterRead( uint16_t u16Address,uint32_t u32Length );
uint8_t I2C_MasterSend( uint16_t Address,uint8_t *pWrBuff, uint32_t u32Length );

#endif //


