//
// SEGGER_SYSVIEW_TickCnt has to be defined in the module which
// handles the SysTick and must be incremented in the SysTick
// handler before any SYSVIEW event is generated.
//
// Example in embOS RTOSInit.c:
//
// unsigned int SEGGER_SYSVIEW_TickCnt; // <<-- Define SEGGER_SYSVIEW_TickCnt.
// void SysTick_Handler(void) {
// #if OS_PROFILE
// SEGGER_SYSVIEW_TickCnt++; // <<-- Increment SEGGER_SYSVIEW_TickCnt asap.
// #endif
// OS_EnterNestableInterrupt();
// OS_TICK_Handle();
// OS_LeaveNestableInterrupt();
// }
//

#include "Global.h"
unsigned int SEGGER_SYSVIEW_TickCnt;
/*********************************************************************
*
* Defines, fixed
*
**********************************************************************
*/
#define SCB_ICSR (*(volatile U32*) (0xE000ED04uL)) // Interrupt Control State Register
#define SCB_ICSR_PENDSTSET_MASK (1UL << 26) // SysTick pending bit
#define SYST_RVR (*(volatile U32*) (0xE000E014uL)) // SysTick Reload Value Register
#define SYST_CVR (*(volatile U32*) (0xE000E018uL)) // SysTick Current Value Register
/*********************************************************************
*
* SEGGER_SYSVIEW_X_GetTimestamp()
*
* Function description
* Returns the current timestamp in ticks using the system tick
* count and the SysTick counter.
* All parameters of the SysTick have to be known and are set via
* configuration defines on top of the file.
*
* Return value
* The current timestamp.
*
** Additional information
* SEGGER_SYSVIEW_X_GetTimestamp is always called when interrupts are
* disabled. Therefore locking here is not required.
*/
U32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
U32 TickCount;
U32 Cycles;
U32 CyclesPerTick;
//
// Get the cycles of the current system tick.
// SysTick is down-counting, subtract the current value from the number of cycles per tick.
//
CyclesPerTick = SYST_RVR + 1;
Cycles = (CyclesPerTick - SYST_CVR);
//
// Get the system tick count.
//
TickCount = SEGGER_SYSVIEW_TickCnt;
//
// If a SysTick interrupt is pending, re-read timer and adjust result
//
if ((SCB_ICSR & SCB_ICSR_PENDSTSET_MASK) != 0) {
Cycles = (CyclesPerTick - SYST_CVR);
TickCount++;
}
Cycles += TickCount * CyclesPerTick;
return Cycles;
}
