/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
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

#undef  VECTOR_036
#define VECTOR_036 RTC_Isr


#undef  VECTOR_028
#undef  VECTOR_029
#undef  VECTOR_030
#define VECTOR_028  UART0_Isr
#define VECTOR_029  UART1_Isr
#define VECTOR_030  UART2_Isr


extern void RTC_Isr(void);
extern void UART0_Isr(void);
extern void UART1_Isr(void);
extern void UART2_Isr(void);

#endif  //__ISR_H

/* End of "isr.h" */
