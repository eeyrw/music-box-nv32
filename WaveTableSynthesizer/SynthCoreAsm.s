#include "SynthCore.h"
#include "WaveTable_Celesta_C5.h"
#include "WaveTable_Celesta_C6.h"
#include "AsmCommon.h"

.syntax unified
.section .text
.thumb_func
.global SynthAsm
.global GenDecayEnvlopeAsm
.global NoteOnAsm

.func GenDecayEnvlopeAsm
GenDecayEnvlopeAsm:
pSoundUnitGenEnv .req r0
loopIndexGenEnv .req r4
push {r1-r2,r4-r7,lr}
movs loopIndexGenEnv,#POLY_NUM
loopGenDecayEnvlope:
@ void GenDecayEnvlopeC(Synthesizer* synth)
@ {
@     SoundUnit* soundUnits = synth->SoundUnitList;
@ 	for (uint32_t i = 0; i < POLY_NUM; i++)
@ 	{
@ 		if((soundUnits[i].wavetablePos>>8) >=soundUnits[i].waveTableAttackLen &&
@ 				soundUnits[i].envelopePos <sizeof(EnvelopeTable)-1)
@ 		{
@ 			soundUnits[i].envelopeLevel = EnvelopeTable[soundUnits[i].envelopePos];
@ 			soundUnits[i].envelopePos += 1;
@ 		}
@ 	}
@ }
    ldr r5,[pSoundUnitGenEnv,#pWavetablePos]
    ldr r6,[pSoundUnitGenEnv,#pWaveTableAttackLen]
    lsls r6,r6,#8 @WaveTableAttackLen*=WaveTableAttackLen*256    
    cmp r5,r6
    blo conditionEnd @ blo Lower (unsigned < )
    ldr r5,[pSoundUnitGenEnv,#pEnvelopePos]
    ldr r6,=#(ENVELOP_LEN-1)
    cmp r5,r6
    bhs conditionEnd @ bhs Higher or same (unsigned >= )
    ldr r6,=EnvelopeTable
    ldrb r6,[r6,r5]  @ Load envelope to r6
    str r6,[pSoundUnitGenEnv,#pEnvelopeLevel]
    adds r5,r5,#1
    str r5,[pSoundUnitGenEnv,#pEnvelopePos]
    conditionEnd:
adds pSoundUnitGenEnv,pSoundUnitGenEnv,#SoundUnitSize    
subs loopIndexGenEnv,loopIndexGenEnv,#1 @ set n = n-1
bne loopGenDecayEnvlope
pop {r1-r2,r4-r7,pc}
.endfunc

.func NoteOnAsm
NoteOnAsm:
pSynth .req r2
note .req r1
push {r1-r2,r4-r7,lr}
movs pSynth,r0
@ void NoteOnC(Synthesizer* synth,uint8_t note)
@ {
@ 	uint8_t lastSoundUnit = synth->lastSoundUnit;
@ 	SoundUnit* soundUnits = synth->SoundUnitList;

@ 	//disable_interrupts();
@ 	soundUnits[lastSoundUnit].increment = WaveTable_Celesta_C5_Increment[note&0x7F];
@ 	soundUnits[lastSoundUnit].wavetablePos = 0;
@ 	soundUnits[lastSoundUnit].waveTableAddress = (uint32_t)WaveTable_Celesta_C5;
@ 	soundUnits[lastSoundUnit].waveTableLen = WAVETABLE_CELESTA_C5_LEN;
@ 	soundUnits[lastSoundUnit].waveTableLoopLen = WAVETABLE_CELESTA_C5_LOOP_LEN;
@ 	soundUnits[lastSoundUnit].waveTableAttackLen = WAVETABLE_CELESTA_C5_ATTACK_LEN;
@ 	//enable_interrupts();

@ 	lastSoundUnit++;
@ 	if (lastSoundUnit== POLY_NUM)
@ 		lastSoundUnit = 0;

@     synth->lastSoundUnit=lastSoundUnit;
@ }

ldr r5,=#pLastSoundUnit
adds r5,r5,pSynth
ldr r5,[r5]

movs r6,#SoundUnitSize
muls r5,r5,r6
adds pSynth,pSynth,r5
movs r5,#0x7F
ands note,note,r5


@cpsid i                @ PRIMASK=1 Disable all interrupt except NMI ands Hardfault
movs r7,#1
MSR PRIMASK,r7
movs r6,#80
cmp note,r6
bhi c6_branch
lsls note,note,#1
ldr r5,=WaveTable_Celesta_C5_Increment
ldrh r5,[r5,note]
str r5,[pSynth,#pIncrement]
ldr r5,=WaveTable_Celesta_C5
str r5,[pSynth,#pWaveTableAddress]
ldr r5,=#WAVETABLE_CELESTA_C5_LEN
str r5,[pSynth,#pWaveTableLen]
ldr r5,=#WAVETABLE_CELESTA_C5_LOOP_LEN
str r5,[pSynth,#pWaveTableLoopLen]
ldr r5,=#WAVETABLE_CELESTA_C5_ATTACK_LEN
str r5,[pSynth,#pWaveTableAttackLen]
b c5_c6_branch_end
c6_branch:
lsls note,note,#1
ldr r5,=WaveTable_Celesta_C6_Increment
ldrh r5,[r5,note]
str r5,[pSynth,#pIncrement]
ldr r5,=WaveTable_Celesta_C6
str r5,[pSynth,#pWaveTableAddress]
ldr r5,=#WAVETABLE_CELESTA_C6_LEN
str r5,[pSynth,#pWaveTableLen]
ldr r5,=#WAVETABLE_CELESTA_C6_LOOP_LEN
str r5,[pSynth,#pWaveTableLoopLen]
ldr r5,=#WAVETABLE_CELESTA_C6_ATTACK_LEN
str r5,[pSynth,#pWaveTableAttackLen]
c5_c6_branch_end:

ldr r5,=#0
str r5,[pSynth,#pEnvelopePos]
str r5,[pSynth,#pWavetablePos]
ldr r5,=#255
str r5,[pSynth,#pEnvelopeLevel]
@ cpsie i               @ PRIMASK=0 enable all interrupt
movs r7,#0
MSR PRIMASK,r7
movs pSynth,r0

ldr r5,=#pLastSoundUnit
adds r5,r5,pSynth
ldr r5,[r5]

adds r5,r5,#1
cmp r5,#POLY_NUM
bne updateLastSoundUnitEnd
movs r5,#0
updateLastSoundUnitEnd:

ldr r6,=#pLastSoundUnit
adds r6,r6,pSynth
str r5,[r6]

pop {r1-r2,r4-r7,pc}
.endfunc