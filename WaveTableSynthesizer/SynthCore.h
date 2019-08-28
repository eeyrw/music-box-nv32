#ifndef __SYNTH_CORE_H__
#define __SYNTH_CORE_H__

#include <stdint.h>

#define POLY_NUM 32

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
}SoundUnit;



typedef struct _Synthesizer
{
    SoundUnit SoundUnitList[POLY_NUM];
	int32_t mixOut;
    uint32_t lastSoundUnit;
}Synthesizer;


extern void SynthInit(Synthesizer* synth);

//#ifdef RUN_TEST
extern void NoteOnC(Synthesizer* synth,uint8_t note);
extern void SynthC(Synthesizer* synth);
extern void GenDecayEnvlopeC(Synthesizer* synth);
//#endif

extern void NoteOnAsm(Synthesizer* synth,uint8_t note);
extern void GenDecayEnvlopeAsm(Synthesizer* synth);
extern void SynthAsm(Synthesizer* synth);


#endif