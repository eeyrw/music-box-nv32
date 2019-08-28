/******************************************************************************
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
#undef  VECTOR_038 
#define VECTOR_038 PIT_Ch0Isr   /*!< Vector 38 points to PIT channel 0 interrupt service routine */

#undef  VECTOR_039 
#define VECTOR_039 PIT_Ch1Isr   /*!< Vector 39 points to PIT channel 1 interrupt service routine */

extern void PIT_Ch1Isr(void);
extern void PIT_Ch0Isr(void);

#endif  //__ISR_H

/* End of "isr.h" */
