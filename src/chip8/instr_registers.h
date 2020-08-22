
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

inline void ldVxI() {
  // clang-format off
  __asm

    LD	HL, _registerI
    LD	A, (HL)
    INC	HL
    LD	H, (HL)
    LD	L, A
    LD	DE, _registers
    LD  A, (_secondNibble)
    LD  C, A
    INC C
    LD  B, 0
    LDIR

  	EX	DE, HL
    LD	HL, _registerI
    LD	(HL), E
    INC	HL
    LD	(HL), D

  __endasm;
  // clang-format on
}

#define andVxVy() registers[secondNibble] &= registers[thirdNibble];

#define shrVxVy()                                 \
  registers[0xF] = registers[secondNibble] & 0x1; \
  registers[secondNibble] = registers[secondNibble] >> 1;

#define _addVxVy(x)                                  \
  uint16_t i##x = registers[secondNibble];           \
  i##x += registers[thirdNibble];                    \
  registers[secondNibble] += registers[thirdNibble]; \
  registers[0xF] = i##x > 255;

#define addVxVy() _addVxVy(__LINE__)

#define subVxVy()                                                    \
  registers[0xF] = registers[secondNibble] > registers[thirdNibble]; \
  registers[secondNibble] -= registers[thirdNibble];

#define xorVxVy() registers[secondNibble] ^= registers[thirdNibble];

#endif
