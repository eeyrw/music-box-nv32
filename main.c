/**********************************************************************
 *
 * ʵ�����ƣ�FLASH��д
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����ʹ��FLASHģ��EEPROM,ʵ���ֽڵı��
 * ʵ��˵����NV32F100xϵ��оƬ������Ӳ��EEPROM��
 *					��128K FLASH�ռ��⣬������5K��NVR�ռ�:0x0040_0000-0x0040_13FF
 *					������ǰ128Kһ�£�������������ִ����Ƭ����ʱ���ᱻ�������ɹ��û�������Ϊ���ݴ洢����
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
    
    /* ������63������*/
    Flash_EraseSector(63*FLASH_SECTOR_SIZE);
    
    for(i=0;i<512;i++)
    {
        u8DataBuff[i] = (uint8_t)i;
    }
/****************��128K�ڵ�FLASH���ж�д����****************/
		
    /*�����������д������*/
    Flash_Program(63*FLASH_SECTOR_SIZE,&u8DataBuff[0],512);
   
   for(int  i=0;i<512/16;i++ )
   {
       for(int ch =0;ch<16;ch++)
       {
           printf("0x%x,",*((uint8_t *)(i*16+ch+63*FLASH_SECTOR_SIZE)));
       }
       printf("\n");
   }

		
/****************Flashģ��EEPROM����****************/
/*		
 *
 *ģ��EEPROM���׵�ַ��0x00401000(NVR�ռ�)
 *ģ��EEPROM�Ĵ�С  ��1K
 *ģ��EEPROM��һ��ʹ��ǰ�������Ƚ��в��������������������д��Ч��
 *
**********************************************************************************/
 
    
  EEPROM_Erase(0x00);
  EEPROM_Erase(0x200);   //�������ʼ����erase����

//����Ϊ��λд���ռ��СΪ1K������EEPROM_Write����
  for(i=0;i<256;i++)
  {
		EEPROM_Write(4*i,1024-4*i);
  }
    //�Ը÷�Χ�ڵ������ַ��д����
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
    
//����д1-2������
  printf("/***************************��������**************************/ \n");
  
//��ַƫ����Ϊ340��дһ������������
	EERPOM_Writeup4byte(340,u8DataBuff,512);
 
  /*for(i=0;i<256;i++)
  {
  printf("adr  %d =0x%x \n",4*i,EEPROM_Read(4*i));
  }  
  */ 
  
  EEPROM_Erase(0x00);
  EEPROM_Erase(0x200);  

//byte�ֽ�д,byte�ֽڶ�ȡ

  printf("/***************************byteд����**************************/ \n"); 
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

