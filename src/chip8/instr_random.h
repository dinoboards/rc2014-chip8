
#ifndef __INSTR_RANDOM
#define __INSTR_RANDOM

extern void rnd();
extern void seedRandom();

#include "random.h"

#define rnd() (registers[secondNibble] = ((byte)chip8Rand() & lowByte))

#endif
