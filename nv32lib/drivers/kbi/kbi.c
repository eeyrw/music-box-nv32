/**************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       kbi.c
 * @brief      �����ж�(KBI)������
 * @author     Navota
 * @date       2017-1-1
 ****************************************************************************/
#include "common.h"
#include "kbi.h"

/****************************************************************************!
 * @ ���KBI�ص������ӿ�
 ****************************************************************************/
KBI_CallbackType KBI_Callback[KBI_MAX_NO] = {(KBI_CallbackType)NULL};

/*****************************************************************************//*!
*
* @brief ��ʼ��KBIģ��.
*        
* @param[in]  pKBI       ָ��KBIģ��.
* @param[in]  pConfig    ָ��KBI���ýṹ��
*
* @return none.
*
* @see KBI_DeInit.
*
*****************************************************************************/
void KBI_Init(KBI_Type *pKBI, KBI_ConfigType *pConfig)
{
#if defined(CPU_NV32)
    uint16_t    i;
    uint8_t     sc = 0;
    uint8_t     u8Port;
    uint8_t     u8PinPos;
    uint16_t    u16PinMapping[KBI_MAX_NO][8] = 
    {
        {
            0, 1, 2, 3, 8, 9, 10, 11           /* KBI0�ж�����������GPIOA�Ĵ����е�λ��*/
        },
        {
            24, 25, 26, 27, 28, 29, 30, 31      /*KBI1�ж�����������GPIOA�Ĵ����е�λ��*/
        }
    };
#elif defined(CPU_NV32M3)
    uint16_t    i;
    uint8_t     sc = 0;
    uint8_t     u8Port;
    uint8_t     u8PinPos;
    uint16_t    u16PinMapping[KBI_MAX_NO][8] = 
    {
        {
            0, 1, 2, 3, 8, 9, 10, 11           /* KBI0�ж�����������GPIOA�Ĵ����е�λ��*/
        },
        {
            20, 21, 16, 17, 18, 19, 12, 13     /* KBI1�ж�����������GPIOA�Ĵ����е�λ��*/
        }
    };
#elif defined(CPU_NV32M4)
     uint32_t    i;
     uint32_t     sc = 0;
     uint32_t     u8Port;
     uint32_t     u8PinPos;

     uint32_t    u16PinMapping[KBI_MAX_NO][KBI_MAX_PINS_PER_PORT] =
    {
        {/* KBI0P0~KBI0P31 �ж�����������GPIOA�Ĵ����е�λ�� */
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31           
        },
        {/* KBI1P0~KBI1P31�ж�����������GPIOA�Ĵ����е�λ�� */
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31		   
        }
    };
#endif	 
 
    
    if(KBI0 == pKBI)
    {
        SIM->SCGC   |= SIM_SCGC_KBI0_MASK;             /* ʹ��KBI0ģ�������ʱ�� */
        u8Port      =  0;
    }
    else if (KBI1 == pKBI)
    {        
        SIM->SCGC   |= SIM_SCGC_KBI1_MASK;             /* ʹ��KBI1ģ�������ʱ�� */
        u8Port      =  1;
    }
    
    /*�趨KBI�жϼ��ģʽ*/
    sc          = pConfig->sBits.bMode;
    pKBI->SC    = sc;

    /* ����KBI�ж��������� */
    for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
    {
        if(pConfig->sPin[i].bEn)
        {
            pKBI->PE    |= (1<<i);                      /* ʹ��I/O����ΪKBI�ж���������*/
            pKBI->ES    = (pKBI->ES & ~(1<<i)) | (pConfig->sPin[i].bEdge << i);     
            u8PinPos = u16PinMapping[u8Port][i];
            ASSERT(!(u8PinPos & 0x80));
		#if defined(CPU_NV32)|| defined(CPU_NV32M3)	
            FGPIOA->PIDR  &= ~(1<<u8PinPos);              /* ʹ��GPIO����*/     
            FGPIOA->PDDR  &= ~(1<<u8PinPos);              /* ��������Ϊͨ������  */  
            PORT->PUEL  |= (1<<u8PinPos);                 /* ʹ���ڲ�����*/
        #elif defined(CPU_NV32M4)
		   if (u8Port == 0)   /* KBI0 */
           {
		   	FGPIOA->PIDR  &= ~(1<<u8PinPos);                 /* ʹ��GPIO����*/       
            FGPIOA->PDDR  &= ~(1<<u8PinPos);             /* ��������Ϊͨ������  */  
            PORT->PUE0  |= (1<<u8PinPos);                /* ʹ���ڲ�����*/
           }
		   else if (u8Port == 1)   /* KBI1 */
           {
		   	FGPIOB->PIDR  &= ~(1<<u8PinPos);                /* ʹ��GPIO����*/      
            FGPIOB->PDDR  &= ~(1<<u8PinPos);            /* ��������Ϊͨ������  */  
            PORT->PUE1  |= (1<<u8PinPos);               /* ʹ���ڲ�����*/
           }
		#endif  
		}
    }
    
    #if defined(CPU_NV32M4)
    /*Reset KBI_SP register*/
	sc = pConfig->sBits.bRstKbsp<<KBI_SC_RSTKBSP_SHIFT;
	pKBI->SC    |= sc;
	
    /*Real KBI_SP register enable*/
	sc = pConfig->sBits.bKbspEn<<KBI_SC_KBSPEN_SHIFT;
	pKBI->SC    |= sc;
	#endif
  
	/*����жϱ�־λ*/
    pKBI->SC    = sc;
    
    /* ʹ��KBI�ж� */
    if(pConfig->sBits.bIntEn)
    {
        pKBI->SC    |=  KBI_SC_KBIE_MASK;
        
        if(KBI0 == pKBI)
        {
            NVIC_EnableIRQ(KBI0_IRQn);
        }
        else
        {
            NVIC_EnableIRQ(KBI1_IRQn);            
        }
    }
}

/*****************************************************************************//*!
*
* @brief ����KBI�ص�������ͨ���жϷ���������
*        
* @param[in] pKBI          ָ��KBIģ��.
* @param[in] pfnCallback   ָ��ص�����.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void KBI_SetCallback(KBI_Type *pKBI, KBI_CallbackType pfnCallback)
{
    if(KBI0 == pKBI)
    {
        KBI_Callback[0] = pfnCallback;
    }
    else
    {
        KBI_Callback[1] = pfnCallback;        
    }
}

/*****************************************************************************//*!
*
* @brief ��λKBIģ��.
*        
* @param[in]  pKBI        ָ��KBIģ��.
*         
* @return none.
*
* @see KBI_Init.
*
*****************************************************************************/
void KBI_DeInit(KBI_Type *pKBI)
{
    if(KBI0 == pKBI)
    {
        NVIC_DisableIRQ(KBI0_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(KBI1_IRQn);        
    }
    
    pKBI->PE = 0;
    pKBI->SC = 0;
    pKBI->ES = 0;
    
    if(KBI0 == pKBI)
    {
        SIM->SCGC   &= ~SIM_SCGC_KBI0_MASK;             /* ����KBI0ģ������ʱ�� */
    }
    else
    {
        SIM->SCGC   &= ~SIM_SCGC_KBI1_MASK;             /* ����KBI1ģ������ʱ�� */       
    }
}

/*****************************************************************************//*!
*
* @brief KBI0ģ���жϷ�����.
*        
* @param  none. 
*
* @return none.
*
*****************************************************************************/

void KBI0_Isr(void)	
{
  KBI0->SC |= KBI_SC_KBACK_MASK;                        /*����жϱ�־λ */

  if(KBI_Callback[0])
  {
      KBI_Callback[0]();
  }
}

/*****************************************************************************//*!
*
* @brief KBI1ģ���жϷ�����
*        
* @param  none. 
*
* @return none.
*
*
*****************************************************************************/

void KBI1_Isr(void)	
{
  KBI1->SC |= KBI_SC_KBACK_MASK;                        /*����жϱ�־λ*/
 
  if(KBI_Callback[1])
  {
      KBI_Callback[1]();
  }
}

