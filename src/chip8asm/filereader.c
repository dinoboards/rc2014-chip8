#include "chip8asm/filereader.h"
#include "charconstants.h"
#include "cpm.h"
#include "datatypes.h"

static uint8_t  diskBuffer[129];
static uint8_t *diskBufferP;

void openFileStream() {
  chk(fOpen(defaultFCB));
  chk(fDmaOff(diskBuffer));
  chk(fRead(defaultFCB));

  diskBufferP = diskBuffer;
}

void closeFileStream() { chk(fClose(defaultFCB)); }

static char getNextCharRaw() {
  if (diskBufferP >= &diskBuffer[128]) {
    chk(fDmaOff(diskBuffer));
    chk(fRead(defaultFCB));
    diskBufferP = diskBuffer;
  }

  if (*diskBufferP == CTRL_Z)
    return '\0';

  return *diskBufferP++;
}

char getNextChar() {
  char c = CR;
  while (c == CR)
    c = getNextCharRaw();
  return c;
}
