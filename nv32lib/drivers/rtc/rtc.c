/************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       rtc.c
 * @brief      rtc��ʱ��������
 * @author     Navota
 * @date       2018-3-1
 ***************************************************************************/

#include "common.h"
#include "rtc.h"

/*!
 * @brief ����жϻص����
 *
 */
RTC_CallbackType RTC_Callback[1] = {(RTC_CallbackType)NULL};  



void RTC_Isr(void);


/******************************************************************************
* ����RTC�Ľӿں���
*******************************************************************************/

/*****************************************************************************//*!
*
* @brief ��ʼ��RTCģ��
*        
* @param[in]   pConfig   ����RTCģ��Ľṹ��  
*
* @return none 
*
*****************************************************************************/
void RTC_Init(RTC_ConfigType *pConfig)
{
    uint16_t    u16Clocksource, u16Prescler;
    uint16_t    u16ModVal;
    
    u16Clocksource =0;  
    u16Prescler    =0;
    u16ModVal      =0;

    SIM->SCGC     |= SIM_SCGC_RTC_MASK; 
    
    u16ModVal      = pConfig->u16ModuloValue;
    RTC_SetModulo(u16ModVal); 
    
    if (pConfig->bRTCOut)
    { 
        RTC->SC= RTC_SC_RTCO_MASK;       
    } 
    
    if (pConfig->bInterruptEn)
    {
        NVIC_EnableIRQ(RTC_IRQn);
        RTC_EnableInt(); 
    }
    else      
    {   
        NVIC_DisableIRQ(RTC_IRQn);
    }
    
    if (pConfig->bFlag)
    {   
        RTC_ClrFlags();         
    }

    u16Clocksource = pConfig->bClockSource; 
    u16Prescler    = pConfig->bClockPresaler;
     
    RTC_SetClock(u16Clocksource,u16Prescler );
}

/*****************************************************************************//*!
*
* @brief ����RTCģ����жϻص�����
*        
* @param[in]   pfnCallback   �ص������ĵ�ַ
*
* @return none 
*
*****************************************************************************/
void RTC_SetCallback(RTC_CallbackType pfnCallback)
{
  RTC_Callback[0] = pfnCallback;
}


/*****************************************************************************//*!
*
* @brief ��λRTCģ��
*        
* @param none  
*
* @return none 
*
*****************************************************************************/
void RTC_DeInit(void)
{
    NVIC_DisableIRQ(RTC_IRQn);    
    RTC->MOD = 0;
    while(RTC->MOD);
    
    if(RTC_GetFlags())
    {
        RTC_ClrFlags();
    }
    
    RTC->SC = 0;
    while(RTC->SC);
    SIM->SCGC &= ~SIM_SCGC_RTC_MASK;
}


/*****************************************************************************//*!
*
* @brief RTC�жϷ�����
*        
* @param none  
*
* @return none 
*
*****************************************************************************/
void RTC_Isr(void) 
{
    RTC_ClrFlags();  //����жϱ�־λ       
    if (RTC_Callback[0])
    {    
        RTC_Callback[0]();     
    }
}


