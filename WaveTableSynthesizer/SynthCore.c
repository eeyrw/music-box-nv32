#include "SynthCore.h"
#include <stdint.h>
#include <stdio.h>
#include "WaveTable_Celesta_C5.h"
#include "WaveTable_Celesta_C6.h"
#include "EnvelopeTable.h"


void SynthInit(Synthesizer* synth)
{
	SoundUnit* soundUnits = synth->SoundUnitList;
	for (uint8_t i = 0; i < POLY_NUM; i++)
	{
		soundUnits[i].increment = 0;
		soundUnits[i].wavetablePos = 0;
		soundUnits[i].envelopeLevel = 0;
		soundUnits[i].envelopePos = 0;
        soundUnits[i].val = 0;
		soundUnits[i].waveTableAddress = (uint32_t)WaveTable_Celesta_C5;
		soundUnits[i].waveTableLen = WAVETABLE_CELESTA_C5_LEN;
		soundUnits[i].waveTableLoopLen = WAVETABLE_CELESTA_C5_LOOP_LEN;
		soundUnits[i].waveTableAttackLen = WAVETABLE_CELESTA_C5_ATTACK_LEN;
	}
    synth->lastSoundUnit=0;
}
//#ifdef RUN_TEST
void NoteOnC(Synthesizer* synth,uint8_t note)
{
	uint32_t lastSoundUnit = synth->lastSoundUnit;
	SoundUnit* soundUnits = synth->SoundUnitList;

	//disable_interrupts();
	soundUnits[lastSoundUnit].increment = WaveTable_Celesta_C5_Increment[note&0x7F];
	soundUnits[lastSoundUnit].wavetablePos = 0;
	soundUnits[lastSoundUnit].waveTableAddress = (uint32_t)WaveTable_Celesta_C5;
	soundUnits[lastSoundUnit].waveTableLen = WAVETABLE_CELESTA_C5_LEN;
	soundUnits[lastSoundUnit].waveTableLoopLen = WAVETABLE_CELESTA_C5_LOOP_LEN;
	soundUnits[lastSoundUnit].waveTableAttackLen = WAVETABLE_CELESTA_C5_ATTACK_LEN;
	//enable_interrupts();

	lastSoundUnit++;
	if (lastSoundUnit== POLY_NUM)
		lastSoundUnit = 0;

    synth->lastSoundUnit=lastSoundUnit;
}

void SynthC(Synthesizer* synth)
{
    synth->mixOut=0;
	int16_t* pWaveTable;
	uint32_t waveTablePosInt;
	SoundUnit* soundUnits = synth->SoundUnitList;

    for(uint32_t i=0;i<POLY_NUM;i++)
    {
		pWaveTable=(int16_t*)soundUnits[i].waveTableAddress;
		waveTablePosInt= (soundUnits[i].wavetablePos)>>8;
        soundUnits[i].val=((int32_t)soundUnits[i].envelopeLevel)*pWaveTable[waveTablePosInt]/256;
        soundUnits[i].sampleVal=pWaveTable[waveTablePosInt];
		uint32_t waveTablePos=soundUnits[i].increment+
                             soundUnits[i].wavetablePos;

        waveTablePosInt= waveTablePos>>8;
        if(waveTablePosInt>=soundUnits[i].waveTableLen)
           waveTablePosInt-=soundUnits[i].waveTableLoopLen;
        soundUnits[i].wavetablePos= waveTablePosInt<<8|(0xFF&waveTablePos);
        synth->mixOut+=soundUnits[i].val;
    }
}

void GenDecayEnvlopeC(Synthesizer* synth)
{
    SoundUnit* soundUnits = synth->SoundUnitList;
	for (uint32_t i = 0; i < POLY_NUM; i++)
	{
		if((soundUnits[i].wavetablePos>>8) >=soundUnits[i].waveTableAttackLen &&
				soundUnits[i].envelopePos <(sizeof(EnvelopeTable)-1))
		{
			soundUnits[i].envelopeLevel = EnvelopeTable[soundUnits[i].envelopePos];
			soundUnits[i].envelopePos += 1;
		}
	}
}
//#endif