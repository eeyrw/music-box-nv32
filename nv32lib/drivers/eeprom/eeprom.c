/*****************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       eeprom.c
 * @brief      flashģ��eeprom�⺯��
 * @author     Navota
 * @date       2018-1-1
 ***************************************************************************/
#include "flash.h"
#include "eeprom.h"
#include <string.h>

/******************************************************************************
*
* EEPROM �����������eeprom
*�����������ַ�������������adr���ڵ�������512�ֽڣ�
*
******************************************************************************/
uint16_t Adress_Js(uint32_t adr)
{
  uint16_t err = EEPROM_ERR_SUCCESS;

  if (adr & 0x03)
  {
    err = EEPROM_ERR_INVALID_PARAM;
    return (err);
  }

  if (adr > 1024)
  {
    err = EEPROM_ADR_OverFlow;
    return (err);
  }
  return (err);
}

/******************************************************************************
*
* EEPROM �����������eeprom
*�����������ַ�������������adr���ڵ�512bytes eeprom
*
******************************************************************************/
uint16_t EEPROM_Erase(uint32_t adr)
{
  uint16_t err = EEPROM_ERR_SUCCESS;
  uint32_t e_adr;

  if (adr & 0x03)
  {
    err = EEPROM_ERR_INVALID_PARAM;
    return (err);
  }

  if (adr > 1024)
  {
    err = EEPROM_ADR_OverFlow;

    return (err);
  }

  e_adr = adr + EEPROM_START_ADR;
  err = Flash_EraseSector(e_adr);
  return (err);
}

/******************************************************************************
*
* EEPROM ��ȡ��������ȡ��ַ���ڵ�eeprom
*�����������ַ
*
******************************************************************************/
uint32_t EEPROM_Read(uint32_t adr)
{
  uint16_t err = EEPROM_ERR_SUCCESS;
  uint32_t e_adr;
  uint32_t data;

  if (adr & 0x03)
  {
    err = EEPROM_ERR_INVALID_PARAM;
    return (err);
  }

  if (adr > 1024)
  {
    err = EEPROM_ADR_OverFlow;
    return (err);
  }

  e_adr = adr + EEPROM_START_ADR;
  data = M32(e_adr);
  return (data);
}
/******************************************************************************
*
* EEPROM д������д��ַ���ڵ�eeprom
* д֮ǰ��ȡ�������ж�eeprom�Ƿ�Ϊ�գ����Ϊ�գ���ֱ��д
* ����ǿգ����Ȱ�����512bytes sector��ȡ��sram���޸�Ҫд��λ��
* Ȼ����д�뵽flash��ģ��һ��eeprom��д����
* �����������ַ
*
******************************************************************************/
uint16_t EEPROM_Write(uint32_t adr, uint32_t Data)
{

  uint32_t err = EEPROM_ERR_SUCCESS;
  uint32_t e_adr;
  uint32_t r_data;
  uint16_t i;
  uint32_t start_adr;
  //   uint32_t modify_adr;
  uint32_t EEPROM_DATA[128];

  if (adr & 0x03)
  {
    err = EEPROM_ERR_INVALID_PARAM;
    return (err);
  }

  if (adr > 1024)
  {
    err = EEPROM_ADR_OverFlow;
    return (err);
  }

  r_data = EEPROM_Read(adr);

  e_adr = adr + EEPROM_START_ADR;

  if (r_data == EEPROM_BLANK) //���Ҫд��λ���ǿյģ���ֱ��д
  {
    err = Flash_Program1LongWord(e_adr, Data);
  }
  else if ((r_data & Data) == Data) //���Ҫд��λ�ö�Ӧ��bit����Ҫд������һ�£�������1��Ҳ�ǿ���ֱ��д
  {
    err = Flash_Program1LongWord(e_adr, Data);
  }
  else if (r_data == Data) //���Ҫд�����ݺ����е�����һ�£��Ͳ������κβ�����ֱ�ӷ���
  {
    return (err);
  }
  else
  {
    start_adr = e_adr & EEPROM_SECTOR_MASK; //�����sector��ͷ��ַ

    for (i = 0; i < 128; i++) //���Ҫд��λ�ò�Ϊ�գ����Ȱ�flash���ݶ�ȡ����������sram�У��޸�
    {
      EEPROM_DATA[i] = M32(start_adr + 4 * i);
    }

    EEPROM_DATA[(adr & EEPROM_ARRAY_ADR_MASK) >> 2] = Data; //�޸�SRAM �е�����

    err = EEPROM_Erase(adr);

    err = Flash_Program(start_adr, (uint8_t *)EEPROM_DATA, 512); //Ȼ��д��flash
  }
  return (err);
}

/******************************************************************************
*
*Byte д����
*
******************************************************************************/
uint16_t EEPROM_WriteByte(uint32_t adr, uint8_t Data)
{
  uint32_t err = EEPROM_ERR_SUCCESS;
  uint32_t data_mask;
  uint32_t r_data;
  uint32_t data_m0;
  uint32_t data_m1;
  uint32_t word_adr = adr & 0x3fc;
  uint32_t b_sit = adr & 0x3;

  //���ø�λΪFF
  data_m0 = Data << b_sit * 8;
  data_mask = 0xFFFFFFFF << (b_sit + 1) * 8;
  //Ȼ���õ�λΪFF
  data_m1 = 0xFFFFFFFF >> (32 - b_sit * 8);
  data_m1 = data_m1 | data_m0 | data_mask;

  r_data = EEPROM_Read(word_adr);
  r_data |= 0xFF << b_sit * 8;

  data_m1 = data_m1 & r_data;

  err = EEPROM_Write(word_adr, data_m1);

  return (err);
}

/******************************************************************************
*
*Byte ������
*
******************************************************************************/
uint8_t EEPROM_ReadByte(uint32_t adr)
{
  uint32_t r_data;
  uint32_t word_adr = adr & 0x3fc;
  uint32_t b_sit = adr & 0x3;
  uint8_t data;

  r_data = EEPROM_Read(word_adr);
  data = (r_data >> b_sit * 8) & 0xff;
  return (data);
}

/******************************************************************************
*
*д������дһ������Ϊbytesize����eeprom
*�Ȱ�1k��eeprom��ȡ����sram��Ȼ���޸�Ҫд��λ�ã�
*��������ǻ��������Ż���
******************************************************************************/
uint16_t EERPOM_Writeup4byte(uint32_t adr, uint8_t *pData, uint32_t length)
{
  uint8_t buf[512];
  uint8_t *pbuf;
  uint32_t e_adr;
  uint32_t e_sec;
  uint32_t e_offset;
  uint32_t a;
  uint32_t err = EEPROM_ERR_SUCCESS;

#ifdef IAR
  if (adr & 0x03)
  {
    err = EEPROM_ERR_INVALID_PARAM;
    return (err);
  }
#endif

  if ((adr + length) > 1024)
  {
    err = EEPROM_ADR_OverFlow;
    return (err);
  }

  e_adr = adr + EEPROM_START_ADR;
  e_sec = e_adr & EEPROM_SECTOR_MASK;
  e_offset = e_adr & 0x1ff;

  while (length > 0)
  {
    //�����ʼ��ַ������0�����߳���С��512 ���������ѭ��
    if (e_offset || (length < 512))
    {
      pbuf = buf;
      a = 512 - e_offset;
      a = (length > a ? a : length);

      memcpy(buf, (uint8_t *)e_sec, 512);
      memcpy(&buf[e_offset], pData, a);
      pData += a;
      length -= a;
      e_offset = 0;
    }
    else
    { //�����ʼ��ַ����0�ҳ��ȴ���512
      pbuf = pData;
      pData += 512;
      length -= 512;
    }
    err = Flash_EraseSector(e_sec);
    err = Flash_Program(e_sec, (uint8_t *)pbuf, 512); //Ȼ��д��flash
    e_sec += 0x200;
  }
  return err;
}
