#ifndef __INSTR_SOUND
#define __INSTR_SOUND

#include "datatypes.h"

#define AUDIO_SAMPLE_SIZE (16 * 8)

extern void ldStVx();
extern void audio();

#define soundOff() audioStop()

#endif
