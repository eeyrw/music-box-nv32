/*****************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       pmc.c
 * @brief      ��Դ����ģ�飨PMC)������
 * @author     Navota
 * @date       2018-3-1
 *****************************************************************************/

#include "common.h"
#include "pmc.h"

/*****************************************************************************//*!
*
* @brief  PMCģ���ʼ������
* 
* @param[in]   pPMC_Config       PMC ���ýṹ��.
* @param[in]   pPMC              PMC
*
* @return none.
*
*****************************************************************************/
void PMC_Init(PMC_Type *pPMC, PMC_ConfigType *pPMC_Config)
{
    pPMC->SPMSC1 = pPMC_Config->sCtrlstatus.byte;
    pPMC->SPMSC2 = pPMC_Config->sDetectVoltSelect.byte; 
    if(pPMC_Config->sCtrlstatus.bits.bLvwIrqEn)
      NVIC_EnableIRQ(LVD_LVW_IRQn);			
}


/*****************************************************************************//*!
*
* @brief  ��λPMCģ��.
* 
* @param[in]  pPMC             PMC
*
* @return none.
*
*****************************************************************************/
void PMC_DeInit(PMC_Type *pPMC)
{
    pPMC->SPMSC1 = 0x1C;
    pPMC->SPMSC2 = 0;         
}


/*****************************************************************************//*!
*
* @brief  MCU����ģʽѡ����
* 
* @param[in]  u8PmcMode         ѡ��MCU����ģʽ.
* @param[in]  pPMC              PMC
*
* @return none.
*
*****************************************************************************/
void PMC_SetMode(PMC_Type *pPMC,uint8_t u8PmcMode)
{
    switch(u8PmcMode & 0x3) 
    {
        case PmcModeRun:    
            break;
        case PmcModeWait:   //�ȴ�ģʽ
            wait();
            break;
        case PmcModeStop4:
            /* ֹͣģʽ�£�ʹ�ܵ�ѹ���*/
            pPMC->SPMSC1 |= (PMC_SPMSC1_LVDE_MASK | PMC_SPMSC1_LVDSE_MASK);	
            stop();   
            break;
        case PmcModeStop3: 
            /* ��ֹͣģʽ�£����õ�ѹ���*/
            pPMC->SPMSC1 &= ~(PMC_SPMSC1_LVDE_MASK | PMC_SPMSC1_LVDRE_MASK | PMC_SPMSC1_LVDSE_MASK);	
            stop();  
            break;
        default: 
            break;
    }
 
}

