/******************************************************************************
*
* @brief  SPI ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef SPI_H_
#define SPI_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/******************************************************************************
* ����SPIģ�����
******************************************************************************/
#define     MAX_SPI_NO              2

/******************************************************************************
* 
* ����SPI�Ĵ渴λֵ
*
*******************************************************************************/

#define SPI_C1_DEFAULT          0x04						/*!< SPI ���ƼĴ���1 */
#define SPI_C2_DEFAULT          0x00						/*!< SPI ���ƼĴ���2 */
#define SPI_BR_DEFAULT          0x00						/*!< SPI �����ʼĴ��� */
#define SPI_S_DEFAULT           0x20						/*!< SPI ״̬�Ĵ��� */
#define SPI_M_DEFAULT           0x00						/*!< SPI ƥ��Ĵ��� */


/******************************************************************************
* 
* ����SPI����״̬
*
*******************************************************************************/

#define     SPI_ERR_SUCCESS            0                                /*!< �ɹ� */
#define     SPI_ERR_CODE_BASE         ((uint32)SPI0 - 0x40000000L)      /*!< SPI����ַ���� */
#define     SPI_ERR_TXBUF_NOT_EMPTY   (SPI_ERR_CODE_BASE+1)             /*!< ʧ�����ڷ��ͻ�������ʱ����־λû����λ*/
#define     SPI_ERR_RXBUF_NOT_FULL    (SPI_ERR_CODE_BASE+2)             /*!< ʧ�����ڽ��ջ�������ʱ����־λû����λ  */
     
/******************************************************************************
* �������ͱ�������
******************************************************************************/

typedef uint8_t   SPI_WidthType;  /* SPI �������ݿ������ */
typedef uint32_t  ResultType;     /* SPI �շ�����״̬�������� */

/******************************************************************************
* 
* SPI�ص���������
*
*******************************************************************************/
typedef void (*SPI_CallbackType)(void);  

/******************************************************************************
*
* SPI �ṹ������
* 
*******************************************************************************/
typedef struct
{
    uint32_t bIntEn                               : 1; /*!< 1: ʹ���ж�, 0: �����ж�*/
    uint32_t bModuleEn                            : 1; /*!< 1: ʹ��SPIģ��, 0: ����SPIģ�� */
    uint32_t bTxIntEn                             : 1; /*!< 1: ʹ�ܷ����ж�, 0: ���÷����ж� */
    uint32_t bMasterMode                          : 1; /*!< 1: ����ģʽ, 0: �ӻ�ģʽ */
    uint32_t bClkPolarityLow                      : 1; /*!< ����ʱ�Ӽ��� */
    uint32_t bClkPhase1                           : 1; /*!< ����ʱ����λ */
    uint32_t bMasterAutoDriveSS                   : 1; /*!< �ӻ�ѡ�����ʹ�� */
    uint32_t bShiftLSBFirst                       : 1; /*!< 1: LSB, 0: MSB */
    uint32_t bMatchIntEn                          : 1; /*!< 1: ʹ��ƥ���ж�, 0: ����ƥ���ж� */
    uint32_t bModeFaultEn                         : 1; /*!< ʹ������ģʽ������ */
    uint32_t bBidirectionModeEn                   : 1; /*!< ˫��ģʽ������� */
    uint32_t bPinAsOuput                          : 1; /*!< SPI���ſ��� */
    uint32_t bStopInWaitMode                      : 1; /*!< ����SPIʱ���ڵȴ�ģʽ�µĹ���״̬ */
    uint32_t bRsvd                                : 19;
} SPI_SettingType;


/******************************************************************************
*
* SPI���ýṹ������
* 
*******************************************************************************/

typedef struct 
{
    SPI_SettingType     sSettings;              /*!< SPI �ṹ�� */
    uint32_t              u32BitRate;           /*!< ���ò����� */
    uint32_t              u32BusClkHz;          /*!< ��������ʱ�� */
} SPI_ConfigType;                              /*!< SPI���ýṹ�� */


/*****************************************************************************//*!
   *
   * @brief SPI�������ݴ�������λ��ʼ(LSB)
   *        
   * @param[in]  pSPI  ָ��SPIģ��  
   *
   * @return  none.
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_SetLSBFirst(SPI_Type *pSPI)
{
	pSPI->C1 |= SPI_C1_LSBFE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief SPI�������ݴ�������λ��ʼ(MSB)
   *        
   * @param[in]  pSPI  ָ��SPIģ��  
   *
   * @return  none.
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_SetMSBFirst(SPI_Type *pSPI)
{
	pSPI->C1 &= ~SPI_C1_LSBFE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPIʱ�Ӽ���.
   *        
   * @param[in]  pSPI  ָ��SPIģ��  .  
   * @param[in]  u8PolLow   ����ʱ�Ӽ���.  1-SCKʱ����Ч״̬Ϊ�ߣ���Ч״̬Ϊ�ͣ�
   *                                       0-SCKʱ����Ч״̬Ϊ�ͣ���Ч״̬Ϊ�ߣ�
   * @return  none
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_SetClockPol(SPI_Type *pSPI,uint8_t u8PolLow)
{
	if( u8PolLow )
	{
		pSPI->C1 |= SPI_C1_CPOL_MASK;	
	}
	else
	{
		pSPI->C1 &= ~SPI_C1_CPOL_MASK;	
	}
}
/*****************************************************************************//*!
   *
   * @brief ����SPIʱ����λ.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  
   * @param[in]  u8Phase  ����ʱ����λ, 1 - ������SCK�½���������������ز���. 
   *                                    0 - ������SCK�����ز��������½������
   * @return  none
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_SetClockPhase(SPI_Type *pSPI,uint8_t u8Phase)
{
	if( u8Phase )
	{
		pSPI->C1 |= SPI_C1_CPHA_MASK;	
	}
	else
	{
		pSPI->C1 &= ~SPI_C1_CPHA_MASK;	
	}
}
/*****************************************************************************//*!
   *
   * @brief ʹ��SPIģ��.
   *        
   * @param[in]  pSPI  ָ��SPIģ��    
   *
   * @return  none
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_Enable(SPI_Type *pSPI)
{
	pSPI->C1 |= SPI_C1_SPE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPIģ��
   *        
   * @param[in]  pSPI   ָ��SPIģ��  
   *  
   * @return  none
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_Disable(SPI_Type *pSPI)
{
	pSPI->C1 &= ~SPI_C1_SPE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ��SPI�ж�.
   *        
   * @param[in]  pSPI  ָ��SPIģ��  
   *
   * @return  none
   *
   *****************************************************************************/

 __STATIC_INLINE void SPI_IntEnable(SPI_Type *pSPI)
{
	pSPI->C1 |= SPI_C1_SPIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief  ����SPI�ж�.
   *        
   * @param[in]  pSPI   ָ��SPIģ��    
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_IntDisable(SPI_Type *pSPI)
{
	pSPI->C1 &= ~SPI_C1_SPIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPIΪ����ģʽ.
   *        
   * @param[in]  pSPI   ָ��SPIģ��    
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_SetMasterMode(SPI_Type *pSPI)
{
	pSPI->C1 |= SPI_C1_MSTR_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPIΪ�ӻ�ģʽ.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_SetSlaveMode(SPI_Type *pSPI)
{
	pSPI->C1 &= ~SPI_C1_MSTR_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ��SPI�����ж�.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_TxIntEnable(SPI_Type *pSPI)
{
	pSPI->C1 |= SPI_C1_SPTIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPI�����ж�.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
 __STATIC_INLINE void SPI_TxIntDisable(SPI_Type *pSPI)
{
	pSPI->C1 &= ~SPI_C1_SPTIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ�ܴӻ�ѡ�����.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_SSOutputEnable(SPI_Type *pSPI )
{
	pSPI->C1 |= SPI_C1_SSOE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ���ôӻ�ѡ�����.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_SSOutputDisable(SPI_Type *pSPI )
{
	pSPI->C1 &= ~SPI_C1_SSOE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ��SPIƥ���ж�.
   *        
   * @param[in]  pSPI  ָ��SPIģ��  .  
   * 
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_MatchIntEnable(SPI_Type *pSPI )
{
	pSPI->C2 |= SPI_C2_SPMIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����SPIƥ���ж�.
   *        
   * @param[in]  pSPI  ָ��SPIģ��    
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_MatchIntDisable(SPI_Type *pSPI )
{
	pSPI->C2 &= ~SPI_C2_SPMIE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ��������ģʽ������
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_ModfDisable(SPI_Type *pSPI )
{
	pSPI->C2 &= ~SPI_C2_MODFEN_MASK;
}
/*****************************************************************************//*!

   *
   * @brief ʹ������ģʽģʽ������.
   *        
   * @param[in]  pSPI   ָ��SPIģ��   
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_ModfEnable(SPI_Type *pSPI )
{
	pSPI->C2 |= SPI_C2_MODFEN_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ��˫��ģʽ���.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_BidirOutEnable(SPI_Type *pSPI )
{
	pSPI->C2 |= SPI_C2_BIDIROE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ����˫��ģʽ���
   *        
   * @param[in]  pSPI  ָ��SPIģ��    
   *
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_BidirOutDisable(SPI_Type *pSPI )
{
	pSPI->C2 &= ~SPI_C2_BIDIROE_MASK;
}
/*****************************************************************************//*!
   *
   * @brief SPIʱ���ڵȴ�ģʽ��һֱ����
   *        
   * @param[in]  pSPI  ָ��SPIģ��  .  
   * 
   * @return  none.
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_ClockStopDisable(SPI_Type *pSPI )
{
	pSPI->C2 &= ~SPI_C2_SPISWAI_MASK;
}
/*****************************************************************************//*!
   *
   * @brief SPIʱ���ڵȴ�ģʽ�¹ر�
   *        
   * @param[in]  pSPI   ָ��SPIģ��    
   *  
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_ClockStopEnable(SPI_Type *pSPI )
{
	pSPI->C2 |= SPI_C2_SPISWAI_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ʹ��˫����������.
   *        
   * @param[in]  pSPI   ָ��SPIģ��    
   *   
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_BidirPinEnable(SPI_Type *pSPI)
{
	pSPI->C2 |= SPI_C2_SPC0_MASK;
}
/*****************************************************************************//*!
   *
   * @brief SPIʹ�ö����������������ݵ��������.
   *        
   * @param[in]  pSPI  ָ��SPIģ��    
   *   
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_BidirPinDisable(SPI_Type *pSPI)
{
	pSPI->C2 &= ~SPI_C2_SPC0_MASK;
}
/*****************************************************************************//*!
   *
   * @brief ��ȡ�������ݻ���������׼λ.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *    
   * @return  TRUE or FALSE.
   *
   *****************************************************************************/
 __STATIC_INLINE uint8_t SPI_IsSPRF(SPI_Type *pSPI )
{
	return(pSPI->S & SPI_S_SPRF_MASK);
}
/*****************************************************************************//*!
   *
   * @brief ��ȡSPIƥ���־λ
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *    
   * @return  TRUE or FALSE.
   *
   *****************************************************************************/
 __STATIC_INLINE uint8_t SPI_IsSPMF(SPI_Type *pSPI )
{
	return(pSPI->S & SPI_S_SPMF_MASK);
}
/*****************************************************************************//*!
   *
   * @brief ��ȡSPI�������ݻ������ձ�־λ.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  .  
   *    
   * @return  TRUE or FALSE.
   *
   *****************************************************************************/
 __STATIC_INLINE uint8_t SPI_IsSPTEF(SPI_Type *pSPI )
{
	return(pSPI->S & SPI_S_SPTEF_MASK);
}
/*****************************************************************************//*!
   *
   * @brief ��ȡ����ģʽ�����־λ
   *        
   * @param[in]  pSPI ָ��SPIģ��    
   *    
   * @return  TRUE or FALSE.
   *
   *****************************************************************************/
 __STATIC_INLINE uint8_t SPI_IsMODF(SPI_Type *pSPI )
{
	return(pSPI->S & SPI_S_MODF_MASK);
}
/*****************************************************************************//*!
   *
   * @brief ��ȡSPI���ݼĴ���.
   *        
   * @param[in]  pSPI   ָ��SPIģ��    
   *    
   * @return ���ݼĴ�����ֵ
   *
   *****************************************************************************/
 __STATIC_INLINE uint8_t SPI_ReadDataReg(SPI_Type *pSPI )
{
	return pSPI->D;
}
/*****************************************************************************//*!
   *
   * @brief  д���ݵ�SPI���ݼĴ���.
   *        
   * @param[in]  pSPI  ָ��SPIģ��   
   * @param[in]  u8WrBuff   д��SPI���ݼĴ��������ݻ����� 
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_WriteDataReg(SPI_Type *pSPI, uint8_t u8WrBuff )
{
	pSPI->D = u8WrBuff;
}
/*****************************************************************************//*!
   *
   * @brief д���ݵ�SPIƥ��Ĵ���.
   *        
   * @param[in]  pSPI   ָ��SPIģ��  
   * @param[in]  u8WrBuff   д��SPIƥ��Ĵ��������ݻ����� 
   *
   * @return  none
   *
   *****************************************************************************/
 __STATIC_INLINE void SPI_WriteMatchValue(SPI_Type *pSPI, uint8_t u8WrBuff )
{
	pSPI->M = u8WrBuff;
}
/******************************************************************************/

void SPI_Enable(SPI_Type *pSPI);
void SPI_Disable(SPI_Type *pSPI);
void SPI_SetLSBFirst(SPI_Type *pSPI);
void SPI_SetMSBFirst(SPI_Type *pSPI);
void SPI_IntEnable(SPI_Type *pSPI);
void SPI_IntDisable(SPI_Type *pSPI);
void SPI_SetMasterMode(SPI_Type *pSPI);
void SPI_SetSlaveMode(SPI_Type *pSPI);
void SPI_TxIntEnable(SPI_Type *pSPI);
void SPI_TxIntDisable(SPI_Type *pSPI);
void SPI_SSOutputEnable(SPI_Type *pSPI );
void SPI_SSOutputDisable(SPI_Type *pSPI );
void SPI_MatchIntEnable(SPI_Type *pSPI );
void SPI_MatchIntDisable(SPI_Type *pSPI );
void SPI_ModfDisable(SPI_Type *pSPI );
void SPI_ModfEnable(SPI_Type *pSPI );
void SPI_BidirOutEnable(SPI_Type *pSPI );
void SPI_BidirOutDisable(SPI_Type *pSPI );
void SPI_ClockStopDisable(SPI_Type *pSPI );
void SPI_ClockStopEnable(SPI_Type *pSPI );
void SPI_BidirPinEnable(SPI_Type *pSPI );
void SPI_BidirPinDisable(SPI_Type *pSPI );
void SPI_SetClockPol(SPI_Type *pSPI,uint8_t u8PolLow);
void SPI_SetClockPhase(SPI_Type *pSPI,uint8_t u8Phase);
void SPI_SetBaudRate(SPI_Type *pSPI,uint32_t u32BusClock,uint32_t u32Bps );
uint8_t SPI_IsSPRF(SPI_Type *pSPI );
uint8_t SPI_IsSPMF(SPI_Type *pSPI );
uint8_t SPI_IsSPTEF(SPI_Type *pSPI );
uint8_t SPI_IsMODF(SPI_Type *pSPI );
uint8_t SPI_ReadDataReg(SPI_Type *pSPI );
void SPI_WriteDataReg(SPI_Type *pSPI, uint8_t u8WrBuff );
void SPI_WriteMatchValue(SPI_Type *pSPI, uint8_t u8WrBuff );
void SPI_Init(SPI_Type *pSPI, SPI_ConfigType *pConfig);
void SPI_DeInit(SPI_Type *pSPI);
ResultType SPI_TransferWait(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType *pWrBuff,uint32 uiLength);
void SPI_SetCallback(SPI_Type *pSPI,SPI_CallbackType pfnCallback);


#ifdef __cplusplus
}
#endif
#endif 
