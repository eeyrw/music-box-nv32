#ifndef __WAVETABLE_CELESTA_C5__
#define __WAVETABLE_CELESTA_C5__

// Sample's base frequency: 523.629906 Hz
// Sample's sample rate: 32000 Hz
#define WAVETABLE_CELESTA_C5_LEN 2608
#define WAVETABLE_CELESTA_C5_ATTACK_LEN 1998
#define WAVETABLE_CELESTA_C5_LOOP_LEN 610

#ifndef __ASSEMBLER__
#include <stdint.h>


extern const int16_t WaveTable_Celesta_C5[WAVETABLE_CELESTA_C5_LEN];
extern const uint16_t WaveTable_Celesta_C5_Increment[];
#else
.extern	WaveTable_Celesta_C5
.extern WaveTable_Celesta_C5_Increment
#endif
#endif