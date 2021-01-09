
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#include "fontsets.h"
#include "timers.h"

#define ldVxByte()  (registers[nibble2nd] = lowByte)
#define ldIAddr()   (registerI = addr)
#define ldILargeAddr() (registerI = invertByteOrder(*chip8PC++))

#define addVxByte() (registers[nibble2nd] += lowByte)

#define ldDtVx()                         \
  {                                      \
    initTimers();                        \
    (delayTimer = registers[nibble2nd]); \
  }

#define ldVxDt() (registers[nibble2nd] = delayTimer)

#define addIVx() (registerI += registers[nibble2nd])

inline void ldVxVy() {
  //(registers[nibble2nd] = registers[nibble3rd])
  // clang-format off
  __asm
    ld        hl, (_currentInstruction)
    ld        a, l

    and	      a, 0x0f
    ld	      d,_registers / 256
    ld	      e, a

    ld        a, h
    rlca
    rlca
    rlca
    rlca
    and	      a, 0x0f
    ld	      c, a
    ld	      b, _registers / 256

    ld	      a, (bc)
    ld	      (de), a
  __endasm;
  // clang-format on
}

// TODO Quicks - set I to end value
inline void ldVxI() {
  // clang-format off
  __asm
    ld	    hl, (_registerI)
    ld	    de, _registers

    ld	    a, (_currentInstruction)
    and	    a, 0x0F
    inc     a
    ld	    c, a
    ld      b, 0

    ldir
  __endasm;
  // clang-format on
}

inline void ldIVx() {
  // clang-format off
  __asm
    ld	    de, (_registerI)
    ld	    hl, _registers

    ld	    a, (_currentInstruction)
    and	    a, 0x0F
    inc     a
    ld	    c, a

    ld      b, 0
    ldir
  __endasm;
  // clang-format on
}

extern void ldIVxVy();
extern void ldVxVyI();

inline void andVxVy() {
  // clang-format off
  __asm
    ; // LOAD VX ADDR INTO DE (2ndNibble)
    ld      d, _registers / 256
    ld	    a, (_currentInstruction)
    and	    a, 0x0F
    ld	    e, a

    ; // LOAD VY ADDR INTO HL (3rdNibble)
    ld	    h, d
    ld	    a, (_currentInstruction + 1)
    rlca
    rlca
    rlca
    rlca
    and	    a, 0x0f
    ld	    l, a

    ld      a, (de)
    and     a, (hl)
    ld      (de), a
  __endasm;
  // clang-format on
}

inline void orVxVy() {
  // clang-format off
  __asm
    ; // LOAD VX ADDR INTO DE (2ndNibble)

    ld      d, _registers / 256
    ld	    a, (_currentInstruction)
    and	    a, 0x0F
    ld	    e, a

    ; // LOAD VY ADDR INTO HL (3rdNibble)
    ld      h, d
    ld	    a, (_currentInstruction + 1)
    rlca
    rlca
    rlca
    rlca
    and	    a, 0x0f
    ld	    l, a

    ld      a, (de)
    or      a, (hl)
    ld      (de), a
  __endasm;
  // clang-format on
}

inline void shrVxVy() {
  uint8_t *register2ndNibble = &registers[nibble2nd];
  registers[0xF] = *register2ndNibble & 0x1;
  *register2ndNibble >>= 1;
}

inline void shlVxVy() {
  uint8_t *register2ndNibble = &registers[nibble2nd];

  registers[0xF] = !!(*register2ndNibble & 0x80);
  *register2ndNibble <<= 1;
}

static void addVxVy() __naked {
  // clang-format off
  __asm
    xor     a
    ld	    ((_registers + 0x0F)), a

    ; // LOAD VY b (3rdNibble)
    ld	    a, (_currentInstruction + 1)
    rlca
    rlca
    rlca
    rlca
    and	    a, 0x0f
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
    ret     nc

    ld      a, 1
    ld	    (_registers + 0x0f), a
    ret
  __endasm;
  // clang-format on

  // const uint16_t i = registers[nibble2nd] + registers[nibble3rd];
  // registers[secondNibble] = i;
  // registers[0xF] = i > 255;
}

static uint8_t *register2ndNibble;

static void subVxVy() {
  register2ndNibble = &registers[nibble2nd];
  const uint8_t *register3rdNibble = &registers[nibble3rd];

  //quirks - set flag before subtraction

  const byte f = *register2ndNibble >= *register3rdNibble;
  *register2ndNibble -= *register3rdNibble;
  registers[0xF] = f;
}

static void subnVxVy() {
  register2ndNibble = &registers[nibble2nd];
  const uint8_t *register3rdNibble = &registers[nibble3rd];

  const byte f = *register2ndNibble <= *register3rdNibble;
  *register2ndNibble = *register3rdNibble - *register2ndNibble;

  //quirks - set flag before subtraction

  registers[0xF] = f;
}

static void xorVxVy() { registers[nibble2nd] ^= registers[nibble3rd]; }

static byte units;
static byte hundreds;
static byte tens;

static void bcdIVx() {
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

static void ldfIVx() {
  const byte x = registers[nibble2nd];

  registerI = (uint16_t)&fonts[x * 5];
}

#endif
