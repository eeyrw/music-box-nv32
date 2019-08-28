/*****************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       etm.c
 * @brief      etm��ʱ��������
 * @author     Navota
 * @date       2018-3-1
 ***************************************************************************/

#include "common.h"
#include "etm.h"


/*!
 * @brief ��Żص����
 *
 */
ETM_CallbackPtr ETM_Callback[3] = {(ETM_CallbackPtr)NULL};


/******************************************************************************
* ����ETM�Ľӿں���
*******************************************************************************/

/*******************************************************************************//*!
*
* @brief ����ETMģ��ʱ����Դ����Ƶϵ��.
*        
* @param[in]    pETM                  ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    ClockSource           ETM ѡ���ʱ��Դ �� ���ã�ETM_CLOCK_NOCLOCK����ϵͳʱ�ӣ�ETM_CLOCK_SYSTEMCLOCK�����̶�Ƶ��ʱ��/2��ETM_CLOCK_FIXEDFREQCLOCK�������ʱ�ӣ�ETM_CLOCK_EXTERNALCLOCK��.
* @param[in]    ClockPrescale         ��Ƶϵ��.
*
* @return none.
*
*********************************************************************************/
void ETM_ClockSet(ETM_Type *pETM, uint8_t u8ClockSource, uint8_t u8ClockPrescale)
{
    uint8_t   u8Temp;
    u8Temp  = (pETM->SC & 0xE0);//pETMָ���SC�Ĵ�����5λ��0����δѡ��ʱ�ӣ�ʱ�������ȡ1��Ƶ
    u8Temp |= (ETM_SC_CLKS(u8ClockSource & 0x3) | ETM_SC_PS(u8ClockPrescale & 0x7));
    pETM->SC = u8Temp;//���ø�ETM��״̬����ƼĴ���ETMx_SC
}

/*********************************************************************************//*!
*
* @brief  ETM��PWM�ĳ�ʼ������ 
*        
* @param[in]    pETM                  ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    PWMModeSelect         ���ж���CPWM��10�������ض���EPWM��01���Լ�����ģʽPWM��11��.
* @param[in]    PWMEdgeSelect         �������壨01�����������壨10��.
*
* @return none.
*
*********************************************************************************/
void ETM_PWMInit(ETM_Type *pETM, uint8_t u8PWMModeSelect, uint8_t u8PWMEdgeSelect)
{
    uint8_t   channels, i;
    
    ASSERT((ETM0== pETM) || (ETM1== pETM) || (ETM2== pETM));//���������ETMͨ���Ƿ���ȷ
    
    /* ѡ��ETMʱ�� */
	if (ETM0 == pETM)
    {
        channels = 2;
        SIM->SCGC |= SIM_SCGC_ETM0_MASK; //ETM0��������ͨ��
    }
    else if(ETM1 == pETM) 
    {
        channels = 2;
        SIM->SCGC |= SIM_SCGC_ETM1_MASK;  //ETM1��������ͨ��
    }        
    else
    {
        channels = 6;
        SIM->SCGC  |= SIM_SCGC_ETM2_MASK; //ETM2��������ͨ��
    }
    
    pETM->SC  = 0x0;                                   //�رռ�����
		pETM->MOD = ETM_MOD_INIT; 
    
    if(ETM_PWMMODE_CENTERALLIGNED == u8PWMModeSelect)   //��CPWM
    {
        pETM->SC |= ETM_SC_CPWMS_MASK; 
    }
    else if(ETM_PWMMODE_COMBINE == u8PWMModeSelect)    
    {
        ASSERT(ETM2 == pETM);
        pETM->MODE    |= ETM_MODE_WPDIS_MASK | ETM_MODE_ETMEN_MASK;
        pETM->COMBINE = ETM_COMBINE_COMBINE0_MASK | ETM_COMBINE_COMP0_MASK | ETM_COMBINE_SYNCEN0_MASK | ETM_COMBINE_DTEN0_MASK |
                        ETM_COMBINE_COMBINE1_MASK | ETM_COMBINE_COMP1_MASK | ETM_COMBINE_SYNCEN1_MASK | ETM_COMBINE_DTEN1_MASK |
                        ETM_COMBINE_COMBINE2_MASK | ETM_COMBINE_COMP2_MASK | ETM_COMBINE_SYNCEN2_MASK | ETM_COMBINE_DTEN2_MASK 
                        ;    // ��ͨ������ģʽ 
        pETM->SC &= ~ETM_SC_CPWMS_MASK; 
    }
    if(ETM_PWM_HIGHTRUEPULSE == u8PWMEdgeSelect)
    {
      /* ����ͨ���Ĵ���������ͨ��״̬��ͨ������ֵ */
        for(i=0; i<channels; i++)
        {
            pETM->CONTROLS[i].CnSC = ETM_CnSC_MSB_MASK | ETM_CnSC_ELSB_MASK;  
            pETM->CONTROLS[i].CnV  = ETM_C0V_INIT + i*100; 
        }
    }
    else if(ETM_PWM_LOWTRUEPULSE == u8PWMEdgeSelect)
    {
        for(i=0; i<channels; i++) 
        {
            pETM->CONTROLS[i].CnSC = ETM_CnSC_MSB_MASK | ETM_CnSC_ELSA_MASK; 
            pETM->CONTROLS[i].CnV  = ETM_C0V_INIT + i*100 ; 
        }
    }  
}

/*********************************************************************************//*!
*
* @brief  ����ETM��ͨ�����ܣ�����GPIO���������� 
*        
* @param[in]    pETM                  ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    u8ETM_Channel         ͨ����.
*
* @return none.
*
*********************************************************************************/

void ETM_disblechannel(ETM_Type *pETM, uint8_t u8ETM_Channel)
{    
     uint8_t   i;
     i= u8ETM_Channel;
     pETM->CONTROLS[i].CnSC &= ~ (ETM_CnSC_ELSB_MASK| ETM_CnSC_ELSA_MASK); 
}

/*********************************************************************************//*!
*
* @brief ���벶׽��ʼ������.
*        
* @param[in]    pETM                  ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    Channel               ����ͨ����.
* @param[in]    CaptureMode           ѡ��׽��ʽ:������, �½��ػ�������.
*
* @return none.
*
*********************************************************************************/
void ETM_InputCaptureInit(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8CaptureMode)
{ 
    ASSERT(((ETM0 == pETM) && (u8ETM_Channel < 2))   || 
           ((ETM1 == pETM) && (u8ETM_Channel < 2))   || 
           ((ETM2 == pETM) && (u8ETM_Channel < 6))
           );

    
      /* ѡ��ETMʱ�� */
		if ((ETM0 == pETM) && (u8ETM_Channel < 2))
    {
        SIM->SCGC |= SIM_SCGC_ETM0_MASK;
        NVIC_EnableIRQ(ETM0_IRQn);
    }
    else if((ETM1 == pETM)  && (u8ETM_Channel < 2))
    {
        SIM->SCGC |= SIM_SCGC_ETM1_MASK;
        NVIC_EnableIRQ(ETM1_IRQn);
    }        
    else 
    {
        SIM->SCGC |= SIM_SCGC_ETM2_MASK;
        NVIC_EnableIRQ(ETM2_IRQn);
    }
    
    pETM->SC  = 0x0;     //�رռ����� 
    pETM->MOD = 0xFFFF;  
    
    if(ETM_INPUTCAPTURE_RISINGEDGE == u8CaptureMode)        //�����жϣ�����������
    {
        pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSA_MASK;
    }
    else if(ETM_INPUTCAPTURE_FALLINGEDGE == u8CaptureMode)  //�����½���
    {
        pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSB_MASK;
    }
    else if(ETM_INPUTCAPTURE_BOTHEDGE == u8CaptureMode)     //����������
    {
        pETM->CONTROLS[u8ETM_Channel].CnSC = ETM_CnSC_CHIE_MASK | ETM_CnSC_ELSA_MASK | ETM_CnSC_ELSB_MASK;       
    }
}

/*********************************************************************************//*!
*
* @brief  ��ETM����˫�߲���ģʽ������һ������Ŀ�Ȼ�����(ETM2).
*        
* @param[in]    pETM                  ETM2.
* @param[in]    ChannelPair           Ƶ�������������Ϊ: 0, 2, 4.
* @param[in]    CaptureMode           ѡ�����ڲ�׽��4������������׽��ʽ��5��.
* @param[in]    Channel_N_Edge        Ƶ��N���ؼ�⣺�ޣ�0���������أ�1���½��أ�2��˫�أ�3��. 
* @param[in]    Channel_Np1_Edge      Ƶ��N+1���ؼ��.
*
* @return none.
*
*********************************************************************************/
void ETM_DualEdgeCaptureInit(ETM_Type *pETM, uint8_t u8ChannelPair, uint8_t u8CaptureMode, 
                                 uint8_t u8Channel_N_Edge, uint8_t u8Channel_Np1_Edge)
{ 
    ASSERT((ETM2 == pETM)  && (u8ChannelPair < 6) && !(u8ChannelPair & 1) );
    
    SIM->SCGC |= SIM_SCGC_ETM2_MASK;
    if((0 == u8ChannelPair) || (2== u8ChannelPair))
    {
                                      
    }
       
    pETM->SC    = 0x0;                  /* �رռ����� */ 
    pETM->MOD   = 0xFFFF;
    pETM->MODE |= ETM_MODE_ETMEN_MASK;  /* ETMEN = 1 */  
    
    pETM->COMBINE |=  ((ETM_COMBINE_DECAPEN0_MASK) << (u8ChannelPair * 4)); 
    
    pETM->CONTROLS[u8ChannelPair].CnSC &= ~ETM_CnSC_CHF_MASK;       /* CH(n)F �� CH(n+1)F λ����Ҫ����� */
    pETM->CONTROLS[u8ChannelPair + 1].CnSC &= ~ETM_CnSC_CHF_MASK;
    
    if(ETM_INPUTCAPTURE_DUALEDGE_ONESHOT == u8CaptureMode)          /* ����ģʽ */
    {
        pETM->CONTROLS[u8ChannelPair].CnSC &= ~ETM_CnSC_MSA_MASK;
        pETM->CONTROLS[u8ChannelPair+1].CnSC &= ~ETM_CnSC_MSA_MASK;
    }
    else if(ETM_INPUTCAPTURE_DUALEDGE_CONTINUOUS == u8CaptureMode)  /* ����ģʽ */
    {
        pETM->CONTROLS[u8ChannelPair].CnSC |= ETM_CnSC_MSA_MASK;
        pETM->CONTROLS[u8ChannelPair+1].CnSC |= ETM_CnSC_MSA_MASK;
    }
    
    pETM->CONTROLS[u8ChannelPair].CnSC |= (u8Channel_N_Edge << 2);   /* ѡ������� */
    pETM->CONTROLS[u8ChannelPair + 1].CnSC |= (u8Channel_Np1_Edge << 2);   
    
    pETM->COMBINE |=  (ETM_COMBINE_DECAP0_MASK << (u8ChannelPair * 4)); 
}

/*********************************************************************************//*!
*
* @brief ����Աȳ�ʼ��.
*        
* @param[in]    pETM                  ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    Channel               ����ͨ����ͨ����.
* @param[in]    CompareMode           ѡ��ģʽ����ת��01������λ��11������0��10��.
*
* @return none.
*
*********************************************************************************/
void ETM_OutputCompareInit(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8CompareMode)
{
    ASSERT(((ETM0 == pETM) && (u8ETM_Channel < 2))   || 
           ((ETM1 == pETM) && (u8ETM_Channel < 2))   || 
           ((ETM2 == pETM) && (u8ETM_Channel < 6))
           );
    
    /* ѡ��ETMģ��ʱ�� */
	if(ETM0 == pETM)
    {
        SIM->SCGC |= SIM_SCGC_ETM0_MASK;
    }
    else if(ETM1 == pETM)
    {
        SIM->SCGC |= SIM_SCGC_ETM1_MASK;
    }        
    else
    {
        SIM->SCGC |= SIM_SCGC_ETM2_MASK;
    }
    
    pETM->SC  = 0x0;                                                                    //�رռ�����
    pETM->MOD = ETM_MOD_INIT; 
    pETM->CONTROLS[u8ETM_Channel].CnSC = (ETM_CnSC_MSA_MASK | (u8CompareMode << 2));   
    pETM->CONTROLS[u8ETM_Channel].CnV  = ETM_C0V_INIT;
}

/*********************************************************************************//*!
*
* @brief ʵ�����ͬ��������ETM2��.
*        
* @param[in]    pETM                  ETM2.
*
* @return none.
*
*********************************************************************************/
void ETM_SoftwareSync(ETM_Type *pETM)
{
    ASSERT(ETM2 == pETM);

    pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   
    pETM->SYNC      |= ETM_SYNC_SWSYNC_MASK;
}

/*********************************************************************************//*!
*
* @brief ETM������ETMx_SYNC �Ĵ�����ѡ��Ӳ������(ETM2).
*        
* @param[in]    pETM                  ETM2.
* @param[in]    u8TriggerN            ѡ��Ӳ��������Դ.
*
* @return none.
*
*********************************************************************************/
void ETM_HardwareSync(ETM_Type *pETM, uint8_t u8TriggerN)
{
    ASSERT(ETM2 == pETM);
    
    pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;  
    
    switch(u8TriggerN)
    {
        case ETM_SYNC_TRIGGER_TRIGGER2: 
                pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
                break;  
        case ETM_SYNC_TRIGGER_TRIGGER1: 
                pETM->SYNC |= ETM_SYNC_TRIG1_MASK;
                break;     /* ��Ҫ������ETM0CH0 */
        case ETM_SYNC_TRIGGER_TRIGGER0:
                pETM->SYNC |= ETM_SYNC_TRIG0_MASK; 
                break;     /* ��Ҫ������ACMP0 */
        default: 
                break;
    }
}

/*********************************************************************************//*!
*
* @brief ͨ������ETM��֤Ӳ��ͬ�����������������ж��������(ETM2).
*        
* @param[in]    pETM               ETM2.
* @param[in]    u8TriggerMask      Ӳ��������Դ��־��.����TRIG0~TREG2.(x000xxxx~x111xxxx)
*
* @return none.
*
*********************************************************************************/
void ETM_HardwareSyncCombine(ETM_Type *pETM, uint8_t u8TriggerMask)
{
    ASSERT(ETM2 == pETM);
    
    pETM->SYNCONF   |= ETM_SYNCONF_SYNCMODE_MASK;   
    pETM->SYNC      &= 0x8F;
    pETM->SYNC      |= (u8TriggerMask & 0x70);
}

/*********************************************************************************//*!
*
* @brief Ӳ������2����ETM2��PWMͬ������(ETM2)
*        
* @param[in]    pETM              ETM2.
*
* @return none.
*
*********************************************************************************/
void ETM_GenerateTrig2(ETM_Type *pETM)
{
    ASSERT(ETM2 == pETM);
    
    if(pETM->SYNC & ETM_SYNC_TRIG2_MASK)
    {
#if defined(CPU_NV32)
        SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;
#endif
    }
}


/*********************************************************************************//*!
*
* @brief ETM����ʱ�����ã�ETM2��.
*        
* @param[in]    pETM                  ETM2.
* @param[in]    PrescalerValue        ����ʱ�ӷ�Ƶֵ, 0 to 3.
* @param[in]    DeadETMeValue         ʱ����ֵ����, 0 to 63.
*
* @return none.
*
*********************************************************************************/
void ETM_PWMDeadETMeSet(ETM_Type *pETM, uint8_t u8PrescalerValue, uint8_t u8DeadETMeValue)
{
    ASSERT(ETM2 == pETM);
    
    pETM->COMBINE |= 0x101010;              /* ʹ������ʱ����� */

    if(!(pETM->MODE & ETM_MODE_WPDIS_MASK)) /* �ж��Ƿ�ʹ��д���� */
    {
        pETM->MODE |= ETM_MODE_WPDIS_MASK;  /* ��ֹд���� */
        pETM->DEADETME = (ETM_DEADETME_DTVAL(u8DeadETMeValue & 0x3F) | ETM_DEADETME_DTPS(u8PrescalerValue & 0x3));
        pETM->MODE &= ~ETM_MODE_WPDIS_MASK; /* ʹ��д���� */       
    }
    else 
    {
        /* ����д���� */
        pETM->DEADETME = (ETM_DEADETME_DTVAL(u8DeadETMeValue & 0x3F) | ETM_DEADETME_DTPS(u8PrescalerValue & 0x3));
    }
    pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;     /* �������ͬ�� */
}    

/*********************************************************************************//*!
*
* @brief ����������Σ�ETM2��.
*        
* @param[in]    pETM                  ETM2.
* @param[in]    Channel               ��Ҫ���ε�ͨ����.
*
* @return none.
*
*********************************************************************************/
void ETM_OutputMaskSet(ETM_Type *pETM, uint8_t u8ETM_Channel)
{
    ASSERT((ETM2 == pETM) && (u8ETM_Channel < 6));

    pETM->OUTMASK |= (1 << u8ETM_Channel);
    
    if(pETM->SYNC & ETM_SYNC_SYNCHOM_MASK)              /* ͨ��PWMͬ������ */
    {
        pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK;   
        if(pETM->SYNCONF & ETM_SYNCONF_SWOM_MASK)       /* ����������� */
        {
            pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;
        }
        else if(pETM->SYNCONF & ETM_SYNCONF_HWOM_MASK)  /* Ӳ���������� */
        {
            pETM->SYNC |= ETM_SYNC_TRIG2_MASK;
            
#if defined(CPU_NV32)
            SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;         /* ʹ��Ӳ��ͬ�� */ 
#endif
        }
        else
        {
        }
    }
    else  /* �������ͬ��, ��ϵͳʱ�ӵ������ض������  */
    {
    }
}

/*********************************************************************************//*!
*
* @brief ��������������SWOCTRL�Ĵ�����ͬ���Ƿ����������(ETM2).
*        
* @param[in]    pETM                  ETM2.
* @param[in]    Channel               �������PWM����ͨ��ѡ��.
* @param[in]    ChannelValue          0��1,0������;1����.
*
* @return none.
*
*********************************************************************************/
void ETM_SWOutputControlSet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8ChannelValue)
{
    ASSERT((ETM2 == pETM) && (u8ETM_Channel < 6));
    
    if(ETM_SWOCTRL_HIGH == u8ChannelValue)
    {
        pETM->SWOCTRL |= (0x0101 << u8ETM_Channel);
    }
    else if(ETM_SWOCTRL_LOW == u8ChannelValue)
    {
        pETM->SWOCTRL |= (1 << u8ETM_Channel);
        pETM->SWOCTRL &= ~(0x100 << u8ETM_Channel);
    }
    if(pETM->SYNCONF & ETM_SYNCONF_SWOC_MASK)               
    {
        pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK;        
        if(pETM->SYNCONF & ETM_SYNCONF_SWSOC_MASK)        
        {
            pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;            
        }
        else if(pETM->SYNCONF & ETM_SYNCONF_HWSOC_MASK)     
        {
            pETM->SYNC |= ETM_SYNC_TRIG2_MASK;

#if defined(CPU_NV32)
            SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;          
#endif
        }
    }
    else 
    {
    }
}

/*********************************************************************************//*!
*
* @brief ����ͨ���������(ETM2).
*        
* @param[in]    pETM                  ETM2.
* @param[in]    Channel               PWM����ͨ��ѡ��.
* @param[in]    ActiveValue           ���Ե�ѡ��0Ϊ�ߵ�ƽ��1Ϊ�͵�ƽ.
*
* @return none.
*
*********************************************************************************/
void ETM_PolaritySet(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8ActiveValue)
{
    ASSERT((ETM2 == pETM) && (u8ETM_Channel < 6));
    
    if(ETM_POLARITY_HIGHACTIVE == u8ActiveValue)
    {
        pETM->POL &=  ~(1 << u8ETM_Channel);
    }
    else if(ETM_POLARITY_LOWACTIVE == u8ActiveValue)
    {
        pETM->POL |=  (1 << u8ETM_Channel);
    }
}

/*********************************************************************************//*!
*
* @brief ����ETMģ����debugģʽ�µ���Ϊ(ETM2).
*        
* @param[in]    pETM             ETM2.
* @param[in]    u8DebugMode      debug ��ģʽ��00-11֮��ѡ��.
*
* @return none.
*
*********************************************************************************/
void ETM_SetDebugModeBehavior(ETM_Type *pETM, uint8_t u8DebugMode)
{
    ASSERT((ETM2 == pETM));
    pETM->CONF &= ~ETM_CONF_BDMMODE_MASK;
    pETM->CONF |= ETM_CONF_BDMMODE(u8DebugMode);
}

/*********************************************************************************//*!
*
* @brief ETM��TOFƵ�ʴ�С������(ETM2).
*        
* @param[in]    pETM             ETM2.
* @param[in]    u8TOFNUM         TOFƵ��������С0��31֮��.
*
* @return none.
*
*********************************************************************************/
void ETM_SetTOFFrequency(ETM_Type *pETM, uint8_t u8TOFNUM)
{
    ASSERT((ETM2 == pETM));
    pETM->CONF &= ~ETM_CONF_NUMTOF_MASK;
    pETM->CONF |= ETM_CONF_NUMTOF(u8TOFNUM);
}

/*********************************************************************************//*!
*
* @brief ����ͨ��CH��n����ͨ��CH��n+1����������(ETM2).
*        
* @param[in]    pETM                  ETM2.
* @param[in]    ChannelPair           Ҫ��������ͨ�����ţ���n��Ϊ0,1,2.
*    
* @return none.
*
*********************************************************************************/
void ETM_InvertChannel(ETM_Type *pETM, uint8_t u8ChannelPair)
{
    ASSERT((ETM2 == pETM)  && u8ChannelPair <= 2);

    pETM->INVCTRL |= 1<<u8ChannelPair;
    if(pETM->SYNCONF & ETM_SYNCONF_INVC_MASK)       
    {
        pETM->SYNCONF |= ETM_SYNCONF_SYNCMODE_MASK; 
        if(pETM->SYNCONF & ETM_SYNCONF_SWINVC_MASK)
        {
            pETM->SYNC |= ETM_SYNC_SWSYNC_MASK;    
        }
        else if(pETM->SYNCONF & ETM_SYNCONF_HWINVC_MASK)  
        {
            pETM->SYNC |= ETM_SYNC_TRIG2_MASK;

#if defined(CPU_NV32)
            SIM->SOPT  |= SIM_SOPT_ETMSYNC_MASK;           
  
#endif
        }
    }
    else 
    {
    }
}  

/*****************************************************************************//*!
*
* @brief ETMģ���ʼ������.
*        
* @param[in]    pETM        ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    pConfig     ����ETMģ��Ľṹ��.
*
* @return none.
*
*****************************************************************************/
void ETM_Init(ETM_Type *pETM, ETM_ConfigType *pConfig)
{
    ASSERT((ETM0 == pETM) || (ETM1 == pETM) || (ETM2 == pETM));
    if(ETM0 == pETM)
    {
        SIM->SCGC |= SIM_SCGC_ETM0_MASK;
    }
    else if(ETM1 == pETM)
    {
        SIM->SCGC |= SIM_SCGC_ETM1_MASK;
    }
    else
    {
        SIM->SCGC |= SIM_SCGC_ETM2_MASK;
    }
        
    /*�رռ�����*/
    pETM->SC = 0; 
		pETM->MODE = pConfig->mode;       
		pETM->MOD = pConfig->modulo;
		pETM->CNT = pConfig->cnt;
        
    if( pETM->MODE & ETM_MODE_ETMEN_MASK  ) 
    {
        /* �� ETMEN = 1ʱ, ���мĴ�������д�� */
        pETM->COMBINE   = pConfig->combine;      
        pETM->CNTIN     = pConfig->cntin;      
        pETM->SYNC      = pConfig->sync;      
        pETM->OUTINIT   = pConfig->outinit;      
        pETM->OUTMASK   = pConfig->outmask;      
        pETM->DEADETME  = pConfig->deadETMe;      
        pETM->EXTTRIG   = pConfig->exttrig;      
        pETM->POL       = pConfig->pol;      
        pETM->FMS       = pConfig->fms;      
        pETM->FILTER    = pConfig->filter;      
        pETM->FLTCTRL   = pConfig->fltctrl;    
        pETM->FLTPOL    = pConfig->fltpol;      
        pETM->CONF      = pConfig->conf;      
        pETM->SYNCONF   = pConfig->synconf;      
        pETM->SWOCTRL   = pConfig->swoctrl;      
        pETM->PWMLOAD   = pConfig->pwmload;      
    }
    /* д��״̬���ƼĴ�����ʹ��ʱ��*/
    pETM->SC = pConfig->sc;
}


/*****************************************************************************//*!
*
* @brief  ��λETMģ��.
*        
* @param[in]    pETM          ָ������ETM��ʱ������һ���Ļ�ַ.
*
* @return none.
*
*****************************************************************************/
void ETM_DeInit(ETM_Type *pETM)
{
    ASSERT((ETM0 == pETM) || (ETM1 == pETM) || (ETM2 == pETM));
    pETM->SC = 0;       
		pETM->MOD = 0;
		pETM->CNT = 0;
    if(ETM2 == pETM) 
    {
          pETM->MODE = 0x4; 
          pETM->COMBINE = 0;      
          pETM->CNTIN = 0;      
          pETM->SYNC = 0;      
          pETM->OUTINIT = 0;      
          pETM->OUTMASK = 0;      
          pETM->DEADETME = 0;      
          pETM->EXTTRIG = 0;      
          pETM->POL = 0;      
          pETM->FMS = 0;      
          pETM->FILTER = 0;      
          pETM->FLTCTRL = 0;  
          pETM->FLTPOL = 0;      
          pETM->CONF = 0;      
          pETM->SYNCONF = 0;      
          pETM->SWOCTRL = 0;      
          pETM->PWMLOAD = 0;      
    }
    /* ��ֹETMģ���ʱ�� */
		if (ETM0 == pETM)
    {
        SIM->SCGC &= ~SIM_SCGC_ETM0_MASK;
        NVIC_DisableIRQ(ETM0_IRQn);
    }
    else if(ETM1 == pETM)
    {
        SIM->SCGC &= ~SIM_SCGC_ETM1_MASK;
        NVIC_DisableIRQ(ETM1_IRQn);
    } 
    else if (ETM2 == pETM)
    {
        SIM->SCGC &= ~SIM_SCGC_ETM2_MASK;
        NVIC_DisableIRQ(ETM2_IRQn);
    }
}
    
/*****************************************************************************//*!
*
* @brief ����ETMͨ��, ����ͨ��״̬�����ƼĴ���CnSC��ͨ������ֵ�Ĵ���CnV.
*        
* @param[in]    pETM               ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    ETM_Channel        ETM��ͨ����.
* @param[in]    pTETMCH_Params     ����ETMͨ�������Ľṹ��.
*
* @return none.
*
*****************************************************************************/
void ETM_ChannelInit(ETM_Type *pETM, uint8_t u8ETM_Channel, ETM_ChParamsType *pTETMCH_Params)
{
    ASSERT((ETM0 == pETM) || (ETM1 == pETM) || (ETM2 == pETM)); //���Լ��ͨ������ȷ��
    
		if (ETM0 == pETM)
    {
        ASSERT(u8ETM_Channel < 2);
        SIM->SCGC |= SIM_SCGC_ETM0_MASK;
    }
    else if(ETM1 == pETM)
    {
        ASSERT(u8ETM_Channel < 2);
        SIM->SCGC |= SIM_SCGC_ETM1_MASK;
    }        
    else
    {
        ASSERT(u8ETM_Channel < 6);
        SIM->SCGC |= SIM_SCGC_ETM2_MASK;
    }
    
    pETM->CONTROLS[u8ETM_Channel].CnSC = pTETMCH_Params->u8CnSC;
    pETM->CONTROLS[u8ETM_Channel].CnV = pTETMCH_Params->u16CnV; 
    
    return;
}

/*****************************************************************************//*!
*
* @brief ���ü���ģʽ��ռ�ձ�(ETM2).
*
* @param[in]    pETM            ETM2.
* @param[in]    ETM_Channel     ��ͨ������1��3��5.
* @param[in]    dutyCycle       ����ռ�ձȣ���DutyCycleΪ10,��ôռ�ձȾ�Ϊ10%.
*
* @return none.
*
*****************************************************************************/
void  ETM_SetDutyCycleCombine(ETM_Type *pETM, uint8_t u8ETM_Channel, uint8_t u8DutyCycle)
{
    uint16_t   cnv = pETM->CONTROLS[u8ETM_Channel-1].CnV;
    uint16_t   modulo = pETM->MOD; 
    
    ASSERT((1 == u8ETM_Channel) || (3 == u8ETM_Channel) || (5 == u8ETM_Channel));
    
    cnv += (u8DutyCycle * (modulo+1))  / 100;
    if(cnv > modulo)
    {
        cnv = modulo - 1;
    }
    pETM->CONTROLS[u8ETM_Channel].CnV = cnv ;    
    
    pETM->PWMLOAD |= ETM_PWMLOAD_LDOK_MASK | (1<<u8ETM_Channel);     
}

/*****************************************************************************//*!
*
* @brief ���üĴ��� ETMx_SYNCONF����λ��λ,��������������������Ŀ����Ƿ���Ӳ������HW���Ƿ����������SW(ETM2)
*
* @param[in]    pETM             ETM2.
* @param[in]    u32ConfigValue   ��������SYNCONF�Ĵ���.
*
* @return none.
*
*****************************************************************************/
void  ETM_SyncConfigActivate(ETM_Type *pETM, uint32_t u32ConfigValue)
{
    ASSERT((ETM2 == pETM));
    pETM->SYNCONF |= u32ConfigValue;   
}

/*****************************************************************************//*!
*
* @brief ���üĴ��� ETMx_SYNCONF����λ���,��������������������Ŀ����Ƿ���Ӳ������HW���Ƿ����������SW(ETM2)
*
* @param[in]    pETM             ETM2.
* @param[in]    u32ConfigValue   ��������SYNCONF�Ĵ���.
*
* @return none.
*
*****************************************************************************/
void  ETM_SyncConfigDeactivate(ETM_Type *pETM, uint32_t u32ConfigValue)
{
    ASSERT((ETM2 == pETM));
    pETM->SYNCONF &= ~u32ConfigValue;   
}

/*****************************************************************************//*!
*
* @brief ���ûص��������.
*
* @param[in]    pETM            ָ������ETM��ʱ������һ���Ļ�ַ.
* @param[in]    pfnCallback     �ص�������ַ.
*
* @return none.
*
*****************************************************************************/
void  ETM_SetCallback(ETM_Type *pETM, ETM_CallbackPtr pfnCallback)
{
   ETM_Callback[((uint32_t)pETM - (uint32_t)ETM0_BASE)>>12] = pfnCallback;
}



/*****************************************************************************//*!
*
* @brief ETM0ͨ���жϷ�����.
*        
* @param none
*
* @return none 
*
*****************************************************************************/
void ETM0_Isr(void)
{
    if(ETM_Callback[0])
    {
        ETM_Callback[0]();
    }
}

/*****************************************************************************//*!
*
* @brief ETM1ͨ���жϷ�����.
*        
* @param none
*
* @return none 
*
*****************************************************************************/
void ETM1_Isr(void)
{
    if(ETM_Callback[1])
    {
        ETM_Callback[1]();
    }
}

/*****************************************************************************//*!
*
* @brief ETM2ͨ���жϷ�����.
*        
* @param none
*
* @return none 
*
*****************************************************************************/
void ETM2_Isr(void)
{
    if(ETM_Callback[2])
    {
        ETM_Callback[2]();
    }
}



