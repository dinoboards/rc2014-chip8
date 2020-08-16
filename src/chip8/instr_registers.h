
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

#define ldDtVx()                            \
  {                                         \
    initTimers();                           \
    (delayTimer = registers[secondNibble]); \
  }

#define ldVxDt() (registers[secondNibble] = delayTimer)

#define addIVx() (registerI += registers[secondNibble])

#endif
