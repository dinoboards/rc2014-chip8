#include "instr_serial_output.h"
#include "datatypes.h"
#include "instr_serial_output.h"
#include "pigfx.h"
#include "systemstate.h"
#include "tty.h"
#include <string.h>

byte videoMemory[128 * 64 / 8];

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
  x = registers[nibble2nd] & videoPixelWidthMask;
  startingY = registers[thirdNibble] & videoPixelHeightMask;

  spritePtr = (byte *)registerI;

  for (y = startingY; y < startingY + fourthNibble; y++) {
    spriteByte = *spritePtr++;
    indexForY = (y & videoPixelHeightMask) * (videoPixelWidth / 8);

    for (bitCounter = 0; bitCounter < 8; bitCounter++) {
      if (spriteByte & 0x80) {

        vIndex = indexForY + ((x & videoPixelWidthMask) >> 3);
        bit = 1 << ((x & videoPixelWidthMask) % 8);
        current = (videoMemory[vIndex] ^= bit) & bit;

        if (current)
          sendDrawCommands("\033[%d;%dH\033[48;5;2m " TERM_RESET, (y & videoPixelHeightMask) + 1, (x & videoPixelWidthMask) + 1);
        else {
          registers[0x0F] = 1;
          sendDrawCommands("\033[%d;%dH ", (y & videoPixelHeightMask) + 1, (x & videoPixelWidthMask) + 1);
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
  sendDrawCommands(TERM_MODE_2 TERM_FONT_8X8 TERM_CURSOR_INVISIBLE TERM_CLS TERM_RESET);
}

void serialVideoInit() { serialCls(); }
