#include "SynthCore.h"
#include "AsmCommon.h"
#include "RegDefNv32.h"

.syntax unified
.section .text
SynthAsm:
pSoundUnit .req r3
loopIndex .req r4
mixOut .req r2

movs loopIndex,#POLY_NUM
movs mixOut,#0
movs pSoundUnit,r0

loopSynth:
    ldrh r7,[pSoundUnit,#pEnvelopeLevel]
    cmp r7,#0
    beq loopSynthEnd
    ldr r5,[pSoundUnit,#pWaveTableAddress]
    ldr r6,[pSoundUnit,#pWavetablePos]
    lsrs r6,r6,#8 @wavetablePos /= 256
    lsls r6,r6,#1 @wavetablePos *= 2
    ldrsh r6,[r5,r6] @ Load signed 16bit sample to r6
    @ strsh r6,[pSoundUnit,#pSampleVal]
    muls r7,r6,r7 @sample*envelope/256
    @ asrs r7,r7,#8
    @ strsh r7,[pSoundUnit,#pVal]
    adds mixOut,r7,mixOut @mixOut+=sample*envelope/256

    ldr r6,[pSoundUnit,#pWavetablePos]
    ldr r5,[pSoundUnit,#pIncrement]
    adds r6,r5,r6
    ldr r5,[pSoundUnit,#pWaveTableLen]
    lsls r5,r5,#8 @pWaveTableLen*=256    
    cmp r5,r6
    bhi wavePosUpdateEnd  @bhi : HI	C = 1 ands Z = 0	Higher, unsigned
    ldr r5,[pSoundUnit,#pWaveTableLoopLen]
    lsls r5,r5,#8 @waveTableLoopLen*=256
    subs r6,r6,r5
    wavePosUpdateEnd:
    str r6,[pSoundUnit,#pWavetablePos]
loopSynthEnd:

adds pSoundUnit,pSoundUnit,#SoundUnitSize
subs loopIndex,loopIndex,#1 @ set n = n-1
bne loopSynth
movs pSoundUnit,r0

ldr r5,=#pMixOut
adds r5,r5,pSoundUnit
str mixOut,[r5]

@ 
ldrh r5,[r5,#(pMainVolume-pMixOut)]
asrs mixOut,mixOut,#8
muls mixOut,r5,mixOut
asrs mixOut,mixOut,#(MAX_VOLUME_SHIFT_BIT+8)

ldr r5,=#-128
cmp mixOut,r5
bge lowerBoundSatisfied
movs mixOut,r5
lowerBoundSatisfied:
ldr r5,=#127
cmp mixOut,r5
ble saturateEnd
movs mixOut,r5
saturateEnd:

@ mixOut: [-512,511] -> [0,1023]
ldr r5,=#128
adds mixOut,mixOut,r5
ldr r5,=#PWM_OUT1
strh mixOut,[r5]
ldr r5,=#PWM_OUT2
strh mixOut,[r5]
