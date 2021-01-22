
#ifndef __INSTR_SOUND_HBIOS
#define __INSTR_SOUND_HBIOS

#include "datatypes.h"

extern void hbiosAudioPlay(uint16_t period) __z88dk_fastcall;
#define hbiosAudioStop() hbSndReset(0)

#endif
