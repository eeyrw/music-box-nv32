/**************************************************************************!     
 *     技术讨论：QQ群  123763203
 *     官网    ：www.navota.com
 *
 * @file       gpio.c
 * @brief      通用输入输出模块(GPIO)函数库
 * @author     Navota
 * @date       2018-3-1
 **************************************************************************/
#include "gpio.h"

//GPIOA | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
// Px		| D7 | D6	| D5 | D4	|	D3 | D2	|	D1 | D0	|	C7 | C6	|	C5 | C4 |	C3 | C2 | C1 | C0 | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |

//GPIOB | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
// Px		| H7 | H6	| -- | --	|	-- | H2	|	H1 | H0	|	-- | --	|	-- | -- |	G3 | G2 | G1 | C0 | F7 | F6 | F5 | F4 | F3 | F2 | F1 | F0 | E7 | E6 | E5 | E4 | E3 | E2 | E1 | E0 |

/*****************************************************************************/ /*!
* @brief    复位GPIO模块.
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
    pGPIO->PCOR = 0x00000000; /* 端口清零输出寄存器 */
    pGPIO->PDDR = 0x00000000; /* 端口数据方向寄存器 */
                              //pGPIO->PDIR = 0x00000000;   /* 端口数据输入寄存器 */
    pGPIO->PDOR = 0x00000000; /* 端口数据输出寄存器 */
    pGPIO->PIDR = 0xFFFFFFFF; /* 端口输入禁用寄存器 */
    pGPIO->PSOR = 0x00000000; /* 端口置位输出寄存器 */
    pGPIO->PTOR = 0x00000000; /* 端口切换输出寄存器 */
}

/*****************************************************************************/ /*!
* @brief    初始化GPIO引脚属性
*        
* @param[in] pGPIO       GPIOA/GPIOB.
* @param[in] u32PinMask  32位引脚掩码(GPIO_PTA0_MASK, GPIO_PTA1_MASK...)
* @param[in] sGpioType   引脚属性(输入GPIO_PinInput、输出GPIO_PinOutput、输出高电流驱动GPIO_PinOutput_HighCurrent、输入并开启上拉GPIO_PinInput_InternalPullup）
*
* @return   none
*
* @ 注：
*   . 如果引脚配置为输入，禁用高电流驱动
*   . 如果引脚配置为输出，禁用内部上拉，内部上拉仅为输入模式使用
*     仅PTH1/0, PTE1/0, PTD1/0, PTB5/4 支持高电流驱动.
*****************************************************************************/
void GPIO_Init(GPIO_Type *pGPIO, uint32_t u32PinMask, GPIO_PinConfigType sGpioType)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    /* 配置GPIO为输入或者输出 */
    if ((sGpioType == GPIO_PinOutput) || (sGpioType == GPIO_PinOutput_HighCurrent))
    {
        pGPIO->PDDR |= u32PinMask; /* 引脚配置为通用输出*/
        pGPIO->PIDR |= u32PinMask; /* 置位端口输入禁用寄存器*/
    }
    else if ((sGpioType == GPIO_PinInput) || (sGpioType == GPIO_PinInput_InternalPullup))
    {
        pGPIO->PDDR &= ~u32PinMask; /* 引脚配置为通用输入 */
        pGPIO->PIDR &= ~u32PinMask; /* 清零端口输入禁用寄存器 */
    }
    /* 设置GPIO端口输入上拉 */
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
    /* 设置GPIO端口为高电流驱动输出 */
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
* @brief    切换GPIO端口数据输出
*        
* @param[in] pGPIO        GPIOA/GPIOB.
* @param[in] u32PinMask   32位引脚掩码(GPIO_PTA0_MASK, GPIO_PTA1_MASK...)
*
* @return none
*
*****************************************************************************/
void GPIO_Toggle(GPIO_Type *pGPIO, uint32_t u32PinMask)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    pGPIO->PTOR = u32PinMask; /* 32位引脚掩码确定要切换输出的引脚 */
}

/*****************************************************************************/ /*!
* @brief  读取端口输入数据寄存器
*        
* @param[in] pGPIO     GPIOA/GPIOB.
*
* @return   		   GPIOx->PDIR端口输入寄存器32位数值
*
*****************************************************************************/
uint32_t GPIO_Read(GPIO_Type *pGPIO)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    return (pGPIO->PDIR); /* 读端口数据输入寄存器 */
}

/*****************************************************************************/ /*!
* @brief 读取某个引脚的电平
*        
* @param[in] pGPIO      指向GPIO模块  GPIOA/GPIOB.
* @param[in] GPIO_Pin   GPIO引脚名 (GPIO_PTA0PIO_PTA1...)
*
* @return  端口数据寄存器某一位的值
*
*****************************************************************************/
uint8_t GPIO_BitRead(GPIO_PinType GPIO_Pin)
{
    uint8_t data = 0;

    ASSERT(GPIO_Pin < GPIO_PIN_MAX);

    if (GPIO_Pin < GPIO_PTE0)
    {
        if (((1 << GPIO_Pin) & GPIOA->PDIR) > 0) /*判断要读取的位，对应的数值是1还是0*/
            data = 0x1;                          /* 如果是1返回1，是0则返回0 */
        else
            data = 0x0;
    }

    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);

        if (((1 << GPIO_Pin) & GPIOB->PDIR) > 0) /*判断要读取的位，对应的数值是1还是0*/
            data = 0x1;                          /* 如果是1返回1，是0则返回0 */
        else
            data = 0x0;
    }

    return data;
}

/*****************************************************************************/ /*!
* @brief  写数据到端口数据输出寄存器 
*        
* @param[in] pGPIO      GPIOA/GPIOB.
* @param[in] u32Value   写入到端口数据输出寄存器GPIOx->PDOR的值
*
* @return   none
*
*****************************************************************************/
void GPIO_Write(GPIO_Type *pGPIO, uint32_t u32Value)
{
    ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));

    pGPIO->PDOR = u32Value; /* 写数据到端口数据输出寄存器 */
}

/*****************************************************************************/ /*!
* @brief   初始化GPIO引脚属性
*        
* @param[in] GPIO_Pin        GPIO引脚名 (GPIO_PTA0、PIO_PTA1...)
* @param[in] GPIO_PinConfig  引脚属性(输入GPIO_PinInput、输出GPIO_PinOutput、输出高电流驱动GPIO_PinOutput_HighCurrent、输入并开启上拉GPIO_PinInput_InternalPullup）
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
            GPIOA->PDDR |= (1 << GPIO_Pin); /* 引脚配置为通用输出 */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* 端口输入禁用寄存器置一*/
            PORT->PUEL &= ~(1 << GPIO_Pin); /* 禁用内部上拉 */
            break;
        case GPIO_PinInput:
            GPIOA->PDDR &= ~(1 << GPIO_Pin); /* 引脚配置为通用输入 */
            GPIOA->PIDR &= ~(1 << GPIO_Pin); /* 端口输入禁用寄存器清零 */
            PORT->PUEL &= ~(1 << GPIO_Pin);  /* 禁用内部上拉*/
            break;
        case GPIO_PinInput_InternalPullup:
            GPIOA->PDDR &= ~(1 << GPIO_Pin); /* 引脚配置为通用输入  */
            GPIOA->PIDR &= ~(1 << GPIO_Pin); /* 端口输入禁用寄存器清零  */
            PORT->PUEL |= (1 << GPIO_Pin);   /* 使能内部上拉 */
            break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1 << GPIO_Pin); /* 引脚配置为通用输出  */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* 端口输入禁用寄存器置一 */
            PORT->PUEL &= ~(1 << GPIO_Pin); /* 禁用内部上拉*/
            break;
        }
    }
    else if (GPIO_Pin < GPIO_PIN_MAX)
    {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOB->PDDR |= (1 << GPIO_Pin); /* 引脚配置为通用输出 */
            GPIOB->PIDR |= (1 << GPIO_Pin); /* 端口输入禁用寄存器置一 */
            PORT->PUEH &= ~(1 << GPIO_Pin); /* 禁用内部上拉*/
            break;
        case GPIO_PinInput:
            GPIOB->PDDR &= ~(1 << GPIO_Pin); /* 引脚配置为通用输入 */
            GPIOB->PIDR &= ~(1 << GPIO_Pin); /* 端口输入禁用寄存器清零  */
            PORT->PUEH &= ~(1 << GPIO_Pin);  /* 禁用内部上拉*/
            break;
        case GPIO_PinInput_InternalPullup:
            GPIOB->PDDR &= ~(1 << GPIO_Pin); /* 引脚配置为通用输入 */
            GPIOB->PIDR &= ~(1 << GPIO_Pin); /* 端口输入禁用寄存器清零  */
            PORT->PUEH |= (1 << GPIO_Pin);   /* 使能内部上拉*/
            break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1 << GPIO_Pin); /* 引脚配置为通用输出  */
            GPIOA->PIDR |= (1 << GPIO_Pin); /* 端口输入禁用寄存器置一 */
            PORT->PUEL &= ~(1 << GPIO_Pin); /* 禁用内部上拉*/
            break;
        }
    }

    /* 配置GPIO输出高电流驱动 */
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
* @brief 切换GPIO端口数据切换输出
*        
* @param[in] GPIO_Pin        GPIO引脚名 (GPIO_PTA0PIO_PTA1...)
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
* @brief  GPIO端口数据输出置1
*        
* @param[in] GPIO_Pin       GPIO引脚名 (GPIO_PTA0PIO_PTA1...)
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
* @brief    GPIO端口数据输出清零
*        
* @param[in] GPIO_Pin        GPIO引脚名 (GPIO_PTA0PIO_PTA1...)
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
