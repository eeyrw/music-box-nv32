/*************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       sim.c
 * @brief      ϵͳ����ģ�飨SIM)������
 * @author     Navota
 * @date       2018-3-1
 *************************************************************************/

#include "common.h"
#include "sim.h"


#if defined(CPU_NV32)
/*****************************************************************************//*!
 *
 * @ ��ʼ��SIM�Ĵ���.
 * 
 * @ ���� pConfig       ָ��SIM���ýṹ��.  
 *
 * @ �޷���
 *
 * @ �ο�  SIM_ConfigType
 *
 *****************************************************************************/
void SIM_Init(SIM_ConfigType *pConfig)
{
    uint32_t    u32Sopt;
    uint32_t    u32PinSel;
    uint32_t    u32Scgc;
    uint32_t    u32BusDiv;
    /*
     * ��ʼ��SIM�Ĵ���
     */
    u32Sopt     = 0x0010000E; /*ʹ��SWD��RESET��NMI���� */
    u32PinSel   = 0;
    u32Scgc     = 0x00003000;  /* ʹ��SWD��FLASHģ�������ʱ�� */
    u32BusDiv   = 0;
    u32BusDiv = pConfig->sBits.bBusDiv;    /*����ʱ�ӷ�Ƶֵ*/ 
    if(pConfig->sBits.bDisableNMI)         /*����NMI����*/
    {
        u32Sopt &= ~SIM_SOPT_NMIE_MASK;        
    }
    if(pConfig->sBits.bDisableRESET)     	 /*����RSTPE����*/
    {
        u32Sopt &= ~SIM_SOPT_RSTPE_MASK;        
    }   
    if(pConfig->sBits.bDisableSWD)     		 /*����SWDE����*/
    {
        u32Sopt &= ~SIM_SOPT_SWDE_MASK;        
    } 
    if(pConfig->sBits.bEnableCLKOUT)  		 /*ʹ������ʱ��*/
    {
        u32Sopt |= SIM_SOPT_CLKOE_MASK;        
    } 
    if(pConfig->sBits.bETMSYNC)            /*ETM2ͬ��ѡ��*/
    {
        u32Sopt |= SIM_SOPT_ETMSYNC_MASK;  /*����ETM2ģ���PWMͬ������*/   
    }  
    if(pConfig->sBits.bRXDCE)       			 /*UAT0_RX��׽ѡ��*/
    {
        u32Sopt |= SIM_SOPT_RXDCE_MASK;    /*UAT0_RX�����źŽӵ�UART0ģ���ETM0ͨ��1*/       
    }
    if(pConfig->sBits.bTXDME)   					 /*URAT0_TX��׽ѡ��*/
    {
        u32Sopt |= SIM_SOPT_TXDME_MASK;  	 /*URAT0_TX���ӳ�䵽������ǰ��ETM0ͨ������*/     
    }
    if(pConfig->sBits.bACIC)             	 /*ģ��Ƚ��������벶��ʹ��*/
    {
        u32Sopt |= SIM_SOPT_ACIC_MASK;     /* ACMP0������ӵ�ETM1���ͨ��0*/
    }
        if(pConfig->sBits.bRTCC)
    {
        u32Sopt |= SIM_SOPT_RTCC_MASK;     /*RTC������ӵ�ETM1����ͨ��*/    
    }
        if(pConfig->sBits.bRXDFE)          /*URT0 RxD�˲���ѡ��*/ 
    {
        u32Sopt |= SIM_SOPT_RXDFE_MASK;    /*RXD0�����ź���ACMP0�˲���Ȼ��ע��UART0*/   
    }
    u32Sopt |= ((pConfig->u8BusRef & 0x07) << 16);     /*����ʱ��128��Ƶ*/  
    u32Sopt |= ((pConfig->u8Delay) << 24);             /*���ô�ETM2��ʼ��/ƥ�䵽����ADCת������ʱʱ��*/
    u32Sopt |= ((pConfig->sBits.u8ADHWT & 0x03) << 8); /*ѡ��ETM2ƥ����ΪADCת����Ӳ������Դ*/
    u32PinSel = pConfig->u32PinSel;
    u32Scgc = pConfig->u32SCGC;   
    
   /*д���ݵ�SIMģ��Ĵ��� */
    SIM->SOPT = u32Sopt;
    SIM->PINSEL = u32PinSel;
    SIM->SCGC = u32Scgc; 
    SIM->BUSDIV = u32BusDiv;
}
#endif

/****************************************************************************//*!
 *
 * @ ����SIMʱ��ѡͨ���ƼĴ�����ʹ�ܻ��߽��������ſ�ʱ��
 * 
 * @ ����  u32PeripheralMask    ����ʱ������
 * @ ����  u8GateOn             1:����, 0:�ر�.  
 *
 * @ �޷���
 *
   *****************************************************************************/

void SIM_SetClockGating(uint32_t u32PeripheralMask, uint8_t u8GateOn)
{
    uint32_t    u32Scgc;
	
    u32Scgc     = SIM->SCGC;
    
    if(u8GateOn)
    {
        u32Scgc |= u32PeripheralMask;
    }
    else
    {
        u32Scgc &= ~u32PeripheralMask;        
    }

    SIM->SCGC = u32Scgc;
}

/*****************************************************************************//*!
 *
 * @ ��ȡ��Ӧ��״̬��־λ.
 * 
 * @ ����  u32StatusMask    ָʾҪ����ȡ��״̬
 *
 * @ ����״̬.
 *
 *****************************************************************************/

uint32_t SIM_GetStatus(uint32_t u32StatusMask)
{
    uint32_t    u32Status;

    u32Status = SIM->SRSID & u32StatusMask;  //��ȡ״̬��־λ
	
    return (u32Status);
}

/*****************************************************************************//*!
 *
 * @ ����Ӧ��ID�Ĵ���.
 *
 * @ ����  u8ID    ID������. 
 *
 * @ ���� ID
 *
 * �ο� IDType
 *
 *****************************************************************************/
uint8_t SIM_ReadID(IDType sID)
{
    uint32_t    u32ID;
    uint8_t u8IDOffset[4] = 
    { 
        28, 24, 20,16
    };  
    u32ID = (SIM->SRSID >> u8IDOffset[sID]) & 0x0F;   //��ȡID
    return (u32ID);
}

