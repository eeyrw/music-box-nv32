#include "SynthCore.h"
#include "Player.h"
#include "AsmCommon.h"

.section .text 

// if (player->decoder.status == STATUS_DECODING)
//     player->decoder.currentTick++;
// if (player->synthesizer.decayGenTick < 200)
//     player->synthesizer.decayGenTick += 1;

UpdateTick:
	ptrSoundUnit .req r0
	ptrScoreDecoder .req r1
	ldr r5,=#pStatus
	add r5,r5,ptrScoreDecoder
	cmp r5,#constSTATUS_DECODING
	bne currentTickUpdateEnd
	ldr r5,=#pCurrentTick
	add r5,r5,ptrScoreDecoder
	ldr r6,[r5]
	ldr r6,[r5]
	adds r6,r6,#1
	str r6,[r5]
currentTickUpdateEnd:

	ldr r5,=#pDecayGenTick
	add r5,r5,ptrSoundUnit
	ldr r6,[r5]
	cmp r6,#DECAY_TIME_FACTOR
	bhs updateDecayGenTickEnd  //bhs Branch if Same or Higher (Unsigned)
	adds r6,r6,#1
	str r6,[r5]
	updateDecayGenTickEnd: