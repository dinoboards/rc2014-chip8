#ifndef __AUDIO
#define __AUDIO

#include "datatypes.h"

extern void audioInit(void);
extern void audioPlay(uint16_t p) __z88dk_fastcall;
extern void audioStop(void);

#endif
