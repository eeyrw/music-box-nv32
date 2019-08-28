/**********************************************************************
 *
 * 实验名称：CRC循环冗余校验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：选择CRC校验模式，设定种子值、多项式、读写数字的转置类型等，
 *           输入要校验的数据，最终生成校验码（校验和）
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

    uint8_t MessageSource[] = {"123456789"} ;                      /*!< 要校验的数据*/  

    /*系统初始化*/
    sysinit();

    printf("\nRunning the CRC_demo project.\r\n");          
    /*配置CRC寄存器，使其工作在16CRC模式 */ 
    pCRC_Config->u32PolyData            = 0x1021;                   /*!< 16位 CRC-CCITT 多项式 */ 
    u32SeedValue                        = 0xFFFF;                   /*!< 设定种子值（检验和初始值） */     
    pCRC_Config->bWidth                 = CRC_WIDTH_16BIT;          /*!< 选择16位CRC协议 */    
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_NONE;  /*!< 读取的CRC数据寄存器的值不转置 */      
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_NONE; /*!< 写入到CRC数据寄存器的值不转置 */ 
      
    CRC_Init(pCRC_Config);                                          /*!< 初始化CRC模块*/  
    
    printf("CRC0->GPOLY=0x%x, CRC0->CTRL=0x%x\r\n",CRC0->GPOLY,CRC0->CTRL);
    /*开始进16位模式CRC计算，生成校验和*/
    u32Crc_ConverterResult = CRC_Cal16(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC-CCITT function calculation result = 0x%x @seed = 0x%x .\r\n", u32Crc_ConverterResult, u32SeedValue );
    
    /* 复位CRC模块*/
    CRC_DeInit();  
    
    /*配置CRC寄存器，使其工作在32位模式*/
    pCRC_Config->u32PolyData            = 0x04C11DB7;               /*!< 设置CRC32 多项式*/      
    u32SeedValue                        = 0xFFFFFFFF;               /*!< 设定种子值（检验和初始值） */   
    pCRC_Config->bWidth                 = CRC_WIDTH_32BIT;          /*!< 选择32位CRC协议 */   
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_ALL;  	/*!< 读取数据寄存器的值，字节中的位和字节均转置 */     
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_BIT;  /*!< 写数据时，字节中的位转置，字节不转置 */ 
    pCRC_Config->bFinalXOR              = TRUE;                     /*!< 反转CRC数据寄存器的读取值*/
      
    CRC_Init(pCRC_Config);                                          /*!< 初始化CRC模块 */  
    /*开始进行32位CRC计算，生成校验和*/		
    u32Crc_ConverterResult = CRC_Cal32(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC32 function calculation result = 0x%x @seed = 0x%x .\r\n", u32Crc_ConverterResult, u32SeedValue );

    
    while(1);
}
