/**********************************************************************
 *
 * 实验名称：FLASH擦写
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：使用FLASH模拟EEPROM,实现字节的编程
 * 实验说明：NV32F100x系列芯片不带有硬件EEPROM，
 *					除128K FLASH空间外，还带有5K的NVR空间:0x0040_0000-0x0040_13FF
 *					操作与前128K一致，不过该区域在执行整片擦除时不会被擦除，可供用户用来作为数据存储区域
 *
************************************************************************/

#include "common.h"
#include "rtc.h"
#include "flash.h"
#include "sysinit.h"
#include "eeprom.h"

int main (void);

/********************************************************************/
int main (void)
{ 
    uint32_t i;
    uint8_t  u8DataBuff[512];

		sysinit();
  	printf("\nRunning the Flash_demo project.\n");
    LED0_Init();  
    LED2_Init();
    Flash_Init();
    
    /* 擦除第63个扇区*/
    Flash_EraseSector(63*FLASH_SECTOR_SIZE);
    
    for(i=0;i<512;i++)
    {
        u8DataBuff[i] = (uint8_t)i;
    }
/****************对128K内的FLASH进行读写操作****************/
		
    /*向擦除的扇区写入数据*/
    Flash_Program(63*FLASH_SECTOR_SIZE,&u8DataBuff[0],512);
   
   for(int  i=0;i<512/16;i++ )
   {
       for(int ch =0;ch<16;ch++)
       {
           printf("0x%x,",*((uint8_t *)(i*16+ch+63*FLASH_SECTOR_SIZE)));
       }
       printf("\n");
   }

		
/****************Flash模拟EEPROM操作****************/
/*		
 *
 *模拟EEPROM的首地址：0x00401000(NVR空间)
 *模拟EEPROM的大小  ：1K
 *模拟EEPROM第一次使用前，可以先进行擦除擦做，这样可以提高写的效率
 *
**********************************************************************************/
 
    
  EEPROM_Erase(0x00);
  EEPROM_Erase(0x200);   //建议在最开始进行erase操作

//以字为单位写，空间大小为1K，调用EEPROM_Write函数
  for(i=0;i<256;i++)
  {
		EEPROM_Write(4*i,1024-4*i);
  }
    //对该范围内的任意地址再写数据
    EEPROM_Write(24,0xaa5555aa);
    EEPROM_Write(68,0x55aaaa55);
    EEPROM_Write(128,0x3fec);
    EEPROM_Write(156,0xccbb);
    EEPROM_Write(256,0xbbcc);
    EEPROM_Write(264,0xccdd);
    EEPROM_Write(300,0x3fff);
    EEPROM_Write(512,0x5f5f);
    EEPROM_Write(900,0x9f9f);
    
	for(i=0;i<256;i++)
  {
    printf("adr:%d =0x%x \n",4*i,EEPROM_Read(4*i));
  }
    
//批量写1-2个扇区
  printf("/***************************批量测试**************************/ \n");
  
//地址偏移量为340，写一个扇区的数据
	EERPOM_Writeup4byte(340,u8DataBuff,512);
 
  /*for(i=0;i<256;i++)
  {
  printf("adr  %d =0x%x \n",4*i,EEPROM_Read(4*i));
  }  
  */ 
  
  EEPROM_Erase(0x00);
  EEPROM_Erase(0x200);  

//byte字节写,byte字节读取

  printf("/***************************byte写测试**************************/ \n"); 
  EEPROM_WriteByte(132,0x13);
  printf("adr :132 =0x%x \n",EEPROM_Read(132)); 
  EEPROM_WriteByte(133,0x11);
  printf("adr :132 =0x%x \n",EEPROM_Read(132)); 
  EEPROM_WriteByte(132,0x14);
  printf("adr :132 =0x%x \n",EEPROM_Read(132)); 
  EEPROM_WriteByte(132,0x12);
  printf("adr :132 =0x%x \n",EEPROM_ReadByte(132)); 
  EEPROM_WriteByte(132,0x15);
  printf("adr :132 =0x%x \n",EEPROM_ReadByte(132)); 
    
  while(1); 
}

