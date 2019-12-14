#include "SynthCore.h"
#include <stdint.h>
#include <stdio.h>
#include "WaveTable_Celesta_C5.h"
#include "WaveTable_Celesta_C6.h"
#include "EnvelopeTable.h"

#ifdef RUN_TEST
Synthesizer synthForC;
#endif

Synthesizer *GlobalSynthPtr;

void SynthInit(Synthesizer *synth)
{
	SoundUnit *soundUnits = synth->SoundUnitList;
	for (uint8_t i = 0; i < POLY_NUM; i++)
	{
		soundUnits[i].increment = 0;
		soundUnits[i].wavetablePos = 0;
		soundUnits[i].envelopeLevel = 0;
		soundUnits[i].envelopePos = 0;
		soundUnits[i].waveTableAddress = (uint32_t)WaveTable_Celesta_C5;
		soundUnits[i].waveTableLen = WAVETABLE_CELESTA_C5_LEN;
		soundUnits[i].waveTableLoopLen = WAVETABLE_CELESTA_C5_LOOP_LEN;
		soundUnits[i].waveTableAttackLen = WAVETABLE_CELESTA_C5_ATTACK_LEN;
	}
	synth->mixOut = 0;
	synth->lastSoundUnit = 0;
	synth->mainVolume = 1 << (MAX_VOLUME_SHIFT_BIT - 1);
	synth->decayGenTick = 0;
	GlobalSynthPtr = synth;
}

void SynthRegisterHwChangeFunc(Synthesizer *synth, void (*hwSet)(SYNTH_HW_STATUS))
{
	synth->hwSet = hwSet;
}

void SynthOn(Synthesizer *synth)
{
	synth->hwSet(SYNTH_HW_ON);
	SynthInit(synth);
}

void SynthOff(Synthesizer *synth)
{
	synth->hwSet(SYNTH_HW_OFF);
	SynthInit(synth);
}

void SynthGenEnvelopeProcess(Synthesizer *synth)
{
	if (synth->decayGenTick >= DECAY_TIME_FACTOR)
	{
		GenDecayEnvlopeAsm(synth);
		synth->decayGenTick = 0;
	}
}
#ifdef RUN_TEST
void NoteOnC(Synthesizer *synth, uint8_t note)
{
	uint32_t lastSoundUnit = synth->lastSoundUnit;
	SoundUnit *soundUnits = synth->SoundUnitList;

	//disable_interrupts();
	soundUnits[lastSoundUnit].increment = WaveTable_Celesta_C5_Increment[note & 0x7F];
	soundUnits[lastSoundUnit].wavetablePos = 0;
	soundUnits[lastSoundUnit].waveTableAddress = (uint32_t)WaveTable_Celesta_C5;
	soundUnits[lastSoundUnit].waveTableLen = WAVETABLE_CELESTA_C5_LEN;
	soundUnits[lastSoundUnit].waveTableLoopLen = WAVETABLE_CELESTA_C5_LOOP_LEN;
	soundUnits[lastSoundUnit].waveTableAttackLen = WAVETABLE_CELESTA_C5_ATTACK_LEN;
	soundUnits[lastSoundUnit].envelopeLevel = 255;
	//enable_interrupts();

	lastSoundUnit++;
	if (lastSoundUnit == POLY_NUM)
		lastSoundUnit = 0;

	synth->lastSoundUnit = lastSoundUnit;
}

void SynthC(Synthesizer *synth)
{
	synth->mixOut = 0;
	int16_t *pWaveTable;
	uint32_t waveTablePosInt;
	SoundUnit *soundUnits = synth->SoundUnitList;

	for (uint32_t i = 0; i < POLY_NUM; i++)
	{
		if (soundUnits[i].envelopeLevel != 0)
		{
			pWaveTable = (int16_t *)soundUnits[i].waveTableAddress;
			waveTablePosInt = (soundUnits[i].wavetablePos) >> 8;
			soundUnits[i].val = ((int32_t)soundUnits[i].envelopeLevel) * pWaveTable[waveTablePosInt] >> 8;
			soundUnits[i].sampleVal = pWaveTable[waveTablePosInt];
			uint32_t waveTablePos = soundUnits[i].increment +
									soundUnits[i].wavetablePos;

			if (waveTablePos >= soundUnits[i].waveTableLen << 8)
				waveTablePos -= soundUnits[i].waveTableLoopLen << 8;
			soundUnits[i].wavetablePos = waveTablePos;
			synth->mixOut += soundUnits[i].val;
		}
	}
}

void GenDecayEnvlopeC(Synthesizer *synth)
{
	SoundUnit *soundUnits = synth->SoundUnitList;
	for (uint32_t i = 0; i < POLY_NUM; i++)
	{
		if ((soundUnits[i].wavetablePos >> 8) >= soundUnits[i].waveTableAttackLen &&
			soundUnits[i].envelopePos < (sizeof(EnvelopeTable) - 1))
		{
			soundUnits[i].envelopeLevel = EnvelopeTable[soundUnits[i].envelopePos];
			soundUnits[i].envelopePos += 1;
		}
	}
}
#endif
