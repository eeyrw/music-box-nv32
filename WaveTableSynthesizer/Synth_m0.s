.syntax unified
.section .text
.thumb_func
.global SynthAsm
.global GenDecayEnvlopeAsm
.global NoteOnAsm

@ typedef struct _SoundUnit
@ {
@ 	uint32_t wavetablePos;
@ 	uint32_t waveTableAddress;
@ 	uint32_t waveTableLen;
@ 	uint32_t waveTableLoopLen;
@ 	uint32_t waveTableAttackLen;
@ 	uint32_t envelopePos;
@ 	uint32_t increment;
@ 	int32_t val;
@ 	int32_t sampleVal;
@ 	uint32_t envelopeLevel;
@ }SoundUnit;

@ typedef struct _Synthesizer
@ {
@     SoundUnit SoundUnitList[POLY_NUM];
@ 	int32_t mixOut;
@     uint32_t lastSoundUnit;
@ }Synthesizer;

.equ pWavetablePos , 0
.equ pWaveTableAddress , 4
.equ pWaveTableLen , 8
.equ pWaveTableLoopLen , 12
.equ pWaveTableAttackLen , 16
.equ pEnvelopePos ,20
.equ pIncrement , 24
.equ pVal , 28
.equ pSampleVal , 32
.equ pEnvelopeLevel , 36
.equ SoundUnitSize,40


.equ ENVELOP_LEN,256
.equ POLY_NUM,32
.equ pMixOut,SoundUnitSize*POLY_NUM
.equ pLastSoundUnit,pMixOut+4

.equ WAVETABLE_CELESTA_C5_LEN,2608
.equ WAVETABLE_CELESTA_C5_ATTACK_LEN,1998
.equ WAVETABLE_CELESTA_C5_LOOP_LEN,610

.equ  WAVETABLE_CELESTA_C6_LEN, 1358
.equ  WAVETABLE_CELESTA_C6_ATTACK_LEN, 838
.equ  WAVETABLE_CELESTA_C6_LOOP_LEN, 520

.equ PWM_OUT1,0x40000438
.equ PWM_OUT2,0x4000043C

.func SynthAsm
SynthAsm:
push {r1-r2,r4-r7,lr}
pSoundUnit .req r1
loopIndex .req r4
mixOut .req r2

movs loopIndex,#POLY_NUM
movs mixOut,#0
movs pSoundUnit,r0

loopSynth:
    ldr r7,[pSoundUnit,#pEnvelopeLevel]
    cmp r7,#0
    beq loopSynthEnd
    ldr r5,[pSoundUnit,#pWaveTableAddress]
    ldr r6,[pSoundUnit,#pWavetablePos]
    lsrs r6,r6,#8 @wavetablePos /= 256
    lsls r6,r6,#1 @wavetablePos *= 2
    ldrsh r6,[r5,r6] @ Load signed 16bit sample to r6
    str r6,[pSoundUnit,#pSampleVal]
    muls r7,r6,r7 @sample*envelope/256
    asrs r7,r7,#8
    str r7,[pSoundUnit,#pVal]
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

subs loopIndex,loopIndex,#1 @ set n = n-1
adds pSoundUnit,pSoundUnit,#SoundUnitSize
bne loopSynth
movs pSoundUnit,r0

ldr r5,=#pMixOut
adds r5,r5,pSoundUnit
str mixOut,[r5]

@ mixOut /=1<<6, 2^(10-1)<= mixOut <=2^(10-1)-1
asrs mixOut,mixOut,#6
ldr r5,=#-512
cmp mixOut,r5
bge saturateEnd
movs mixOut,r5
ldr r5,=#511
cmp mixOut,r5
ble saturateEnd
movs mixOut,r5
saturateEnd:

@ mixOut: [-512,511] -> [0,1023]
ldr r5,=#512
adds mixOut,mixOut,r5
ldr r5,=#PWM_OUT1
strh mixOut,[r5]
ldr r5,=#PWM_OUT2
strh mixOut,[r5]
pop {r1-r2,r4-r7,pc}
.endfunc

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
subs loopIndexGenEnv,loopIndexGenEnv,#1 @ set n = n-1
adds pSoundUnitGenEnv,pSoundUnitGenEnv,#SoundUnitSize
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
lsls note,note,#1
ldr r5,=WaveTable_Celesta_C5_Increment
ldrh r5,[r5,note]
cpsid i                @ PRIMASK=1 Disable all interrupt except NMI ands Hardfault
str r5,[pSynth,#pIncrement]
movs r5,#0
str r5,[pSynth,#pWavetablePos]
ldr r5,=WaveTable_Celesta_C5
str r5,[pSynth,#pWaveTableAddress]
ldr r5,=#WAVETABLE_CELESTA_C5_LEN
str r5,[pSynth,#pWaveTableLen]
ldr r5,=#WAVETABLE_CELESTA_C5_LOOP_LEN
str r5,[pSynth,#pWaveTableLoopLen]
ldr r5,=#WAVETABLE_CELESTA_C5_ATTACK_LEN
str r5,[pSynth,#pWaveTableAttackLen]
ldr r5,=#0
str r5,[pSynth,#pEnvelopePos]
ldr r5,=#255
str r5,[pSynth,#pEnvelopeLevel]
cpsie i               @ PRIMASK=0 enable all interrupt
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