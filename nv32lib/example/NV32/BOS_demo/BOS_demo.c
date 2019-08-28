/**********************************************************************
 *
 * 实验名称：BOS位带操作实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验效果：BOS位带操作，能够降低总线的占用率和CPU的执行时间，也可节省代码
 *           空间，以降低系统能耗的效果。
 *
************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "systick.h"
#include "bos.h"
#include "sysinit.h"

/************************************************************************************
*
* 使用BFI/UBFX功能需要注意的是，BFI/UBFX操作指定的外设偏移地址是addr[18:0]，addr[19]
* 用作"w"指示符的最低有效位,而非地址位。GPIO寄存器的偏移地址位是addr[19:0]，地址空间
* 在FF000.因此会有1bit 的地址冲突。为此在使用BFI/UBFX操作GPIO的寄存器时，使用的是内存
* 映射出来的地址空间，此时GPIO的起始地址将为F000，如果还使用原来的地址，命令将会无效。
*
**********************************************************************************/
#define GPIO_ALIAS_OFFSET       0x000F0000L                                        
#define GPIOB_PDOR_ALIAS        (((uint32_t)&GPIOB->PDOR)-GPIO_ALIAS_OFFSET)      /*!<GPIO寄存器内存映射地址空间 */   
/******************************************************************************/

int main (void);
void RTC_Task(void);
uint32_t BOS_LogicOPwithC(void);
uint32_t BOS_LogicOPwithBOS(void);
uint32_t BOS_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data);
uint32_t BOS_BFIwithBOS(void);

/*****************************************************************************/
#ifdef __GNUC__
#define __ramfunc __attribute__((section (".data")))
#endif

/********************************************************************/

int main (void)
{
    /*系统初始化*/
    sysinit();

    ICS_ConfigType  sICSConfig;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
    UART_ConfigType sConfig;

    printf("\r\nRunning the BOS_demo project.\r\n");
#if defined(__ICCARM__) 
    printf("Build by IAR\r\n");
#elif defined(__GNUC__) 
    printf("Build by GUNC\r\n");
#elif defined(__CC_ARM)
    printf("Build by MDK\r\n");
#else
    printf("Unrecognized compiler!\r\n");
#endif
    LED0_Init();  
    LED2_Init();

    /*配置RTC每隔一秒产生一次中断*/
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* 使能中断 */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /* 时钟频率1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /* 分频系数100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

    printf("\nIt is in FEE mode now,");
    UART_WaitTxComplete(TERM_PORT);   

    /*将时钟模式由 FEE 模式切换到 FEI 模式*/ 
    sICSConfig.u32ClkFreq = 10000;
    ICS_SwitchMode(FEE,FEI, &sICSConfig);

    /* 由于总线时钟发生改变，从新初始化UART模块*/
    sConfig.u32SysClkHz = 40000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEI mode.\n");

    OSC_Enable();

    /* 开始测试常规C位操作和BOS位操作CPU执行时间 */
    printf("Logic operation in C takes %d ticks!\r\n", BOS_LogicOPwithC());
    printf("Logic operation with BOS takes %d ticks!\r\n", BOS_LogicOPwithBOS());
    /*! 
     * 配置 PTG0 到 PTG3 引脚为输出引脚
     */
    GPIOB->PDDR |= (0xF << 16);             /* 配置PTG0~PTG3引脚为输出 */
    GPIOB->PIDR &= ~(0xF << 16);            /* 注: 为了从引脚中读到正确的数据，必须清理PIDR寄存器 */

    printf("Bit field operation in C takes %d ticks!\r\n", BOS_BFIwithC((uint32_t*)&GPIOB->PDOR,16,4-1, 5<<16));  
    printf("Bit field operation with BOS takes %d ticks!\r\n", BOS_BFIwithBOS());  
    printf("Test completed!\n");
    while(1)
    {
    } 
}

/*****************************************************************************//*!
*
* @brief 采用常规C对寄存器执行异或操作.
*       
* @param  none
*
* @return none
*
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BOS_LogicOPwithC(void)
#elif defined(__CC_ARM)
uint32_t BOS_LogicOPwithC(void)
#endif
{
    uint32_t    u32PortVal = 0;
    uint32_t    u32LogicOPTicks;    
          
    /*! 
     * 配置PTA1 作为输出引脚
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02; /* 注：为了从引脚中读到正确的数据，端口输入禁用寄存器对应位清零 */

    /*初始化SysTick定时器，计算代码执行时间*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/*清除当前值寄存器 */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    /*! \
    * 常规的读、修改、写顺序去切换一个GPIO输出 
    */
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
    /* 测量代码执行的时间 */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}


/*****************************************************************************//*!
*
* @brief 采用BOS位带操作执行异或操作 .
*       
* @param  none
*
* @return none
*
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BOS_LogicOPwithBOS(void)
#elif defined(__CC_ARM)
uint32_t BOS_LogicOPwithBOS(void)
#endif
{
    uint32_t    u32LogicOPTicks;
    /*! 
    * 配置PTA1 作为输出引脚
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           /* 注：为了从引脚中读到正确的数据，端口输入禁用寄存器对应位清零 */

    /*初始化SysTick定时器，计算代码执行时间*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/*清除当前值寄存器 */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    /* 测量代码执行的时间 */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
}

/*****************************************************************************//*!
*
* @brief 采用常规C，对寄存器执行一段数据的插入.
*       
* @param[in] pAddr          指向目标地址
* @param[in] u8BitPos       插入数据的其实位
* @param[in] u8FieldWidth   插入数据宽度-1
* @param[in] u32Data        要插入寄存器的数据 
*                         
* @return none
*
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BOS_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data)
#elif defined(__CC_ARM)
uint32_t BOS_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data)
#endif
{
    uint32_t    u32RegVal;
    uint32_t    u32Mask;  
    uint32_t    u32LogicOPTicks;

    /*初始化SysTick定时器，计算代码执行时间*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;		/*清除当前值寄存器 */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    u32RegVal = *pAddr;   
    u32Mask = ((1 << (u8FieldWidth+1)) - 1) << u8BitPos;
    u32RegVal = (u32RegVal & ~u32Mask)|((u32Data) & u32Mask);
    *pAddr = u32RegVal;  
    /* 测量代码执行的时间 */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
}


/*****************************************************************************//*!
*
* @brief 采用BOS位操作对寄存器执行一段数据的插入
*       
* @param  none
*
* @return none
*
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BOS_BFIwithBOS(void)
#elif defined(__CC_ARM)
uint32_t BOS_BFIwithBOS(void) 
#endif
{
      
    uint32_t    u32LogicOPTicks;
    uint32_t    u32Data = (0x5 << 16);
    uint32_t    u32Addr = GPIOB_PDOR_ALIAS;
    /* 配置 PTG0~PTG3 为输出引脚 */
    GPIOB->PDDR |= (0xF << 16);              /* 配置 PTG0~PTG3 为输出引脚 */
    GPIOB->PDOR =  0;                        /*PTG0 to PTG3输出为0 */
    GPIOB->PIDR &= ~(0xF << 16);            /* 注：为了从引脚中读到正确的数据，端口输入禁用寄存器对应位清零 */
    
   /*初始化SysTick定时器，计算代码执行时间*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;		/*清除当前值寄存器 */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */
    BOS_BITFIELD_INSERT(u32Addr,16,4) =  u32Data;  /* 写 5 位数据到16~19位*/
    /* 测量代码执行的时间 */   
    u32LogicOPTicks = SysTick->VAL;

    /* check if the data field is inserted correctly (canceled)*/
    /*
    if(u32Data != GPIOB->PDIR)
    {
        printf("Error: bitfield insertion with BOS failed!\n");
    }
    */
    return (SysTick->LOAD - u32LogicOPTicks);  
}

/*****************************************************************************//*!
*
* @brief RTC回调函数
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void RTC_Task(void)
{
    /* 切换 LED1 */
    LED0_Toggle();
}
/********************************************************************/
