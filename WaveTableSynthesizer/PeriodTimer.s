ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 1


   1              	# 1 "WaveTableSynthesizer/PeriodTimer.S"
   1              	...
   0              	
   0              	
   0              	
   1              	#ifndef __ScoreDecoder_H__
   2              	#define __ScoreDecoder_H__
   3              	
   4              	#include "SynthCore.h"
   1              	#ifndef __SYNTH_CORE_H__
   2              	#define __SYNTH_CORE_H__
   3              	
   4              	
   5              	
   6              	#define pWavetablePos		0
   7              	#define pWaveTableAddress 	4
   8              	#define pWaveTableLen		8
   9              	#define pWaveTableLoopLen	12
  10              	#define pWaveTableAttackLen	16
  11              	#define pEnvelopePos		20
  12              	#define pIncrement			24
  13              	#define pVal				28
  14              	#define pSampleVal			32
  15              	#define pEnvelopeLevel		36
  16              	#define SoundUnitSize		40
  17              	
  18              	
  19              	#define ENVELOP_LEN			256
  20              	#define pMixOut 			(SoundUnitSize*POLY_NUM)
  21              	#define pLastSoundUnit		(pMixOut+4)
  22              	#define pMainVolume			(pLastSoundUnit+4)
  23              	#define pDecayGenTick		(pMainVolume+4)
  24              	#define SynthesizerSize 	(pDecayGenTick+4)
  25              	
  26              	
  27              	#define POLY_NUM 20
  28              	#define MAX_VOLUME_SHIFT_BIT 8
  29              	#define DECAY_TIME_FACTOR 120
  30              	
  31              	#ifndef __ASSEMBLER__
  32              	#include <stdint.h>
  33              	
  34              	#ifdef __cplusplus
  35              	extern "C" {
  36              	#endif
  37              	
  38              	
  39              	typedef struct _SoundUnit
  40              	{
  41              		uint32_t wavetablePos;
  42              		uint32_t waveTableAddress;
  43              		uint32_t waveTableLen;
  44              		uint32_t waveTableLoopLen;
  45              		uint32_t waveTableAttackLen;
  46              		uint32_t envelopePos;
  47              		uint32_t increment;
  48              		int32_t val;
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 2


  49              		int32_t sampleVal;
  50              		uint32_t envelopeLevel;
  51              	} SoundUnit;
  52              	
  53              	typedef enum _SYNTH_HW_STATUS
  54              	{
  55              		SYNTH_HW_OFF = 0,
  56              		SYNTH_HW_ON
  57              	}SYNTH_HW_STATUS;
  58              	
  59              	typedef struct _Synthesizer
  60              	{
  61              		SoundUnit SoundUnitList[POLY_NUM];
  62              		int32_t mixOut;
  63              		uint32_t lastSoundUnit;
  64              		uint32_t mainVolume;
  65              		uint32_t decayGenTick;
  66              		void (*hwSet)(SYNTH_HW_STATUS);
  67              	} Synthesizer;
  68              	
  69              	extern void SynthInit(Synthesizer *synth);
  70              	extern void SynthGenEnvelopeProcess(Synthesizer *synth);
  71              	extern void SynthOn(Synthesizer *synth);
  72              	extern void SynthOff(Synthesizer *synth);
  73              	void SynthRegisterHwChangeFunc(Synthesizer *synth, void (*hwSet)(SYNTH_HW_STATUS));
  74              	
  75              	#ifdef RUN_TEST
  76              		extern void NoteOnC(Synthesizer *synth, uint8_t note);
  77              	extern void SynthC(Synthesizer *synth);
  78              	extern void GenDecayEnvlopeC(Synthesizer *synth);
  79              	#endif
  80              	
  81              	extern void NoteOnAsm(Synthesizer *synth, uint8_t note);
  82              	extern void GenDecayEnvlopeAsm(Synthesizer *synth);
  83              	extern void SynthAsm(Synthesizer *synth);
  84              	extern Synthesizer* GlobalSynthPtr;
  85              	
  86              	#ifdef __cplusplus
  87              	} //end extern "C"
  88              	#endif
  89              	
  90              	#else
  91              	.extern EnvelopeTable
  92              	.extern GlobalSynthPtr
  93              	#endif
   5              	
   6              	#define pSynthesizer    0
   7              	#define pScoreDecoder   (pSynthesizer+SynthesizerSize)
   8              	#define pCurrentTick    0
   9              	#define pLastScoreTick  (pCurrentTick+4)
  10              	#define pStatus         (pLastScoreTick+4)
  11              	
  12              	#ifndef __ASSEMBLER__
  13              	
  14              	#include <stdint.h>
  15              	
  16              	
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 3


  17              	#ifdef __cplusplus
  18              	extern "C" {
  19              	#endif
  20              	
  21              	enum DECODER_STATUS
  22              	{
  23              	    STATUS_STOP = 0,
  24              	    STATUS_REDAY_TO_DECODE = 1,
  25              	    STATUS_DECODING = 2
  26              	};
  27              	
  28              	enum SCHEDULER_MODE
  29              	{
  30              	    MODE_ORDER_PLAY = 0,
  31              	};
  32              	
  33              	enum SCHEDULER_STATUS
  34              	{
  35              	    SCHEDULER_READY_TO_SWITCH = 0,
  36              	    SCHEDULER_SWITCHING,
  37              	    SCHEDULER_SCORE_PREV,
  38              	    SCHEDULER_SCORE_NEXT,
  39              	    SCHEDULER_STOP,
  40              	};
  41              	
  42              	typedef struct _ScoreListHeader
  43              	{
  44              	    char identifer[4];
  45              	    uint32_t scoreCount;
  46              	    uint32_t firstAddr;
  47              	}ScoreListHeader;
  48              	
  49              	typedef struct _ScoreDecoder
  50              	{
  51              	    uint32_t currentTick;
  52              	    uint32_t lastScoreTick;
  53              	    uint32_t status;
  54              	    uint8_t *scorePointer;
  55              	} ScoreDecoder;
  56              	
  57              	typedef struct _PlayScheduler
  58              	{
  59              	    uint32_t schedulerMode;
  60              	    int32_t currentScoreIndex;
  61              	    uint32_t maxScoreNum;
  62              	    ScoreListHeader* scoreListHeader;
  63              	    uint32_t status;
  64              	    uint32_t switchDirect;
  65              	} PlayScheduler;
  66              	
  67              	typedef struct _Player
  68              	{
  69              	    Synthesizer synthesizer;
  70              	    ScoreDecoder decoder;
  71              	    PlayScheduler scheduler;
  72              	}Player;
  73              	
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 4


  74              	
  75              	
  76              	extern ScoreListHeader ScoreDataList;
  77              	extern Player *GlobalPlayerPtr;
  78              	
  79              	extern void PlayerInit(Player *player);
  80              	extern void Player32kProc(Player *player);
  81              	extern void PlayerProcess(Player *player);
  82              	extern void ScoreDecodeProcess(Player *player);
  83              	extern void PlayScore(Player* player, uint8_t *score);
  84              	extern void StopDecode(Player *player);
  85              	extern void StartPlayScheduler(Player* player);
  86              	extern void StopPlayScheduler(Player *player);
  87              	extern void PlaySchedulerNextScore(Player *player);
  88              	extern void PlaySchedulerPreviousScore(Player *player);
  89              	extern void SchedulerSetIntialRandomSeed(Player *player,uint8_t randomSeed);
  90              	extern void UpdateTick(ScoreDecoder *decoder);
  91              	extern void UpdateNextScoreTick(ScoreDecoder *decoder);
  92              	
  93              	#ifdef __cplusplus
  94              	} //end extern "C"
  95              	#endif
  96              	
  97              	#else
  98              	.extern GlobalPlayerPtr;
  99              	#endif
   1              	#ifndef __ASM_COMMON_H__
   1              	#include "SynthCore.h"
   2              	#include "AsmCommon.h"
   3              	#include "RegDefNv32.h"
   1              	#ifndef __REG_DEF_NV32_H__
   4              	
   5              	.syntax unified
   6              	.section .text
   7              	SynthAsm:
   8 000c F6B4     	push {r1-r2,r4-r7}
   9              	pSoundUnit .req r1
  10              	loopIndex .req r4
  11              	mixOut .req r2
  12              	
  13 000e 1424     	movs loopIndex,#POLY_NUM
  14 0010 0022     	movs mixOut,#0
  15 0012 0100     	movs pSoundUnit,r0
  16              	
  17              	loopSynth:
  18 0014 4F6A     	    ldr r7,[pSoundUnit,#pEnvelopeLevel]
  19 0016 002F     	    cmp r7,#0
  20 0018 11D0     	    beq loopSynthEnd
  21 001a 4D68     	    ldr r5,[pSoundUnit,#pWaveTableAddress]
  22 001c 0E68     	    ldr r6,[pSoundUnit,#pWavetablePos]
  23 001e 360A     	    lsrs r6,r6,#8 @wavetablePos /= 256
  24 0020 7600     	    lsls r6,r6,#1 @wavetablePos *= 2
  25 0022 AE5F     	    ldrsh r6,[r5,r6] @ Load signed 16bit sample to r6
  26              	    @ str r6,[pSoundUnit,#pSampleVal]
  27 0024 7743     	    muls r7,r6,r7 @sample*envelope/256
  28              	    @ asrs r7,r7,#8
  29              	    @ str r7,[pSoundUnit,#pVal]
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 5


  30 0026 BA18     	    adds mixOut,r7,mixOut @mixOut+=sample*envelope/256
  31              	
  32 0028 0E68     	    ldr r6,[pSoundUnit,#pWavetablePos]
  33 002a 8D69     	    ldr r5,[pSoundUnit,#pIncrement]
  34 002c AE19     	    adds r6,r5,r6
  35 002e 8D68     	    ldr r5,[pSoundUnit,#pWaveTableLen]
  36 0030 2D02     	    lsls r5,r5,#8 @pWaveTableLen*=256    
  37 0032 B542     	    cmp r5,r6
  38 0034 02D8     	    bhi wavePosUpdateEnd  @bhi : HI	C = 1 ands Z = 0	Higher, unsigned
  39 0036 CD68     	    ldr r5,[pSoundUnit,#pWaveTableLoopLen]
  40 0038 2D02     	    lsls r5,r5,#8 @waveTableLoopLen*=256
  41 003a 761B     	    subs r6,r6,r5
  42              	    wavePosUpdateEnd:
  43 003c 0E60     	    str r6,[pSoundUnit,#pWavetablePos]
  44              	loopSynthEnd:
  45              	
  46 003e 2831     	adds pSoundUnit,pSoundUnit,#SoundUnitSize
  47 0040 013C     	subs loopIndex,loopIndex,#1 @ set n = n-1
  48 0042 E7D1     	bne loopSynth
  49 0044 0100     	movs pSoundUnit,r0
  50              	
  51 0046 144D     	ldr r5,=#pMixOut
  52 0048 6D18     	adds r5,r5,pSoundUnit
  53 004a 2A60     	str mixOut,[r5]
  54              	
  55              	@ 
  56 004c AD68     	ldr r5,[r5,#(pMainVolume-pMixOut)]
  57 004e 1212     	asrs mixOut,mixOut,#8
  58 0050 6A43     	muls mixOut,r5,mixOut
  59 0052 1214     	asrs mixOut,mixOut,#(MAX_VOLUME_SHIFT_BIT+8)
  60              	
  61 0054 114D     	ldr r5,=#-128
  62 0056 AA42     	cmp mixOut,r5
  63 0058 00DA     	bge lowerBoundSatisfied
  64 005a 2A00     	movs mixOut,r5
  65              	lowerBoundSatisfied:
  66 005c 104D     	ldr r5,=#127
  67 005e AA42     	cmp mixOut,r5
  68 0060 00DD     	ble saturateEnd
  69 0062 2A00     	movs mixOut,r5
  70              	saturateEnd:
  71              	
  72              	@ mixOut: [-512,511] -> [0,1023]
  73 0064 0F4D     	ldr r5,=#128
  74 0066 5219     	adds mixOut,mixOut,r5
  75 0068 0F4D     	ldr r5,=#PWM_OUT1
  76 006a 2A80     	strh mixOut,[r5]
  77 006c 0F4D     	ldr r5,=#PWM_OUT2
  78 006e 2A80     	strh mixOut,[r5]
  79 0070 F6BC     	pop {r1-r2,r4-r7}...
   1              	#include "SynthCore.h"
   2              	#include "Player.h"
   3              	#include "AsmCommon.h"
   4              	
   5              	.section .text 
   6              	
   7              	UpdateTick:
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 6


   8              		ptrSoundUnit .req r0
   9              		ptrScoreDecoder .req r1
  10 0072 074D     		ldr r5,=#pCurrentTick
  11 0074 0D44     		add r5,r5,ptrScoreDecoder
  12 0076 2E68     		ldr r6,[r5]
  13 0078 2E68     		ldr r6,[r5]
  14 007a 0136     		adds r6,r6,#1
  15 007c 2E60     		str r6,[r5]
  16              	
  17 007e 0C4D     		ldr r5,=#pDecayGenTick
  18 0080 0544     		add r5,r5,ptrSoundUnit
  19 0082 2E68     		ldr r6,[r5]
  20 0084 782E     		cmp r6,#DECAY_TIME_FACTOR
  21 0086 01D2     		bhs updateDecayGenTickEnd  //bhs Branch if Same or Higher (Unsigned)
  22 0088 0136     		adds r6,r6,#1
  23 008a 2E60     		str r6,[r5]
  24              		updateDecayGenTickEnd:...
ARM GAS  C:\Users\YRW\AppData\Local\Temp\ccxNKGtH.s 			page 7


DEFINED SYMBOLS
WaveTableSynthesizer/PeriodTimer.S:8      .text:00000000 PIT_Task
WaveTableSynthesizer/PeriodTimer.S:10     .text:00000000 $t
WaveTableSynthesizer/Synth.inc:7      .text:0000000c SynthAsm
WaveTableSynthesizer/Synth.inc:17     .text:00000014 loopSynth
WaveTableSynthesizer/Synth.inc:44     .text:0000003e loopSynthEnd
WaveTableSynthesizer/Synth.inc:42     .text:0000003c wavePosUpdateEnd
WaveTableSynthesizer/Synth.inc:65     .text:0000005c lowerBoundSatisfied
WaveTableSynthesizer/Synth.inc:70     .text:00000064 saturateEnd
WaveTableSynthesizer/UpdateTick.inc:7      .text:00000072 UpdateTick
WaveTableSynthesizer/UpdateTick.inc:24     .text:0000008c updateDecayGenTickEnd
WaveTableSynthesizer/PeriodTimer.S:19     .text:0000008c $d

UNDEFINED SYMBOLS
GlobalPlayerPtr
