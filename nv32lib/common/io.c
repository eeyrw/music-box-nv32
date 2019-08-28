/******************************************************************************
*
* @brief provide serial Input/Output routines. 
*
*******************************************************************************/
#include "common.h"
#include "uart.h"
#include "stdio.h"

//********************************************************************/
char
in_char (void)
{
	return UART_GetChar(TERM_PORT);
}
/********************************************************************/
void
out_char (char ch)
{
	UART_PutChar(TERM_PORT, ch);
}
/********************************************************************/
int
char_present (void)
{
	return UART_CharPresent(TERM_PORT);
}

#ifdef KEIL
/********************************************************************/
int fputc(int ch, FILE *f)
{
	out_char((char)ch);
	return 1;
}
#endif


/********************************************************************/
