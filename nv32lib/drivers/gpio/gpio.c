/**************************************************************************!     
 *     �������ۣ�QQȺ  123763203
 *     ����    ��www.navota.com
 *
 * @file       gpio.c
 * @brief      ͨ���������ģ��(GPIO)������
 * @author     Navota
 * @date       2018-3-1
 **************************************************************************/
#include "gpio.h"

//GPIOA | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
// Px		| D7 | D6	| D5 | D4	|	D3 | D2	|	D1 | D0	|	C7 | C6	|	C5 | C4 |	C3 | C2 | C1 | C0 | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |

//GPIOB | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
// Px		| H7 | H6	| -- | --	|	-- | H2	|	H1 | H0	|	-- | --	|	-- | -- |	G3 | G2 | G1 | C0 | F7 | F6 | F5 | F4 | F3 | F2 | F1 | F0 | E7 | E6 | E5 | E4 | E3 | E2 | E1 | E0 |

/*****************************************************************************/ /*!
* @brief    ��λGPIOģ��.
*        
* @param[in] pGPIO     GPIOA/GPIOB.
*
* @return none
*
*****************************************************************************/
void GPIO_DeInit(GPIO_Type *pGPIO)
{
    /* Sanity check */
#if defined(CPU_NV32)
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
    pGPIO->PCOR = 0x00000000; /* �˿���������Ĵ��� */
    pGPIO->PDDR = 0x00000000; /* �˿����ݷ���Ĵ��� */
                              //pGPIO->PDIR = 0x00000000;   /* �˿���������Ĵ��� */
    pGPIO->PDOR = 0x00000000; /* �˿���������Ĵ��� */
    pGPIO->PIDR = 0xFFFFFFFF; /* �˿�������üĴ��� */
    pGPIO->PSOR = 0x00000000; /* �˿���λ����Ĵ��� */
    pGPIO->PTOR = 0x00000000; /* �˿��л�����Ĵ��� */
}

/*****************************************************************************/ /*!
* @brief    ��ʼ��GPIO��������
*        
* @param[in] pGPIO       GPIOA/GPIOB.
* @param[in] u32PinMask  32λ��������(GPIO_PTA0_MASK, GPIO_PTA1_MASK...)
* @param[in] sGpioType   ��������(����GPIO_PinInput�����GPIO_PinOutput������ߵ�������GPIO_PinOutput_HighCurrent�����벢��������GPIO_PinInput_InternalPullup��
*
* @return   none
*
* @ ע��
*   . �����������Ϊ���룬���øߵ�������
*   . �����������Ϊ����������ڲ��������ڲ�������Ϊ����ģʽʹ��
*     ��PTH1/0, PTE1/0, PTD1/0, PTB5/4 ֧�ָߵ�������.
*****************************************************************************/
void GPIO_Init(GPIO_Type *pGPIO, uint32_t u32PinMask, GPIO_PinConfigType sGpioType)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    /* ����GPIOΪ���������� */
    if ((sGpioType == GPIO_PinOutput) || (sGpioType == GPIO_PinOutput_HighCurrent))
    {
        pGPIO->PDDR |= u32PinMask; /* ��������Ϊͨ�����*/
        pGPIO->PIDR |= u32PinMask; /* ��λ�˿�������üĴ���*/
    }
    else if ((sGpioType == GPIO_PinInput) || (sGpioType == GPIO_PinInput_InternalPullup))
    {
        pGPIO->PDDR &= ~u32PinMask; /* ��������Ϊͨ������ */
        pGPIO->PIDR &= ~u32PinMask; /* ����˿�������üĴ��� */
    }
    /* ����GPIO�˿��������� */
    switch ((uint32_t)pGPIO)
    {
    case GPIOA_BASE:
        (sGpioType == GPIO_PinInput_InternalPullup) ? (PORT->PUEL |= u32PinMask) : (PORT->PUEL &= ~u32PinMask);
        break;
    case GPIOB_BASE:
        (sGpioType == GPIO_PinInput_InternalPullup) ? (PORT->PUEH |= u32PinMask) : (PORT->PUEH &= ~u32PinMask);
        break;
    default:
        break;
    }
    /* ����GPIO�˿�Ϊ�ߵ���������� */
    if (pGPIO == GPIOA)
    {
        if (u32PinMask & GPIO_PTB4_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
        }
        if (u32PinMask & GPIO_PTB5_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
        }
        if (u32PinMask & GPIO_PTD0_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
        }
        if (u32PinMask & GPIO_PTD1_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
        }
    }
    if (pGPIO == GPIOB)
    {
        if (u32PinMask & GPIO_PTE0_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
        }
        if (u32PinMask & GPIO_PTE1_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
        }
        if (u32PinMask & GPIO_PTH0_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
        }
        if (u32PinMask & GPIO_PTH1_MASK)
        {
            PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
        }
    }
}

/*****************************************************************************/ /*!
* @brief    �л�GPIO�˿��������
*        
* @param[in] pGPIO        GPIOA/GPIOB.
* @param[in] u32PinMask   32λ��������(GPIO_PTA0_MASK, GPIO_PTA1_MASK...)
*
* @return none
*
*****************************************************************************/
void GPIO_Toggle(GPIO_Type *pGPIO, uint32_t u32PinMask)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    pGPIO->PTOR = u32PinMask; /* 32λ��������ȷ��Ҫ�л���������� */
}

/*****************************************************************************/ /*!
* @brief  ��ȡ�˿��������ݼĴ���
*        
* @param[in] pGPIO     GPIOA/GPIOB.
*
* @return   		   GPIOx->PDIR�˿�����Ĵ���32λ��ֵ
*
*****************************************************************************/
uint32_t GPIO_Read(GPIO_Type *pGPIO)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    return (pGPIO->PDIR); /* ���˿���������Ĵ��� */
}

/*****************************************************************************/ /*!
* @brief ��ȡĳ�����ŵĵ�ƽ
*        
* @param[in] pGPIO      ָ��GPIOģ��  GPIOA/GPIOB.
* @param[in] GPIO_Pin   GPIO������ (GPIO_PTA0�GPIO_PTA1...)
*
* @return  �˿����ݼĴ���ĳһλ��ֵ
*
*****************************************************************************/
uint8_t GPIO_BitRead(GPIO_PinType GPIO_Pin)
{
    uint8_t data = 0;

    ASSERT(GPIO_Pin < GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        if (((1 << GPIO_Pin) & GPIOA->PDIR) > 0) /*�ж�Ҫ��ȡ��λ����Ӧ����ֵ��1����0*/
            data = 0x1;                          /* �����1����1����0�򷵻�0 */
        else
            data = 0x0;
    }

    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);

        if (((1 << GPIO_Pin) & GPIOB->PDIR) > 0) /*�ж�Ҫ��ȡ��λ����Ӧ����ֵ��1����0*/
            data = 0x1;                          /* �����1����1����0�򷵻�0 */
        else
            data = 0x0;
    }

    return data;
}

/*****************************************************************************/ /*!
* @brief  д���ݵ��˿���������Ĵ��� 
*        
* @param[in] pGPIO      GPIOA/GPIOB.
* @param[in] u32Value   д�뵽�˿���������Ĵ���GPIOx->PDOR��ֵ
*
* @return   none
*
*****************************************************************************/
void GPIO_Write(GPIO_Type *pGPIO, uint32_t u32Value)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    pGPIO->PDOR = u32Value; /* д���ݵ��˿���������Ĵ��� */
}

/*****************************************************************************/ /*!
* @brief   ��ʼ��GPIO��������
*        
* @param[in] GPIO_Pin        GPIO������ (GPIO_PTA0��PIO_PTA1...)
* @param[in] GPIO_PinConfig  ��������(����GPIO_PinInput�����GPIO_PinOutput������ߵ�������GPIO_PinOutput_HighCurrent�����벢��������GPIO_PinInput_InternalPullup��
*
* @return   none
*
*****************************************************************************/
void GPIO_PinInit(GPIO_PinType GPIO_Pin, GPIO_PinConfigType GPIO_PinConfig)
{
    /* Sanity check */
    ASSERT(GPIO_Pin < GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOA->PDDR |= (1 << GPIO_Pin); /* ��������Ϊͨ����� */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* �˿�������üĴ�����һ*/
            PORT->PUEL &= ~(1 << GPIO_Pin); /* �����ڲ����� */
            break;
        case GPIO_PinInput:
            GPIOA->PDDR &= ~(1 << GPIO_Pin); /* ��������Ϊͨ������ */
            GPIOA->PIDR &= ~(1 << GPIO_Pin); /* �˿�������üĴ������� */
            PORT->PUEL &= ~(1 << GPIO_Pin);  /* �����ڲ�����*/
            break;
        case GPIO_PinInput_InternalPullup:
            GPIOA->PDDR &= ~(1 << GPIO_Pin); /* ��������Ϊͨ������  */
            GPIOA->PIDR &= ~(1 << GPIO_Pin); /* �˿�������üĴ�������  */
            PORT->PUEL |= (1 << GPIO_Pin);   /* ʹ���ڲ����� */
            break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1 << GPIO_Pin); /* ��������Ϊͨ�����  */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* �˿�������üĴ�����һ */
            PORT->PUEL &= ~(1 << GPIO_Pin); /* �����ڲ�����*/
            break;
        }
    }
    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOB->PDDR |= (1 << GPIO_Pin); /* ��������Ϊͨ����� */
            GPIOB->PIDR |= (1 << GPIO_Pin); /* �˿�������üĴ�����һ */
            PORT->PUEH &= ~(1 << GPIO_Pin); /* �����ڲ�����*/
            break;
        case GPIO_PinInput:
            GPIOB->PDDR &= ~(1 << GPIO_Pin); /* ��������Ϊͨ������ */
            GPIOB->PIDR &= ~(1 << GPIO_Pin); /* �˿�������üĴ�������  */
            PORT->PUEH &= ~(1 << GPIO_Pin);  /* �����ڲ�����*/
            break;
        case GPIO_PinInput_InternalPullup:
            GPIOB->PDDR &= ~(1 << GPIO_Pin); /* ��������Ϊͨ������ */
            GPIOB->PIDR &= ~(1 << GPIO_Pin); /* �˿�������üĴ�������  */
            PORT->PUEH |= (1 << GPIO_Pin);   /* ʹ���ڲ�����*/
            break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1 << GPIO_Pin); /* ��������Ϊͨ�����  */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* �˿�������üĴ�����һ */
            PORT->PUEL &= ~(1 << GPIO_Pin); /* �����ڲ�����*/
            break;
        }
    }

    /* ����GPIO����ߵ������� */
    if (GPIO_PinConfig == GPIO_PinOutput_HighCurrent)
    {
        switch (GPIO_Pin)
        {
        case GPIO_PTB4:
            PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
            break;
        case GPIO_PTB5:
            PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
            break;
        case GPIO_PTD0:
            PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
            break;
        case GPIO_PTD1:
            PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
            break;
        case GPIO_PTE0:
            PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
            break;
        case GPIO_PTE1:
            PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
            break;
        case GPIO_PTH0:
            PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
            break;
        case GPIO_PTH1:
            PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
            break;
        default:
            break;
        }
    }
}

/*****************************************************************************/ /*!
* @brief �л�GPIO�˿������л����
*        
* @param[in] GPIO_Pin        GPIO������ (GPIO_PTA0�GPIO_PTA1...)
*
* @return   none
*
*****************************************************************************/
void GPIO_PinToggle(GPIO_PinType GPIO_Pin)
{
    ASSERT(GPIO_Pin <= GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        GPIOA->PTOR = (1 << GPIO_Pin);
    }
    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PTOR = (1 << GPIO_Pin);
    }
}

/*****************************************************************************/ /*!
* @brief  GPIO�˿����������1
*        
* @param[in] GPIO_Pin       GPIO������ (GPIO_PTA0�GPIO_PTA1...)
*
* @return   none
*
*****************************************************************************/
void GPIO_PinSet(GPIO_PinType GPIO_Pin)
{
    ASSERT(GPIO_Pin <= GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        GPIOA->PSOR = (1 << GPIO_Pin);
    }
    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PSOR = (1 << GPIO_Pin);
    }
}

/*****************************************************************************/ /*!
* @brief    GPIO�˿������������
*        
* @param[in] GPIO_Pin        GPIO������ (GPIO_PTA0�GPIO_PTA1...)
*
* @return   none
*
*****************************************************************************/
void GPIO_PinClear(GPIO_PinType GPIO_Pin)
{
    ASSERT(GPIO_Pin <= GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        GPIOA->PCOR = (1 << GPIO_Pin);
    }
    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PCOR = (1 << GPIO_Pin);
    }
}
