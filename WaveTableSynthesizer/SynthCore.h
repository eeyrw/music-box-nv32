#ifndef __SYNTH_CORE_H__
#define __SYNTH_CORE_H__

#include <stdint.h>

#define POLY_NUM 20
#define MAX_VOLUME_SHIFT_BIT 8

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

#endif