/**********************************************************************
 *
 * 实验名称：FGPIO端口数据输出切换实验
 * 实验平台：NV32开发板
 * 板载芯片：NV32F100FL64E
 * 实验条件: KEIL的优化等级调整到3级优化
 * 实验效果：PB2引脚为输出引脚，切换PB2数据输出，可测量PB2输出波形的周期
 *          最大可达到内核时钟频率的1/2
 *
 **********************************************************************/

#include "common.h"
#include "rtc.h"
#include "gpio.h"
#include "sim.h"
#include "sysinit.h"

int main (void);
void gpio_t(void);

/********************************************************************/
int main (void)
{  

    /*系统初始化*/
    sysinit();
  
    printf("\nRunning the GPIO_OneCycle_demo project.\r\n");
    printf("\nBy default Core Clock is 40MHz.\r\n");
    printf("\nBy one cycle GPIO access, PTB2 output frequency is expected to be 20MHz.\r\n");

    /* 使能总线时钟在PH2上 */
    SIM_EnableClockOutput();

    /* 配置PB2为输出引脚*/
    GPIO_Init(GPIOA, GPIO_PTB2_MASK, GPIO_PinOutput);

    /* PB2输出为 0 */
    GPIO_PinClear(GPIO_PTB2);
    gpio_t();

}

void gpio_t(void)
{
    for(;;)
    {
        /* 通过FastGPIO切换PB2输出, 输出波形的频率= 内核时钟/2 */
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
    }
}
/*****************************************************************************/
