#ifndef __SYNTH_CORE_H__
#define __SYNTH_CORE_H__



#define pWavetablePos		0
#define pWaveTableAddress 	(pWavetablePos+4)
#define pWaveTableLen		(pWaveTableAddress+4)
#define pWaveTableLoopLen	(pWaveTableLen+4)
#define pWaveTableAttackLen	 (pWaveTableLoopLen+4)
#define pIncrement			(pWaveTableAttackLen+4)
#define pEnvelopePos		(pIncrement+4)
#define pEnvelopeLevel		(pEnvelopePos+2)

#ifdef RUN_TEST

#define pVal				(pEnvelopeLevel+2)
#define pSampleVal			(pVal+2)
#define SoundUnitSize		(pSampleVal+2)

#else

#define SoundUnitSize		(pEnvelopeLevel+2)

#endif


#define ENVELOP_LEN			256
#define pMixOut 			(SoundUnitSize*POLY_NUM)
#define pLastSoundUnit		(pMixOut+4)
#define pMainVolume			(pLastSoundUnit+2)
#define pDecayGenTick		(pMainVolume+2)
#define pFuncHwSet			(pDecayGenTick+4)
#define SynthesizerSize 	(pFuncHwSet+4)


#define POLY_NUM 24
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
	uint32_t increment;
	uint16_t envelopePos;
	uint16_t envelopeLevel;
#ifdef RUN_TEST
	int16_t val;
	int16_t sampleVal;
#endif
}  __attribute__((packed)) SoundUnit;

typedef enum _SYNTH_HW_STATUS
{
	SYNTH_HW_OFF = 0,
	SYNTH_HW_ON
}SYNTH_HW_STATUS;

typedef struct _Synthesizer
{
	SoundUnit SoundUnitList[POLY_NUM];
	int32_t mixOut;
	uint16_t lastSoundUnit;
	uint16_t mainVolume;
	uint32_t decayGenTick;
	void (*hwSet)(SYNTH_HW_STATUS);
}  __attribute__((packed)) Synthesizer;

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
