
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

#define ldVxVy() (registers[secondNibble] = registers[thirdNibble])

#define _ldVxI(x)                                          \
  for (byte(i##x) = 0; (i##x) <= secondNibble; (i##x)++) { \
    registers[(i##x)] = ((byte *)registerI)[(i##x)];       \
  }

#define ldVxI() _ldVxI(__LINE__)

#define andVxVy() registers[secondNibble] &= registers[thirdNibble];

#define shrVxVy()                                \
  registers[0xF] = registers[thirdNibble] & 0x1; \
  registers[secondNibble] = registers[thirdNibble] >> 1;

#endif
