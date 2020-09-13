
#ifndef __INSTR_RANDOM
#define __INSTR_RANDOM

extern void rnd();
extern void seedRandom();

#include "random.h"

#define rnd() (registers[nibble2nd] = ((byte)chip8Rand() & lowByte))

#endif
