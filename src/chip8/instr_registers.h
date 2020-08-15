
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "timers.h"

#define ldVxByte()  (registers[secondNibble] = lowByte)
#define ldIAddr()   (registerI = addr)
#define addVxByte() (registers[secondNibble] += lowByte)
#define ldStVx()                            \
  {                                         \
    initTimers();                           \
    (soundTimer = registers[secondNibble]); \
  }

#endif
