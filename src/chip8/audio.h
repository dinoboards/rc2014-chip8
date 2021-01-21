#ifndef __AUDIO
#define __AUDIO

#include "datatypes.h"

extern void    audioInit();
extern uint8_t audioChipProbe();

typedef struct _audioCommand {
  uint8_t  channel; // 2 bits
  uint8_t  volume;  // 4 bits
  uint16_t period;  // 12 bits
} audioCommand;

extern uint16_t audioCmd(const audioCommand *) __z88dk_fastcall;

#endif
