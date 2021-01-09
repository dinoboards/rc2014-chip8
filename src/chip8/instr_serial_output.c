#include "datatypes.h"
#include "hbios.h"
#include "pigfx.h"
#include "systemstate.h"
#include "timers.h"
#include "tty.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "xstdio.h"

#define ESC "\033"

#define PIXEL_WIDTH 64
#define PIXEL_HEIGHT 32

byte videoMemory[64 * 32 / 8];
byte bufferedVideoMemory[64 * 32 / 8];

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

void drawFrame();

static uint8_t pendingChanges = false;
static uint8_t lastTick = 0;
static uint8_t counter = 255;

void serialDraw() {
  registers[0xF] = 0;
  pendingChanges = true;
  x = registers[nibble2nd] & 63;
  startingY = registers[nibble3rd] & 31;
  spritePtr = (byte *)registerI;

  for (y = startingY; y < startingY + fourthNibble; y++) {
    spriteByte = *spritePtr++;

    indexForY = (y & 31) * (64 / 8);

    for (bitCounter = 8; bitCounter != 0; bitCounter--) {
      if (spriteByte & 0x80) {
        vIndex = indexForY + ((x & 63) >> 3);
        bit = 1 << ((x & 63) % 8);
        current = (videoMemory[vIndex] ^= bit) & bit;

        if (!current)
          registers[0x0F] = 1;
      }

      spriteByte = spriteByte << 1;
      x++;
    }
    x -= 8;
  }

  xprintf("\r\n");
}

void drawFrame() {
  if (!pendingChanges)
    return;

  if (lastTick == timerTick)
    return;

  lastTick = timerTick;
  counter++;

  if (counter % 4)
    return;

  pendingChanges = false;

  for (y = 0; y < 32; y++) {
    indexForY = (y & 31) * (64 / 8);

    for (x = 0; x < 64; x += 8) {
      vIndex = indexForY + ((x & 63) >> 3);
      byte data = videoMemory[vIndex];
      byte data2 = bufferedVideoMemory[vIndex];

      if (data2 != data) {
        sendDrawCommands("\033[%d;%dH", y + 1, x + 1);
        bufferedVideoMemory[vIndex] = data;
        for (byte i = 8; i != 0; i--) {
          if ((data & 0x01))
            sendDrawCommands("\033[48;5;2m " TERM_RESET);
          else
            sendDrawCommands(" ");

          data = data >> 1;
        }
      }
    }
  }
}

void serialVideoInit() {
  memset(videoMemory, 0, sizeof(videoMemory));
  memset(bufferedVideoMemory, 0, sizeof(bufferedVideoMemory));
}

void serialCls() {
  memset(videoMemory, 0, sizeof(videoMemory));
  memset(bufferedVideoMemory, 0, sizeof(bufferedVideoMemory));
  sendDrawCommands(TERM_MODE_2 TERM_FONT_8X8 TERM_CURSOR_INVISIBLE TERM_CLS TERM_RESET);
  pendingChanges = false;
}
