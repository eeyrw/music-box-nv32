#include "Player.h"

#define PeriodTimerHandler PIT_Task

.syntax unified
.section .text
.thumb_func
.global PeriodTimerHandler
.func PeriodTimerHandler
PeriodTimerHandler:
push {r1-r3,r4-r7,lr}
ldr r5,=#GlobalPlayerPtr
ldr r5,[r5]
ldr r6,=#pSynthesizer
adds r0,r5,r6
ldr r6,=#pScoreDecoder
adds r1,r5,r6
#include "Synth.inc"
#include "UpdateTick.inc"
pop {r1-r3,r4-r7,pc}
.endfunc
