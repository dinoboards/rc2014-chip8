
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "fontsets.h"
#include "timers.h"

void addVxVy() __naked {
  // clang-format off
  __asm
    ; // LOAD VY b (3rdNibble)
    ld	    a, (_currentInstruction + 1)
    rlca
    rlca
    rlca
    rlca
    and	    a, 0x0F
    ld      l, a
    ld      h, _registers / 256
    ld      b, (hl)

    ; // LOAD VX a (2ndNibble)
    ld	    a, (_currentInstruction)
    and	    a, 0x0F
    ld	    l, a
    ld	    a, (hl)

    add     a, b
    ld      (hl), a
    jr     nc, addVxVyClrF

    ld      a, 1
    ld	    (_registers + 0x0F), a
    ret

addVxVyClrF:
    xor     a
    ld	    (_registers + 0x0F), a
    ret
  __endasm;
  // clang-format on

  // const uint16_t i = registers[nibble2nd] + registers[nibble3rd];
  // registers[secondNibble] = i;
  // registers[0xF] = i > 255;
}

uint8_t *register2ndNibble;

void subnVxVy() {
  register2ndNibble = &registers[nibble2nd];
  const uint8_t *register3rdNibble = &registers[nibble3rd];

  const byte f = *register2ndNibble <= *register3rdNibble;
  *register2ndNibble = *register3rdNibble - *register2ndNibble;

  // quirks - set flag before subtraction

  registers[0xF] = f;
}

static byte units;
static byte hundreds;
static byte tens;

void bcdIVx() {
  const byte x = registers[nibble2nd];

  const byte im = x / 10;
  units = x - im * 10;
  hundreds = im / 10;
  tens = im - hundreds * 10;

  byte *p = (byte *)registerI;
  *p++ = hundreds;
  *p++ = tens;
  *p++ = units;
}

void ldfIVx() {
  const byte x = registers[nibble2nd];

  registerI = (uint16_t)&fonts[x * 5];
}

#endif
