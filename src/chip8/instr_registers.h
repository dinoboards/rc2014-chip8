
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "fontsets.h"
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
    ld	    hl, _registerI
    ld	    a, (hl)
    inc	    hl
    ld	    h, (hl)
    ld	    l, a
    ld	    de, _registers
    ld      a, (_secondNibble)
    ld      c, a
    inc     c
    ld      b, 0
    ldir

  	ex	    de, hl
    ld	    hl, _registerI
    ld	    (hl), e
    inc	    hl
    ld	    (hl), d

  __endasm;
  // clang-format on
}

inline void ldIVx() {
  // clang-format off
  __asm
    ld	    hl, _registerI
    ld	    e, (hl)
    inc	    hl
    ld	    d, (hl)
    ld	    hl, _registers
    ld      a, (_secondNibble)
    ld      c, a
    inc     c
    ld      b, 0
    ldir

    ld	    hl, _registerI
    ld	    (hl), e
    inc	    hl
    ld	    (hl), d
  __endasm;
  // clang-format on
}

inline void andVxVy() {
  // clang-format off
  __asm
    ld      a, (_secondNibble)
    ld      hl, _registers
    ld      c, a
    ld      b, 0
    add     hl, bc
    ex      de, hl
    ld      hl, _registers
    ld      a, (_thirdNibble)
    ld      c, a
    add     hl, bc
    ld      a, (de)
    and     a, (hl)
    ld      (de), a
  __endasm;
  // clang-format on
}

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

static byte units;
static byte hundreds;
static byte tens;

static void bcdIVx() {
  const byte x = registers[secondNibble];

  const byte im = x / 10;
  units = x - im * 10;
  hundreds = im / 10;
  tens = im - hundreds * 10;

  byte *p = (byte *)registerI;
  *p++ = hundreds;
  *p++ = tens;
  *p++ = units;
}

static void ldfIVx() {
  const byte x = registers[secondNibble];

  registerI = (uint16_t)&fonts[x * 5];
}
#endif
