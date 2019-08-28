/******************************************************************************
*
* @brief provide general-purpose memory testing functions. 
*
*******************************************************************************/

#ifndef _memtest_h
#define _memtest_h


/*
 * Define NULL pointer value.
 */
#ifndef NULL
#define NULL  (void *) 0
#endif

/*
 * Set the data bus width.
 */
typedef unsigned long datum;

/*
 * Function prototypes.
 */
datum   memTestDataBus(volatile datum * address);
datum * memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes);
datum * memTestDevice(volatile datum * baseAddress, unsigned long nBytes);


#endif /* _memtest_h */

