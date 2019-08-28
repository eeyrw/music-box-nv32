/******************************************************************************
* @brief provide systick utility routines.
*
*******************************************************************************/

#ifndef __SYSTICK_H
#define __SYSTICK_H
typedef void (*SysTick_CallbackType)(void);		

#define SYS_COUNT_ETME		10 //ms
#define SYS_COUNT 				MCU_CLCOK*SYS_COUNT_ETME/16
/* Global variables */
extern uint32_t cnt_start_value;
extern uint32_t cnt_end_value;
extern uint32_t overhead;
void SysTick_SetCallBack(SysTick_CallbackType pSysTick_CallBack);

/* Function declaration */
void systick_init(void);
void systick_disable(void);
void cal_systick_read_overhead(void);
//void delay_us(int n);
//void delay_ms(int n);
void SysTick_SetCallBack(SysTick_CallbackType pSysTick_CallBack);
#endif
