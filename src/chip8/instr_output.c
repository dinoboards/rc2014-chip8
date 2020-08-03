#include "instr_output.h"
#include "datatypes.h"
#include "hbios.h"
#include "systemstate.h"
#include "xstdio.h"

byte videoMemory[64 * 32 / 8];

inline void togglePixelAt(int index, byte x, byte y) {
  byte bit = 1 << (x % 8);
  byte current = videoMemory[index] ^= bit;

  if (!bit)
    return;

  if (current)
    xprintf("\033[%d;%dH\x1b[47;1m \x1b[40m", y, x);
  else {
    registers[0xF] = 1;
    xprintf("\033[%d;%dH ", y, x);
  }
}

void draw() {
  const byte x = registers[secondNibble];
  const byte y = registers[thirdNibble];

  registers[0xF] = 0;

  byte *spritePtr = (byte *)registerI;
  byte  index = (y * (64 / 8)) + (x / 8);

  for (byte i = y + 1; i < y + fourthNibble + 1; i++) {
    byte b = *spritePtr++;
    for (byte k = x; k < x + 8; k++) {
      if (b & 0x80)
        togglePixelAt(index, k, i);
      b = b << 1;
      index += (64 / 8);
    }
  }
}

void cls() { print("\033[2J\033[0;0H"); }
