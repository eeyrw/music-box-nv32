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
 */
#undef  VECTOR_036
#define VECTOR_036      RTC_Isr          /*!< Vector 36 points to RTC interrupt service routine */

#undef  VECTOR_015
#define VECTOR_015      SysTick_Isr          /*!< Vector 15 points to SysTick interrupt service routine */

#undef  VECTOR_040
#define VECTOR_040 KBI0_Isr

extern void KBI0_Isr(void);
extern void SysTick_Isr(void);
extern void RTC_Isr(void);

#endif  //__ISR_H

/* End of "isr.h" */
