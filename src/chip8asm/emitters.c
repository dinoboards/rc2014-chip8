#include "emitters.h"
#include "datatypes.h"
#include "systemstate.h"

void emitByte(byte data) __z88dk_fastcall {
  *programPtr++ = data;
  currentAddress += 1;
}

void emit(uint16_t data) __z88dk_fastcall {
  *programPtr++ = data >> 8;
  *programPtr++ = data & 0xFF;

  currentAddress += 2;
}

void emitNibbles(byte nibble1, byte nibble2, byte nibble3, byte nibble4) {
  *programPtr++ = (((int)nibble1) << 4) + nibble2;
  *programPtr++ = (((int)nibble3) << 4) + nibble4;
  currentAddress += 2;
}

void emit2Nibble(byte nibble1, byte nibble2) {
  *programPtr++ = (((int)nibble1) << 4) + nibble2;
  currentAddress++;
}
