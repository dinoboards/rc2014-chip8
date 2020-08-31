#include "instr_serial_output.h"
#include "datatypes.h"
#include "instr_serial_output.h"
#include "systemstate.h"
#include "tty.h"
#include <string.h>

byte videoMemory[64 * 32 / 8];

byte *spritePtr;
byte  indexForY;
byte  spriteByte;
byte  x;
byte  y;
byte  vIndex;
byte  bit;
byte  current;
byte  bitCounter;
byte  startingY;

void serialDraw() {
  registers[0x0F] = 0;
  x = registers[secondNibble] & 63;
  startingY = registers[thirdNibble] & 31;

  spritePtr = (byte *)registerI;

  for (y = startingY; y < startingY + fourthNibble; y++) {
    spriteByte = *spritePtr++;
    indexForY = (y & 31) * (64 / 8);

    for (bitCounter = 0; bitCounter < 8; bitCounter++) {
      if (spriteByte & 0x80) {

        vIndex = indexForY + ((x & 63) >> 3);
        bit = 1 << ((x & 63) % 8);
        current = (videoMemory[vIndex] ^= bit) & bit;

        if (current)
          sendDrawCommands("\033[%d;%dHX", (y & 31) + 1, (x & 63) + 1);
        else {
          registers[0x0F] = 1;
          sendDrawCommands("\033[%d;%dH ", (y & 31) + 1, (x & 63) + 1);
        }
      }

      spriteByte = spriteByte << 1;
      x++;
    }
    x -= 8;
  }
}

void serialCls() {
  memset(videoMemory, 0, sizeof(videoMemory));
  sendDrawCommands("\033[?25l\033[2J\033[0;0H");
}

void serialVideoInit() { serialCls(); }
