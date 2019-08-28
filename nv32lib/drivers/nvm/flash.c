/*!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       flash.c
 * @brief      flash������
 * @author     Navota
 * @date       2018-6-19
 */


#include "flash.h"

#define FLASH_ENABLE_STALLING_FLASH_CONTROLLER


/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: Flash_Init
*
* @brief ��ʼ��FLASH
*        
* @param  
*
* @return none
*
*****************************************************************************/

uint16_t Flash_Init(void)
{
	uint16_t err   = FLASH_ERR_SUCCESS;
	uint32_t clkDIV = BUS_CLK_HZ/1000000L - 1;
	uint32_t Tpgs  =(285 *(BUS_CLK_HZ/100))/1000000L;  //update 2016.8.4 by ��Ű��GG
	uint32_t Tprog =(675*(BUS_CLK_HZ/100))/1000000L;   //by ��Ű��GG
 
        
	EFMCR=(clkDIV<<24) + 0x00000001; //divide to 1M hz 
	EFMETM0=(Tpgs<<16)  + 0x00001194;  //0x00281194; //
	EFMETM1=(Tprog<<16) + 0x000088B8; //

  return(err);
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: FlashProgram
*
* @brief    flash���ر�̣��ܵĹ��̾��ǣ������ֽڳ��ȣ���д�����������ֵĸ�����
*                ��д��ʣ������һ���ֵĸ��������д��ʣ����ֽ�����
*        
* @param[in]     wNVMTargetAddress     ��Ҫ��ŵ�FLASH�׵�ַ
* @param[in]     *pData                ��Ҫ��ŵ�����
* @param[in]     sizeBytes             �ֽڳ���
*
* @return none
*
*****************************************************************************/

__ramfunc uint16_t Flash_Program(uint32_t wNVMTargetAddress, uint8_t *pData, uint16_t sizeBytes)
{
	uint16_t err = FLASH_ERR_SUCCESS;
	uint16_t w2LongWordCount = sizeBytes>>3;//����һ�£��õ�2���ֵĸ���
	uint8_t  wLeftBytes = (sizeBytes & 0x07);//��λ�����ֽڵĸ���
	uint16_t wLeftLongWords = wLeftBytes>>2;//��λ����һ���ָ���
	uint32_t wTargetAddress = wNVMTargetAddress;
	uint32_t dwData0,dwData1;
	uint32_t *pdwData = (uint32_t*)pData;//����
	int  i;
        //�ж��Ƿ��ֶ���
	if(wNVMTargetAddress & 0x03)
	{
		err = FLASH_ERR_INVALID_PARAM;
		return (err);//������Ч�Ĳ���
	}
	//ѭ��д���������֣���8���ֽڣ�����д��w2LongWordCount * 8���ֽ�
	for(i = 0; i < w2LongWordCount; i++)
	{
		dwData0 = *pdwData++;
		dwData1 = *pdwData++;
		err = Flash_Program2LongWords(wTargetAddress, dwData0, dwData1);//���������ֵı��
		if(err) //��ַ��Ϊ4���ֽڶ��룬��ֱ����ת
		{
			goto EndP;
			//break;
		}
		wTargetAddress += 8;//ѭ��һ��д��8���ֽڣ���2���֣�NV32��flash���ֶ����
	}
	// һ���ֵı�̣���4bytes
	for(i = 0; i < wLeftLongWords; i++)
	{
		dwData0 = *pdwData++;
		err = Flash_Program1LongWord(wTargetAddress, dwData0);
		if(err)
		{			
			goto EndP;
			//break;
		}
		wTargetAddress += 4;
	}
	wLeftBytes = (wLeftBytes-(wLeftLongWords<<2));	//�����ֺ�һ�ֵı�̶��������ʣ��ĵ���λ�ֽ���
	if(!wLeftBytes){ //����ʣ���ֽ��������سɹ�
		return (err);
	}
        
#if     defined(BIG_ENDIAN)                
	dwData0 = 0;
	pData = (uint8_t*)pdwData;	
	for(i = wLeftBytes; i >0; i--)
	{
		dwData0 <<= 8;
		dwData0 |= *pData++;	
	}
	
	wLeftBytes = 4 - wLeftBytes;	 
	for(i = wLeftBytes; i >0; i--)
	{
		dwData0 <<= 8;
		dwData0 |= 0xFF;	
	}
#else
	dwData0 = 0xFFFFFFFFL;        
	pData = (uint8_t*)pdwData+wLeftBytes-1;	
	for(i = wLeftBytes; i >0; i--)
	{
		dwData0 <<= 8;
		dwData0 |= *pData--;	
	}
#endif	
	err = Flash_Program1LongWord(wTargetAddress, dwData0);	
EndP:	
	return (err);
}


/*****************************************************************************//*!

* @function name: FlashProgram1LongWord
*
* @brief  ����һ���ֵĴ�С����̵�FLASH��(Ҳ�����ĸ��ֽ�)
*        
* @param[in]     wNVMTargetAddress     ��Ҫ��ŵ�FLASH�׵�ַ
* @param[in]     dwData                ��Ҫ��ŵ�����
*
* @return none  
*
*****************************************************************************/
__ramfunc uint16_t Flash_Program1LongWord(uint32_t wNVMTargetAddress, uint32_t dwData)
{
	uint16_t err = FLASH_ERR_SUCCESS;
	
	//�ж��Ƿ�Ϊ�ֶ���
	if(wNVMTargetAddress & 0x03)
	{
		err = FLASH_ERR_INVALID_PARAM;
		return (err);
	}
        // ��������־
	EFMCMD = FLASH_CMD_CLEAR;
	//д�����ݵ���Ӧ�ĵ�ַ��
	DisableInterrupts;
	M32(wNVMTargetAddress) = dwData;
	//���ر������	
    EFM_LaunchCMD(FLASH_CMD_PROGRAM);
	EnableInterrupts;
	return (err);//����״̬
}

/*****************************************************************************//*!

* @function name: FlashProgram2LongWords
*
* @brief  ���������ֵĴ�С����̵�FLASH��(Ҳ���ǰ˸��ֽ�)
*        
* @param[in]     wNVMTargetAddress     ��Ҫ��ŵ�FLASH�׵�ַ
* @param[in]     dwData0               ��4���ֽ�
* @param[in]     dwData1               ��4���ֽ�
*
* @return none  
*
*****************************************************************************/
__ramfunc uint16_t Flash_Program2LongWords(uint32_t wNVMTargetAddress, uint32_t dwData0, uint32_t dwData1)
{
	uint16_t err = FLASH_ERR_SUCCESS;

	
	//�ж��Ƿ�Ϊ�ֶ���
	if(wNVMTargetAddress & 0x03)
	{
		err = FLASH_ERR_INVALID_PARAM;
		return (err);
	}
	// ��������־

	EFMCMD = FLASH_CMD_CLEAR;
    DisableInterrupts;
	M32(wNVMTargetAddress) = dwData0;//������ݵ���Ŀ���ַΪ��ʼ��4���ֽڵĿռ���
    EFM_LaunchCMD(FLASH_CMD_PROGRAM);//0x20000000,���ر������
	EnableInterrupts;
	wNVMTargetAddress = wNVMTargetAddress +4;//��ַ���ֶ���ģ���ַ�����һ����
	
	EFMCMD = FLASH_CMD_CLEAR;
	DisableInterrupts;
	M32(wNVMTargetAddress) = dwData1;//�ڶ������ݷ��봦���ĵ�ַ��4���ֽڵĿռ���
    EFM_LaunchCMD(FLASH_CMD_PROGRAM);//���ر������
    EnableInterrupts;
	return (err);//���ش���״̬
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: Flash_EraseSector
*
* @brief ����Ŀ���ַ��һ��������512�ֽڣ�,.
*        
* @param[in]    wNVMTargetAddress   �����������׵�ַ
*
* @return none
*
*****************************************************************************/
__ramfunc uint16_t  Flash_EraseSector(uint32_t wNVMTargetAddress)
{
	uint16_t err = FLASH_ERR_SUCCESS;
	// �ж��Ƿ��ֶ���
	if(wNVMTargetAddress & 0x03)
	{
		err = FLASH_ERR_INVALID_PARAM;
		return (err);
	}
	// ��������־
	  EFMCMD = FLASH_CMD_CLEAR;
	  DisableInterrupts;
	  M32(wNVMTargetAddress) = 0xffffffff;
	  EFM_LaunchCMD(FLASH_CMD_ERASE_SECTOR);//���ز�������
	  EnableInterrupts;
	  return (err);
}


__ramfunc uint16_t Flash_VerifyBackdoorKey()
{
	uint16_t err = FLASH_ERR_SUCCESS;
        
	// Clear error flags
	 EFMCMD = FLASH_CMD_CLEAR;
	// Write index to specify the command code to be loaded
	 Custombkd = FLASH_FACTORY_KEY;
	return (err);
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: NVM_EraseAll
*
* @brief ��Ƭ����FLASH(���ã�ע��0X40E�����ַ���ڵ��Խ׶�д��0XFE)
*        
* @param  
*	
* @return none
*
*****************************************************************************/
__ramfunc uint16_t NVM_EraseAll(void)
{
	uint16_t err = FLASH_ERR_SUCCESS;
	  EFMCMD = FLASH_CMD_CLEAR;
	  EFM_LaunchCMD(FLASH_CMD_ERASE_ALL);
	// Clear error flags
    return err;
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: NVM_Unsecure
*
* @brief unsecure 
*        
* @param  
*	
* @return none
*
*****************************************************************************/
__ramfunc uint16_t NVM_Unsecure(void)
{
	uint16_t err = FLASH_ERR_SUCCESS;

    return err;
}



/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: EFM_LaunchCMD
*
* @brief ������غ���(ע���˺��������SRAM������)
*        
* @param  
*	
* @return none
*
*****************************************************************************/
#ifdef IAR
void __ramfunc EFM_LaunchCMD(uint32_t EFM_CMD)
#else
__ramfunc void EFM_LaunchCMD(uint32_t EFM_CMD)
#endif
{
	DisableInterrupts;
		if((EFMCMD&EFM_DONE_MASK)== EFM_STATUS_READY)
		{
	  EFMCMD = EFM_CMD;
		}
	while(1)
	{
		if((EFMCMD&EFM_DONE_MASK) == EFM_STATUS_DONE) break;
	}
    EnableInterrupts;
}
