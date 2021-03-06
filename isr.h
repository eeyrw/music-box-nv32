/******************************************************************************
*
* @brief define interrupt service routines referenced by the vector table. 
*
* Note: Only "vectors.c" should include this header file.
*
*******************************************************************************
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H


/* Example */
/*
#undef  VECTOR_036
#define VECTOR_036 RTC_Isr

// ISR(s) are defined in your project directory.
extern void RTC_Isr(void);
*/

/*!
 * @brief define interrupt service routine for different vectors.
 *
 * 
 * 
 * 
 */

#undef  VECTOR_038 
#define VECTOR_038 PIT_Ch0Isr_Override   /*!< Vector 38 points to PIT channel 0 interrupt service routine */

#undef  VECTOR_039 
#define VECTOR_039 PIT_Ch1Isr   /*!< Vector 39 points to PIT channel 1 interrupt service routine */

extern void PIT_Ch1Isr(void);
extern void PIT_Ch0Isr_Override(void);

#undef  VECTOR_036
#define VECTOR_036      RTC_Isr          /*!< Vector 36 points to RTC interrupt service routine */

extern void RTC_Isr(void);

#undef  VECTOR_015
#define VECTOR_015      SysTick_Isr          /*!< Vector 15 points to SysTick interrupt service routine */

extern void SysTick_Isr(void);


#undef  VECTOR_028
#define VECTOR_028  UART0_Isr

extern void UART0_Isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
