#include "Player.h"

#define PeriodTimerHandler PIT_Task

.section .text
.global PeriodTimerHandler
PeriodTimerHandler:
ldr r5,=#GlobalPlayerPtr
ldr r5,[r5]
ldr r6,=pSynthesizer
add r0,r5,r6
ldr r6,=pScoreDecoder
add r1,r5,r6
  #include "Synth.inc"
  #include "UpdateTick.inc"
