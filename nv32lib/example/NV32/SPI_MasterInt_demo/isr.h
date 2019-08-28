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

#undef  VECTOR_026
#define VECTOR_026      SPI0_Isr          /*!< Vector 36 points to RTC interrupt service routine */

#undef  VECTOR_027
#define VECTOR_027      SPI1_Isr          /*!< Vector 36 points to RTC interrupt service routine */

extern void RTC_Isr(void);

void SPI0_Isr(void);
void SPI1_Isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
