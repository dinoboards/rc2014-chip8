
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "fontsets.h"
#include "timers.h"

void ldfIVx() {
  const byte x = registers[nibble2nd];

  registerI = (uint16_t)&fonts[x * 5];
}

#endif
