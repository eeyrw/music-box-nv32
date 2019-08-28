/******************************************************************************
*
* @brief  CRC ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef CRC_H_
#define CRC_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/**********************************************************************!
 * @brief CRC���ƼĴ���λ����
 *
 *******************************************************************/

#define CRC_WIDTH_16BIT                 0	/*!< ѡ��16λCRCЭ�� */   
#define CRC_WIDTH_32BIT                 1	/*!< ѡ��32λCRCЭ�� */ 
#define CRC_DATA_SEED                   1	/*!< д��CRC���ݼĴ�����ֵΪ����ֵ */  
#define CRC_DATA_DATA                   0	/*!< д��CRC���ݼĴ�����ֵΪ����ֵ */ 
#define CRC_READ_COMPLETE               1	/*!< ��ת�򲹳�CRC���ݼĴ����Ķ�ȡֵ */ 
#define CRC_READ_NONE                   0	/*!< ��ȡ���ݼĴ���ʱ��ִ���������*/ 
#define CRC_READ_TRANSPOSE_NONE         0	/*!< ��ȡ���ݼĴ�����ֵ��ת�� */ 
#define CRC_READ_TRANSPOSE_BIT          1	/*!< ��ȡ���ݼĴ�����ֵ���ֽ��е�λת�ã��ֲ�ת��*/
#define CRC_READ_TRANSPOSE_ALL          2	/*!< ��ȡ���ݼĴ�����ֵ���ֽ��е�λ���ֽھ�ת�� */
#define CRC_READ_TRANSPOSE_BYTE         3	/*!< ��ȡ���ݼĴ�����ֵ�����ֽ�ת�ã��ֽ��е�λ��ת�� */
#define CRC_WRITE_TRANSPOSE_NONE        0	/*!< д����ʱ��ת�� */
#define CRC_WRITE_TRANSPOSE_BIT         1	/*!< д����ʱ���ֽ��е�λת�ã��ֽڲ�ת�� */ 
#define CRC_WRITE_TRANSPOSE_ALL         2	/*!< д����ʱ���ֽ��е�λ���ֽھ�ת�� */
#define CRC_WRITE_TRANSPOSE_BYTE        3	/*!< д����ʱ�����ֽ�ת�ã��ֽ��е�λ��ת�� */

 
/******************************************************************************
*
* CRC ���ýṹ������.
*
*******************************************************************************/ 

typedef struct
{          
    uint8_t bWidth                  : 1;    /*!< 1: 32λCRCЭ��  0: 16λCRCЭ�� */
    uint8_t bDataType               : 1;    /*!< 1: д������ֵ , 0: д������ */
    uint8_t bFinalXOR               : 1;    /*!< 1: �������Ƿ�ת�򲹳� , 0: ������ʱ��ִ�����*/
    uint8_t bRESERVED               : 1;    /*!< ����λ */  
    uint8_t bTransposeReadType      : 2;    /*!< ��ȡ����ʱ��ת������, ���Ĳο��ֲ� */
    uint8_t bTransposeWriteType     : 2;    /*!< д����ʱ��ת������, ���Ĳο��ֲ� */      
    uint32_t u32PolyData               ;    /*!< 32λ��16λ����ʽ*/           
} CRC_ConfigType, *CRC_ConfigPtr  ; 


/******************************************************************************/

void        CRC_Init(CRC_ConfigType *pConfig);
uint32_t    CRC_Cal16(uint32_t u32Seed, uint8_t *msg, uint32_t u32SizeBytes);
uint32_t    CRC_Cal32(uint32_t u32Seed, uint8_t *msg, uint32_t u32SizeBytes);
void        CRC_DeInit(void);


#ifdef __cplusplus
}
#endif
#endif 



