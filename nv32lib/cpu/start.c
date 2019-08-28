/******************************************************************************
* @brief provide high-level startup routines for NV32Fxx.
*
*******************************************************************************/

#include "start.h"
#include "common.h"
#include "wdog.h"
#include "sysinit.h"

void start(void);
void SystemInit( void );
/********************************************************************/
/*!
 * \brief   Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
void start(void)
{ 

	/* Disable the watchdog ETMer but enable update */
    WDOG_DisableWDOGEnableUpdate();    
    
#ifndef __GNUC__ 
#ifndef KEIL

	/* Copy any vector or data sections that need to be in RAM */
	common_startup();
#endif
#endif

	/* Jump to main process */
	main();

	/* No actions to perform after this so wait forever */
	while(1);
}

/********************************************************************/
/********************************************************************/
/*!
 * \brief   flash SystemInit
 * \return  None
 *
 * this is a system initialization function which dediu16Cated in Keil 
 * others complier don't use it.
 * it is similar to start function
 */
void SystemInit( void )
{
#if !defined(ENABLE_WDOG)   
	/* Disable the watchdog ETMer */
    WDOG_Disable();
#else
	/* Disable the watchdog ETMer but enable update */
    WDOG_DisableWDOGEnableUpdate();    
#endif
	
}



