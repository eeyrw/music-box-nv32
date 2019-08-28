/**********************************************************************
 *
 * ʵ�����ƣ�BOSλ������ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч����BOSλ���������ܹ��������ߵ�ռ���ʺ�CPU��ִ��ʱ�䣬Ҳ�ɽ�ʡ����
 *           �ռ䣬�Խ���ϵͳ�ܺĵ�Ч����
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
* ʹ��BFI/UBFX������Ҫע����ǣ�BFI/UBFX����ָ��������ƫ�Ƶ�ַ��addr[18:0]��addr[19]
* ����"w"ָʾ���������Чλ,���ǵ�ַλ��GPIO�Ĵ�����ƫ�Ƶ�ַλ��addr[19:0]����ַ�ռ�
* ��FF000.��˻���1bit �ĵ�ַ��ͻ��Ϊ����ʹ��BFI/UBFX����GPIO�ļĴ���ʱ��ʹ�õ����ڴ�
* ӳ������ĵ�ַ�ռ䣬��ʱGPIO����ʼ��ַ��ΪF000�������ʹ��ԭ���ĵ�ַ���������Ч��
*
**********************************************************************************/
#define GPIO_ALIAS_OFFSET       0x000F0000L                                        
#define GPIOB_PDOR_ALIAS        (((uint32_t)&GPIOB->PDOR)-GPIO_ALIAS_OFFSET)      /*!<GPIO�Ĵ����ڴ�ӳ���ַ�ռ� */   
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
    /*ϵͳ��ʼ��*/
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

    /*����RTCÿ��һ�����һ���ж�*/
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* ʹ���ж� */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /* ʱ��Ƶ��1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /* ��Ƶϵ��100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

    printf("\nIt is in FEE mode now,");
    UART_WaitTxComplete(TERM_PORT);   

    /*��ʱ��ģʽ�� FEE ģʽ�л��� FEI ģʽ*/ 
    sICSConfig.u32ClkFreq = 10000;
    ICS_SwitchMode(FEE,FEI, &sICSConfig);

    /* ��������ʱ�ӷ����ı䣬���³�ʼ��UARTģ��*/
    sConfig.u32SysClkHz = 40000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEI mode.\n");

    OSC_Enable();

    /* ��ʼ���Գ���Cλ������BOSλ����CPUִ��ʱ�� */
    printf("Logic operation in C takes %d ticks!\r\n", BOS_LogicOPwithC());
    printf("Logic operation with BOS takes %d ticks!\r\n", BOS_LogicOPwithBOS());
    /*! 
     * ���� PTG0 �� PTG3 ����Ϊ�������
     */
    GPIOB->PDDR |= (0xF << 16);             /* ����PTG0~PTG3����Ϊ��� */
    GPIOB->PIDR &= ~(0xF << 16);            /* ע: Ϊ�˴������ж�����ȷ�����ݣ���������PIDR�Ĵ��� */

    printf("Bit field operation in C takes %d ticks!\r\n", BOS_BFIwithC((uint32_t*)&GPIOB->PDOR,16,4-1, 5<<16));  
    printf("Bit field operation with BOS takes %d ticks!\r\n", BOS_BFIwithBOS());  
    printf("Test completed!\n");
    while(1)
    {
    } 
}

/*****************************************************************************//*!
*
* @brief ���ó���C�ԼĴ���ִ��������.
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
     * ����PTA1 ��Ϊ�������
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02; /* ע��Ϊ�˴������ж�����ȷ�����ݣ��˿�������üĴ�����Ӧλ���� */

    /*��ʼ��SysTick��ʱ�����������ִ��ʱ��*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/*�����ǰֵ�Ĵ��� */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    /*! \
    * ����Ķ����޸ġ�д˳��ȥ�л�һ��GPIO��� 
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
    /* ��������ִ�е�ʱ�� */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}


/*****************************************************************************//*!
*
* @brief ����BOSλ������ִ�������� .
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
    * ����PTA1 ��Ϊ�������
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           /* ע��Ϊ�˴������ж�����ȷ�����ݣ��˿�������üĴ�����Ӧλ���� */

    /*��ʼ��SysTick��ʱ�����������ִ��ʱ��*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/*�����ǰֵ�Ĵ��� */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    BOS_XOR(&GPIOA->PDOR) = 0x02;
    /* ��������ִ�е�ʱ�� */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
}

/*****************************************************************************//*!
*
* @brief ���ó���C���ԼĴ���ִ��һ�����ݵĲ���.
*       
* @param[in] pAddr          ָ��Ŀ���ַ
* @param[in] u8BitPos       �������ݵ���ʵλ
* @param[in] u8FieldWidth   �������ݿ��-1
* @param[in] u32Data        Ҫ����Ĵ��������� 
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

    /*��ʼ��SysTick��ʱ�����������ִ��ʱ��*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;		/*�����ǰֵ�Ĵ��� */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    u32RegVal = *pAddr;   
    u32Mask = ((1 << (u8FieldWidth+1)) - 1) << u8BitPos;
    u32RegVal = (u32RegVal & ~u32Mask)|((u32Data) & u32Mask);
    *pAddr = u32RegVal;  
    /* ��������ִ�е�ʱ�� */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
}


/*****************************************************************************//*!
*
* @brief ����BOSλ�����ԼĴ���ִ��һ�����ݵĲ���
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
    /* ���� PTG0~PTG3 Ϊ������� */
    GPIOB->PDDR |= (0xF << 16);              /* ���� PTG0~PTG3 Ϊ������� */
    GPIOB->PDOR =  0;                        /*PTG0 to PTG3���Ϊ0 */
    GPIOB->PIDR &= ~(0xF << 16);            /* ע��Ϊ�˴������ж�����ȷ�����ݣ��˿�������üĴ�����Ӧλ���� */
    
   /*��ʼ��SysTick��ʱ�����������ִ��ʱ��*/
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;		/*�����ǰֵ�Ĵ��� */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */
    BOS_BITFIELD_INSERT(u32Addr,16,4) =  u32Data;  /* д 5 λ���ݵ�16~19λ*/
    /* ��������ִ�е�ʱ�� */   
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
* @brief RTC�ص�����
*        
* @param  none
*
* @return none
*
*****************************************************************************/

void RTC_Task(void)
{
    /* �л� LED1 */
    LED0_Toggle();
}
/********************************************************************/
