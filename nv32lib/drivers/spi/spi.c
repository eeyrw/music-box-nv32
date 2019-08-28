/**************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       spi.c
 * @brief      ��������ӿ�ģ��(SPI)������
 * @author     Navota
 * @date       2017-1-1
 ****************************************************************************/
#include "common.h"
#include "spi.h"

/****************************************************************************!
 * @ ���SPI�ص������ӿ�
 ****************************************************************************/

SPI_CallbackType SPI_Callback[MAX_SPI_NO] = {(SPI_CallbackType)NULL};

/*****************************************************************************//*!
   *
   * @brief ��ʼ��SPIģ��
   *        
   * @param[in]  pSPI     ָ��SPIģ��.
   * @param[in]  pConfig  ָ��SPI���ò���.
   *
   * @return none.
   *
   *****************************************************************************/
void SPI_Init(SPI_Type *pSPI, SPI_ConfigType *pConfig)
{
#if defined(CPU_NV32M3)  
     /* sanity check */
    ASSERT((pSPI == SPI0));
    SIM->SCGC |= SIM_SCGC_SPI0_MASK;
#else
    /* sanity check */
    ASSERT((pSPI == SPI0) ||  (pSPI == SPI1));
    
    /*ʹ��SPIģ������ʱ��*/
    if( pSPI == SPI0)
    {
        SIM->SCGC |= SIM_SCGC_SPI0_MASK;
    }
    else
    {
        SIM->SCGC |= SIM_SCGC_SPI1_MASK;     
    }
#endif
    /*����SPI���ƼĴ��� */
    if( pConfig->sSettings.bIntEn)  //ʹ��SPI�ж�
    {
        SPI_IntEnable(pSPI);
#if defined(CPU_NV32M3)  
        NVIC_EnableIRQ(SPI0_IRQn);  
#else
        if( pSPI == SPI0 )
        {
			NVIC_EnableIRQ(SPI0_IRQn);
        }
        else
        {
			NVIC_EnableIRQ(SPI1_IRQn);
        }
#endif
    }

    if( pConfig->sSettings.bTxIntEn)  //ʹ��SPI�����ж�
    {
        SPI_TxIntEnable(pSPI);
#if defined(CPU_NV32M3)  
        NVIC_EnableIRQ(SPI0_IRQn);  
#else
        if( pSPI == SPI0 )
        {
			NVIC_EnableIRQ(SPI0_IRQn);
        }
        else
        {
			NVIC_EnableIRQ(SPI1_IRQn);
        }
#endif
    }
    if( pConfig->sSettings.bMasterMode)  //��/�ӻ�ѡ��
    {
        SPI_SetMasterMode(pSPI); //SPI����Ϊ����ģʽ
    }
    else
    {
		SPI_SetSlaveMode(pSPI); //SPI����Ϊ�ӻ�ģʽ
    }

    if( pConfig->sSettings.bClkPolarityLow) //ʱ�Ӽ�������
    {
    	SPI_SetClockPol(pSPI,1);
    }
    if( pConfig->sSettings.bClkPhase1) //ʱ����λ����
    {
        SPI_SetClockPhase(pSPI,1);
    }
    if( pConfig->sSettings.bShiftLSBFirst) //����SPI�������ݴ��俪ʼλ
    {
        SPI_SetLSBFirst(pSPI);  // SPI�������ݴ�������λ��ʼ(LSB)
    }
    if( pConfig->sSettings.bMatchIntEn)
    {
        SPI_MatchIntEnable(pSPI); //ʹ��SPIƥ���ж�
    }
    if( pConfig->sSettings.bModeFaultEn)
    {
        SPI_ModfEnable(pSPI);  //ʹ������ģʽģʽ������.
    }
    if( pConfig->sSettings.bMasterAutoDriveSS)
    {
			  /*����SSOE��MODFENλ���ӻ�ģʽ��SS����Ϊ�ӻ�������Զ������ӻ�SS����ʹ��*/
        SPI_SSOutputEnable(pSPI);
        SPI_ModfEnable(pSPI);
    }    
 
    if( pConfig->sSettings.bPinAsOuput)
    {
        SPI_BidirPinEnable(pSPI); //ʹ��˫����������.
    }

    if( pConfig->sSettings.bBidirectionModeEn)
    {
        SPI_BidirOutEnable(pSPI); //ʹ��˫��ģʽ���.
    }
    if( pConfig->sSettings.bStopInWaitMode)
    {
        SPI_ClockStopEnable(pSPI); //SPIʱ���ڵȴ�ģʽ�¹ر�
    }
    
    if(pConfig->sSettings.bMasterMode) //���ò�����
    {
        SPI_SetBaudRate(pSPI,pConfig->u32BusClkHz,pConfig->u32BitRate); 
    }

	/*ʹ��SPIģ��*/
    if( pConfig->sSettings.bModuleEn)
    {
        SPI_Enable(pSPI);
    }
}

/*****************************************************************************//*!
   *
   * @brief ����SPI������.
   *        
   * @param[in]  pSPI   ָ��SPIģ��
   * @param[in]  u32BusClock   ����ʱ��.
   * @param[in]  u32Bps   SPI������.
   *
   * @return  none.
   *
   *****************************************************************************/
void SPI_SetBaudRate(SPI_Type *pSPI,uint32_t u32BusClock,uint32_t u32Bps)
{
	uint32_t u32BitRateDivisor;
	uint8_t u8Sppr;
	uint8_t u8Spr;
    uint8_t u8ReadFlag;
	u32BitRateDivisor = u32BusClock/u32Bps; /* ���㲨�������� */
       
    u8ReadFlag = 0;
    /* �������ʺϵ� SPPR �� SPR */
    for (u8Spr = 0; u8Spr <= 8; u8Spr++)
    {
        for(u8Sppr = 0; u8Sppr <= 7; u8Sppr++)
        {
            if((u32BitRateDivisor>>(u8Spr+1))<=(u8Sppr+1))
            {
                u8ReadFlag = 1;
                break;
            }
        }
        if(u8ReadFlag)
        {
            break;
        }
    }
    if(u8Sppr >=8)
    {
        u8Sppr = 7;
    }
    if(u8Spr >8)
    {
        u8Spr = 8;
    }
    /* ���ò����� */
    pSPI->BR = SPI_BR_SPPR(u8Sppr) | SPI_BR_SPR(u8Spr);
}

/*****************************************************************************//*!
   *
   * @brief ��/д���ݼĴ���.
   *        
   * @param[in]   pSPI   ָ��SPIģ��.
   * @param[in]   pWrBuff -- ���ͣ�д������ָ��
   * @param[in]   uiLength -- ��/д���ݳ���.
   * @param[out]   pRdBuff --  ���գ���������ָ��. 
   * 
   * @return  if <0, means error, 0: success.
   *
   *****************************************************************************/
ResultType SPI_TransferWait(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType *pWrBuff,uint32 uiLength)
{
    ResultType err = SPI_ERR_SUCCESS;
    uint32_t  i;
    
    if(!uiLength)
    {
        return (err);
    }
    for(i = 0; i < uiLength; i++)
    {
        while(!SPI_IsSPTEF(pSPI));
        SPI_WriteDataReg(pSPI,pWrBuff[i]);
        while(!SPI_IsSPRF(pSPI));
        pRdBuff[i] = SPI_ReadDataReg(pSPI);
    }
    return (err);        
}



/*****************************************************************************//*!
   *
   * @brief ��λSPIģ��
   *        
   * @param[in]   pSPI ָ��SPIģ��
   *         
   * @return none.
   *
   *****************************************************************************/
void SPI_DeInit(SPI_Type *pSPI)
{  
    int16 i;
    pSPI->C1 = SPI_C1_DEFAULT;
    pSPI->C2 = SPI_C2_DEFAULT;
    pSPI->BR = SPI_BR_DEFAULT;
    pSPI->M  = SPI_M_DEFAULT;
    for(i = 0; i<100; i++);                 /*��ʱһ��ʱ�䣬�ȴ�SPI�˳��ж� */
}

/*****************************************************************************//*!
   *
   * @brief ����SPI�ص����������жϷ������е���
   *        
   * @param[in]   pSPI  ָ��SPIģ��
   * @param[in]  pfnCallback  �ص�����. 
   *  
   * @return none.
   *
*****************************************************************************/
void SPI_SetCallback(SPI_Type *pSPI,SPI_CallbackType pfnCallback)
{
    uint32_t    u32Port = ((uint32_t)pSPI-(uint32_t)SPI0)>>12;
    ASSERT(u32Port <2);
    SPI_Callback[u32Port] = pfnCallback;
}

/*****************************************************************************//*!
   *
   * @brief SPI0 �жϷ�����.
   *        
   * @param   none.
   * @return  none.
   *
   *****************************************************************************/

void SPI0_Isr(void)
{
    if( SPI_Callback[0] )
    {
		SPI_Callback[0]();
    }
}
#ifndef CPU_NV32M3
/*****************************************************************************//*!
   *
   * @brief SPI1 �жϷ�����
   *        
   * @param   none.
   * @return  none.
   *
   *****************************************************************************/

void SPI1_Isr(void)
{
    if( SPI_Callback[1] )
    {
		SPI_Callback[1]();
    }
}
#endif


