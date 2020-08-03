#ifndef __EMITTERS
#define __EMITTERS
#include "datatypes.h"

void emitByte(byte data) __z88dk_fastcall;

void emit(uint16_t data) __z88dk_fastcall;

void emitNibbles(byte nibble1, byte nibble2, byte nibble3, byte nibble4);

void emit2Nibble(byte nibble1, byte nibble2);

#endif
