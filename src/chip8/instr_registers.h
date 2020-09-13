
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "fontsets.h"
#include "timers.h"

#define ldVxByte()  (registers[secondNibble] = lowByte)
#define ldIAddr()   (registerI = addr)
#define addVxByte() (registers[secondNibble] += lowByte)

#define ldDtVx()                            \
  {                                         \
    initTimers();                           \
    (delayTimer = registers[secondNibble]); \
  }

#define ldVxDt() (registers[secondNibble] = delayTimer)

#define addIVx() (registerI += registers[secondNibble])

#define ldVxVy() (registers[secondNibble] = registers[thirdNibble])

// TODO Quicks - set I to end value
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

inline void orVxVy() {
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
    or      a, (hl)
    ld      (de), a
  __endasm;
  // clang-format on
}

inline void shrVxVy() {
  registers[0xF] = registers[secondNibble] & 0x1;
  registers[secondNibble] = registers[secondNibble] >> 1;
}

inline void shlVxVy() {
  registers[0xF] = !!(registers[secondNibble] & 0x80);
  registers[secondNibble] = registers[secondNibble] << 1;
}

static void addVxVy() __naked {
  __asm
    ld      a, 0
    ld	    ((_registers + 0x0F)), a
    ld	    de, _registers

    ld	    hl, (_thirdNibble)
    ld	    h, 0x00
    add	    hl, de
    ld	    b, (hl)

    ld	    hl, (_secondNibble)
    ld	    h, 0x00
    add	    hl, de
    ld	    a, (hl)

    add     a, b
    ld      (hl), a
    ret     nc

    ld      a, 1
    ld	    (_registers + 0x0f), a
    ret
  __endasm;
  // const uint16_t i = registers[secondNibble] + registers[thirdNibble];
  // registers[secondNibble] = i;
  // registers[0xF] = i > 255;
}

static void subVxVy() {
  registers[0xF] = registers[secondNibble] > registers[thirdNibble];
  registers[secondNibble] -= registers[thirdNibble];
}

static void subnVxVy() {
  registers[0xF] = registers[secondNibble] < registers[thirdNibble];
  registers[secondNibble] = registers[thirdNibble] - registers[secondNibble];
}

static void xorVxVy() { registers[secondNibble] ^= registers[thirdNibble]; }

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
