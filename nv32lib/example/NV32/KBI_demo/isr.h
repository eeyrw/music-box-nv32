/******************************************************************************
*
* @brief define interrupt service routines referenced by the vector table. 
*
* Note: Only "vectors.c" should include this header file.
*
*******************************************************************************
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
/*
#undef  VECTOR_036
#define VECTOR_036 RTC_Isr

// ISR(s) are defined in your project directory.
extern void RTC_Isr(void);
*/

#undef  VECTOR_036
#define VECTOR_036 RTC_Isr
#undef  VECTOR_040
#define VECTOR_040 KBI0_Isr
#undef  VECTOR_041
#define VECTOR_041 KBI1_Isr

extern void RTC_Isr(void);
extern void KBI0_Isr(void);
extern void KBI1_Isr(void);

#endif  //__ISR_H

/* End of "isr.h" */
