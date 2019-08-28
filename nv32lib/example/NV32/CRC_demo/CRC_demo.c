/**********************************************************************
 *
 * ʵ�����ƣ�CRCѭ������У��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ѡ��CRCУ��ģʽ���趨����ֵ������ʽ����д���ֵ�ת�����͵ȣ�
 *           ����ҪУ������ݣ���������У���루У��ͣ�
 *					 
************************************************************************/

#include "common.h"
#include "crc.h"
#include "uart.h"
#include "sysinit.h"

int main (void);

/********************************************************************/
int main (void)
{   
    uint32_t        u32Crc_ConverterResult;
    uint32_t        u32SeedValue;  
    CRC_ConfigType  sCRC_ConfigType = {0};
    CRC_ConfigType  *pCRC_Config=&sCRC_ConfigType;

    uint8_t MessageSource[] = {"123456789"} ;                      /*!< ҪУ�������*/  

    /*ϵͳ��ʼ��*/
    sysinit();

    printf("\nRunning the CRC_demo project.\r\n");          
    /*����CRC�Ĵ�����ʹ�乤����16CRCģʽ */ 
    pCRC_Config->u32PolyData            = 0x1021;                   /*!< 16λ CRC-CCITT ����ʽ */ 
    u32SeedValue                        = 0xFFFF;                   /*!< �趨����ֵ������ͳ�ʼֵ�� */     
    pCRC_Config->bWidth                 = CRC_WIDTH_16BIT;          /*!< ѡ��16λCRCЭ�� */    
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_NONE;  /*!< ��ȡ��CRC���ݼĴ�����ֵ��ת�� */      
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_NONE; /*!< д�뵽CRC���ݼĴ�����ֵ��ת�� */ 
      
    CRC_Init(pCRC_Config);                                          /*!< ��ʼ��CRCģ��*/  
    
    printf("CRC0->GPOLY=0x%x, CRC0->CTRL=0x%x\r\n",CRC0->GPOLY,CRC0->CTRL);
    /*��ʼ��16λģʽCRC���㣬����У���*/
    u32Crc_ConverterResult = CRC_Cal16(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC-CCITT function calculation result = 0x%x @seed = 0x%x .\r\n", u32Crc_ConverterResult, u32SeedValue );
    
    /* ��λCRCģ��*/
    CRC_DeInit();  
    
    /*����CRC�Ĵ�����ʹ�乤����32λģʽ*/
    pCRC_Config->u32PolyData            = 0x04C11DB7;               /*!< ����CRC32 ����ʽ*/      
    u32SeedValue                        = 0xFFFFFFFF;               /*!< �趨����ֵ������ͳ�ʼֵ�� */   
    pCRC_Config->bWidth                 = CRC_WIDTH_32BIT;          /*!< ѡ��32λCRCЭ�� */   
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_ALL;  	/*!< ��ȡ���ݼĴ�����ֵ���ֽ��е�λ���ֽھ�ת�� */     
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_BIT;  /*!< д����ʱ���ֽ��е�λת�ã��ֽڲ�ת�� */ 
    pCRC_Config->bFinalXOR              = TRUE;                     /*!< ��תCRC���ݼĴ����Ķ�ȡֵ*/
      
    CRC_Init(pCRC_Config);                                          /*!< ��ʼ��CRCģ�� */  
    /*��ʼ����32λCRC���㣬����У���*/		
    u32Crc_ConverterResult = CRC_Cal32(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC32 function calculation result = 0x%x @seed = 0x%x .\r\n", u32Crc_ConverterResult, u32SeedValue );

    
    while(1);
}
