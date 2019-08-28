/**************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       crc.c
 * @brief      ����У��ģ�飨CRC)������
 * @author     Navota
 * @date       2018-3-1
 ****************************************************************************/
#include "common.h"
#include "crc.h"

/*****************************************************************************//*!
*
* @brief ��ʼ��CRC
*        
* @param[in]  pConfig ָ��CRC���ýṹ��. 
*
* @return none
*
*****************************************************************************/
void CRC_Init(CRC_ConfigType *pConfig)
{       
    uint32_t     u32Sc ;
    
    u32Sc      = 0;
    
    SIM->SCGC |= SIM_SCGC_CRC_MASK;   //ʹ��CRC����ģ��ʱ��
    
    u32Sc     |= ((pConfig->bWidth & 0x01)<<24);   //CRCģʽѡ��
    u32Sc     |= CRC_CTRL_TOTR(pConfig->bTransposeReadType & 0x03);  //�������ݶ�ȡ��ת������
    u32Sc     |= CRC_CTRL_TOT(pConfig->bTransposeWriteType & 0x03);  //��������д���ת������
    
    if (pConfig->bFinalXOR)  
    {
        u32Sc |= CRC_CTRL_FXOR_MASK;  //��CRC���ݼĴ�Ķ�ȡֵ����������     
    }
    
    CRC0->CTRL  = u32Sc;

    if ( pConfig->bWidth )      
    {
        CRC0->GPOLY = pConfig->u32PolyData;  //д��32λ����ʽ
    }
    else 
    {
        CRC0->GPOLY_ACCESS16BIT.GPOLYL = pConfig->u32PolyData;  /*!< ������д16λ����ʽ*/             
    }  
    
}     


/*****************************************************************************//*!
*
* @brief 16λģʽ��CRC���㺯��.
*          
* @param[in]  seed  ����ֵ
* @param[in]  msg   ָ����������
* @param[in]  sizeBytes  ���ݴ�С
*
* @return data_out convertion result
*
*****************************************************************************/
uint32_t CRC_Cal16(uint32_t seed, uint8_t *msg, uint32_t sizeBytes)
{
  uint32_t ctrl_reg,data_out,data_in;
  uint8_t  *pCRCBytes;
  uint32_t sizeWords;
  uint32_t i,j;  
  
  /* ����WaS=1��д������ֵ */
  ctrl_reg  = CRC0->CTRL;
  CRC0->CTRL  = ctrl_reg | CRC_CTRL_WAS_MASK;
  CRC0->ACCESS16BIT.DATAL = seed; 
  
  /* Set WaS=0��׼��д������*/
  CRC0->CTRL  = ctrl_reg & 0xFD000000; 

  /*�ȴ��������*/
  sizeWords = sizeBytes>>1;
  j = 0;
  for(i=0;i<sizeWords;i++){ 
      data_in = (msg[j] << 8) | (msg[j+1]);  /*������д��CRC���ݼĴ�������16λ��ʽд��*/
      j += 2;
      CRC0->ACCESS16BIT.DATAL =data_in; 
  }
  if (j<sizeBytes)
  { 
     pCRCBytes = (uint8_t*)&CRC0->ACCESS8BIT.DATALL; /*��8λ�ķ�ʽд��ʣ�������*/
     *pCRCBytes++ = msg[j];
  }
  if ((CRC0->CTRL& CRC_CTRL_TOTR_MASK)>1)   //Modify
  {
     data_out=CRC0->ACCESS16BIT.DATAH;  //�����ֽ�ת�ú�16λ������
  }
  else
  {
     data_out=CRC0->ACCESS16BIT.DATAL;  //����16λ������
  }
  
  return(data_out);
}


/*****************************************************************************//*!
*
* @brief  32λģʽ��CRC����.
*        
* @param[in]  seed 
* @param[in]  msg  ָ����������
* @param[in]  sizeBytes  ���ݴ�С
*
* @return data_out convertion result 
*
*****************************************************************************/
uint32_t CRC_Cal32(uint32_t seed, uint8_t *msg, uint32_t sizeBytes)
{  
  uint32_t ctrl_reg,data_out,data_in;
  uint32_t sizeDwords;
  uint8_t  *pCRCBytes;
  uint32_t i,j;
  
 /* ����WaS=1��д������ֵ */
  ctrl_reg = CRC0->CTRL;
  CRC0->CTRL = ctrl_reg | 0x02000000;
  CRC0->DATA = seed;
 
 /* Set WaS=0��׼��д������*/
  CRC0->CTRL = ctrl_reg & 0xFD000000;
  
  /*�ȴ����ݼ������*/
  sizeDwords = sizeBytes>>2;
  j = 0;
  for(i=0;i<sizeDwords;i++)
  { 
      data_in = ((msg[j] << 24) | (msg[j+1] << 16) | (msg[j+2] << 8) | msg[j+3]); /*������д��CRC���ݼĴ�������32��ʽд��*/
      j += 4;
      CRC0->DATA = data_in; 
  }
  if (j<sizeBytes)
  {
    pCRCBytes = (uint8_t*)&CRC0->ACCESS8BIT.DATALL;    /*��8λ�ķ�ʽд��ʣ�������*/

#if  defined(BYTE_ENABLES_1_2_4_8)    
    
    /*ֻд�����ֽ�*/  
    for(;j<sizeBytes;j++)
    {     
       *pCRCBytes++ = msg[j];
    }
#elif  defined(BYTE_ENABLES_3_6_C)
    
    /*д�������ֽ�*/ 
    data_in = 0;
    i = 0;
    for(;j<sizeBytes;j++)
    {     
      data_in = (data_in <<8) | msg[j];
      i++;
      if (i==2)
      {
        i = 0;
        CRC0->ACCESS16BIT.DATAL = data_in;
      }
    }
    if (i==1)
    {
       CRC0->ACCESS8BIT.DATALL = data_in;                /*!< д�����һ���ֽ� */
    }
#elif  defined(BYTE_ENABLES_7_E)                         
    /*!< д�������ֽ�*/
    data_in = 0;
    i = 0;
    for(;j<sizeBytes;j++)
    {     
      data_in = (data_in <<8) | msg[j];
      i++;
      if (i==3)
      {
        i = 0;
        /*д���һ���ַ�*/
        CRC0->ACCESS8BIT.DATAHL  = (data_in>>16) & 0xff; /*!< д����ֵĵ��ֽ� */
        /*д����������ֽ�*/
        CRC0->ACCESS16BIT.DATAL = data_in & 0x00ffff;    /*!< д����� */
       }
    }
    if ( i == 2)
    {
       CRC0->ACCESS16BIT.DATAL = (data_in);              /*!< д��������ֽ� */
    }
    else if (i == 1)
    {
       CRC0->ACCESS8BIT.DATALL = data_in;                /*!< д���һ���ֽ� */      
    }
#else                                                    /*!< ֻд���ֽ� */
    for(;j<sizeBytes;j++)
    {     
       *pCRCBytes = msg[j];
    }
#endif            
  }
  data_out=CRC0->DATA;    

  return(data_out);   //����32λ������
}


/*****************************************************************************//*!
*
* @brief ��λCRCģ��
*        
* @param none 
*
* @return none 
*
*****************************************************************************/
void CRC_DeInit(void)
{  
   CRC0->CTRL  = 0x3000000; /*!<����CRCλ32λģʽ*/ 
   CRC0->DATA  = 0xFFFFFFFF;/*!< д32λ����ֵ��CRC���ݼĴ���*/ 
   while(!(CRC0->DATA == 0xFFFFFFFF));
   CRC0->GPOLY = 0x00001021; 
   CRC0->CTRL  = 0;         /*!<CRC���ƼĴ�������*/  
   SIM->SCGC &= ~SIM_SCGC_CRC_MASK;
}



