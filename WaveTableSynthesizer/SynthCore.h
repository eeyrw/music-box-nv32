#ifndef __SYNTH_CORE_H__
#define __SYNTH_CORE_H__



#define pWavetablePos		0
#define pWaveTableAddress 	4
#define pWaveTableLen		8
#define pWaveTableLoopLen	12
#define pWaveTableAttackLen	16
#define pEnvelopePos		20
#define pIncrement			24
#define pVal				28
#define pSampleVal			32
#define pEnvelopeLevel		36
#define SoundUnitSize		40


#define ENVELOP_LEN			256
#define pMixOut 			(SoundUnitSize*POLY_NUM)
#define pLastSoundUnit		(pMixOut+4)
#define pMainVolume			(pLastSoundUnit+4)
#define pDecayGenTick		(pMainVolume+4)
#define pFuncHwSet			(pDecayGenTick+4)
#define SynthesizerSize 	(pFuncHwSet+4)


#define POLY_NUM 5
#define MAX_VOLUME_SHIFT_BIT 8
#define DECAY_TIME_FACTOR 230

#ifndef __ASSEMBLER__
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SoundUnit
{
	uint32_t wavetablePos;
	uint32_t waveTableAddress;
	uint32_t waveTableLen;
	uint32_t waveTableLoopLen;
	uint32_t waveTableAttackLen;
	uint32_t envelopePos;
	uint32_t increment;
	int32_t val;
	int32_t sampleVal;
	uint32_t envelopeLevel;
} SoundUnit;

typedef enum _SYNTH_HW_STATUS
{
	SYNTH_HW_OFF = 0,
	SYNTH_HW_ON
}SYNTH_HW_STATUS;

typedef struct _Synthesizer
{
	SoundUnit SoundUnitList[POLY_NUM];
	int32_t mixOut;
	uint32_t lastSoundUnit;
	uint32_t mainVolume;
	uint32_t decayGenTick;
	void (*hwSet)(SYNTH_HW_STATUS);
} Synthesizer;

extern void SynthInit(Synthesizer *synth);
extern void SynthGenEnvelopeProcess(Synthesizer *synth);
extern void SynthOn(Synthesizer *synth);
extern void SynthOff(Synthesizer *synth);
void SynthRegisterHwChangeFunc(Synthesizer *synth, void (*hwSet)(SYNTH_HW_STATUS));

#ifdef RUN_TEST
	extern void NoteOnC(Synthesizer *synth, uint8_t note);
extern void SynthC(Synthesizer *synth);
extern void GenDecayEnvlopeC(Synthesizer *synth);
#endif

extern void NoteOnAsm(Synthesizer *synth, uint8_t note);
extern void GenDecayEnvlopeAsm(Synthesizer *synth);
extern void SynthAsm(Synthesizer *synth);
extern Synthesizer* GlobalSynthPtr;

#ifdef __cplusplus
} //end extern "C"
#endif

#else
.extern EnvelopeTable
.extern GlobalSynthPtr
#endif

#endif
