/**********************************************************************
 *
 * ʵ�����ƣ�FGPIO�˿���������л�ʵ��
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ������: KEIL���Ż��ȼ�������3���Ż�
 * ʵ��Ч����PB2����Ϊ������ţ��л�PB2����������ɲ���PB2������ε�����
 *          ���ɴﵽ�ں�ʱ��Ƶ�ʵ�1/2
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

    /*ϵͳ��ʼ��*/
    sysinit();
  
    printf("\nRunning the GPIO_OneCycle_demo project.\r\n");
    printf("\nBy default Core Clock is 40MHz.\r\n");
    printf("\nBy one cycle GPIO access, PTB2 output frequency is expected to be 20MHz.\r\n");

    /* ʹ������ʱ����PH2�� */
    SIM_EnableClockOutput();

    /* ����PB2Ϊ�������*/
    GPIO_Init(GPIOA, GPIO_PTB2_MASK, GPIO_PinOutput);

    /* PB2���Ϊ 0 */
    GPIO_PinClear(GPIO_PTB2);
    gpio_t();

}

void gpio_t(void)
{
    for(;;)
    {
        /* ͨ��FastGPIO�л�PB2���, ������ε�Ƶ��= �ں�ʱ��/2 */
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
