/************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       pit.c
 * @brief      pit��ʱ��������
 * @author     Navota
 * @date       2018-3-1
 ************************************************************************/

#include "common.h"
#include "pit.h"
#include "SEGGER_SYSVIEW.h"

/*!
 * @brief ��Żص����
 *
 */
PIT_CallbackType PIT_Callback[2] = {(PIT_CallbackType)NULL}; 

void PIT_Ch0Isr(void);
void PIT_Ch1Isr(void);

/******************************************************************************
* ����PIT�Ľӿں���
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief ��ʼ��PITģ��.
*        
* @param[in]   u8Channel_No   ͨ����
* @param[in]   pConfig        ����ģ��Ľṹ��ָ��
*
* @return none
*
*****************************************************************************/
void PIT_Init(uint8_t u8Channel_No, PIT_ConfigType *pConfig)
{  
    SIM->SCGC |= SIM_SCGC_PIT_MASK;     /*!< ѡͨPITģ���ſ�ʱ�� */
    
    if (pConfig->bFreeze)            
    {                                               
        PIT_SetDebugFreeze();            
    } 
    
    if (pConfig->bModuleDis == 0) 
    {                                              
        PIT_Enable();       					/*!< ��׼PIT��ʱ����ʱ��ʹ�� */                                                                                
    }  
    
    PIT_SetLoadVal(u8Channel_No, pConfig->u32LoadValue);  //���ض�Ӧͨ���Ķ�ʱ����ʼֵ
    
    if (pConfig->bInterruptEn)            
    {                                   
        if (u8Channel_No)
        {        
             NVIC_EnableIRQ(PIT_CH1_IRQn);  //������Ӧͨ��IRQ�ж�         
        }
        else
        {                  
            NVIC_EnableIRQ(PIT_CH0_IRQn);
        }
        PIT_ChannelEnableInt(u8Channel_No); //������Ӧͨ�����ж�����
    }                   
    else                                     
    {                     
        NVIC_DisableIRQ(PIT_CH0_IRQn); //��ֹͨ��0��IRQ�ж�          
    }

    if (pConfig->bChainMode)            
    {                                               
        PIT_ChannelEnableChain(u8Channel_No); //��ʱ�����ӵ�ǰһ��ʱ��            
    }
    
    if (pConfig->bETMerEn)            
    {                                               
        PIT_ChannelEnable(u8Channel_No); //��ʱ��ͨ��ʹ��               
    }             

}


/*****************************************************************************//*!
*
* @brief װ�ض�ʱ����ʼֵ������ֵ�Ĵ�����.
*        
* @param[in]   u8Channel_No   ͨ���� 
* @param[in]   u32loadvalue   ���м��ص���ֵ
*
* @return none
*
*****************************************************************************/
void PIT_SetLoadVal(uint8_t u8Channel, uint32_t u32loadvalue)

{
    PIT->CHANNEL[u8Channel].LDVAL = u32loadvalue;   
}


/*****************************************************************************//*!
*
* @brief ����PITģ��ص�����.
*        
* @param[in]   u8Channel_No   ͨ����.   
* @param[in]   pfnCallback    ָ��ص�����.   
*
* @return none
*
*****************************************************************************/
void PIT_SetCallback(uint8_t u8Channel_No, PIT_CallbackType pfnCallback)
{
  PIT_Callback[u8Channel_No] = pfnCallback;
}


/*****************************************************************************//*!
*
* @brief ��λPITģ��
*        
* @param none  
*
* @return none 
*
*****************************************************************************/
void PIT_DeInit(void)
{
    NVIC_DisableIRQ(PIT_CH0_IRQn);
    NVIC_DisableIRQ(PIT_CH1_IRQn);    
    PIT_SetLoadVal(0,0);
    PIT_SetLoadVal(1,0);
    PIT_ChannelDisable(0);
    PIT_ChannelDisable(1);
    PIT_ChannelDisableInt(0);
    PIT_ChannelDisableInt(1);
    PIT_ChannelDisableChain(0);
    PIT_ChannelDisableChain(1);    
    PIT_ChannelClrFlags(0);
    PIT_ChannelClrFlags(1);    
    PIT_SetDebugOn();
    PIT_Disable();
    SIM->SCGC &= ~SIM_SCGC_PIT_MASK;
}


/*****************************************************************************//*!
*
* @brief PIT0ͨ���жϷ�����.
*        
* @param none
*
* @return none 
*
*****************************************************************************/
void PIT_Ch0Isr(void) 
{
	//SEGGER_SYSVIEW_RecordEnterISR();
   PIT_ChannelClrFlags(0); //����жϱ�־λ
    
    if (PIT_Callback[0])
    {    
        PIT_Callback[0]();     
    }
    //SEGGER_SYSVIEW_RecordExitISR();
}
    
/*****************************************************************************//*!
*
* @brief PIT0ͨ���жϷ�����.
*        
* @param none
*
* @return none 
*
*****************************************************************************/
void PIT_Ch1Isr(void) 
{   
    PIT_ChannelClrFlags(1); //����жϱ�־λ
    if (PIT_Callback[1])
    {    
        PIT_Callback[1]();     
    }
}    


