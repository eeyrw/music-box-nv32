/******************************************************************************
*
* @brief Define interrupt service routines referenced by the vector table. 
*
* Note: Only "vectors.c" should include this header file.
*
*******************************************************************************
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 

/******************************************************************************
* Macros
******************************************************************************/
/* Example */
/*
#undef  VECTOR_036
#define VECTOR_036 RTC_Isr

// ISR(s) are defined in your project directory. 
extern void RTC_Isr(void);
*/

#undef  VECTOR_032
#define VECTOR_032 ACMP0_Isr

#undef  VECTOR_037
#define VECTOR_037 ACMP1_Isr

extern void ACMP0_Isr(void);
extern void ACMP1_Isr(void);

#endif  /*__ISR_H */

/* End of "isr.h" */
