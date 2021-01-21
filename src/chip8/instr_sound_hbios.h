
#ifndef __INSTR_SOUND_HBIOS
#define __INSTR_SOUND_HBIOS

#include "hbios.h"

extern void hbios_ldStVx();
#define hbios_soundOff() hbSndReset(0)

#endif
