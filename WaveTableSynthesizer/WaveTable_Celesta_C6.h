#ifndef __WAVETABLE_CELESTA_C6__
#define __WAVETABLE_CELESTA_C6__

// Sample's base frequency: 1046.832025 Hz
// Sample's sample rate: 32000 Hz
#define WAVETABLE_CELESTA_C6_LEN 1358
#define WAVETABLE_CELESTA_C6_ATTACK_LEN 838
#define WAVETABLE_CELESTA_C6_LOOP_LEN 520

#ifndef __ASSEMBLER__
#include <stdint.h>

extern const int16_t WaveTable_Celesta_C6[WAVETABLE_CELESTA_C6_LEN];
extern const uint16_t WaveTable_Celesta_C6_Increment[];
#else
.extern	WaveTable_Celesta_C6
.extern WaveTable_Celesta_C6_Increment
#endif
#endif