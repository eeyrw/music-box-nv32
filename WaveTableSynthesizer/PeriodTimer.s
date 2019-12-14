#include "Player.h"
#include "RegDefNv32.h"

#define PeriodTimerHandler PIT_Ch0Isr_Override

.syntax unified
.section .text
.thumb_func
.global PeriodTimerHandler
.func PeriodTimerHandler
PeriodTimerHandler:
push {r4-r7,lr} // r0-r3,r12 saved by hardware when expection happening
ldr r0,=#PIT_TFLG0
ldr r1,[r0]
ldr r2,=#PIT_TFLG_TIF_BIT
orrs r1,r1,r2
str r1,[r0]

ldr r5,=#GlobalPlayerPtr
ldr r5,[r5]
ldr r6,=#pSynthesizer
adds r0,r5,r6
ldr r6,=#pScoreDecoder
adds r1,r5,r6
#include "Synth.inc"
#include "UpdateTick.inc"
pop {r4-r7,pc}
.endfunc
