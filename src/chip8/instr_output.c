#include "chip8/instr_output.h"
#include "chip8/systemstate.h"
#include "chip8/tty.h"
#include "datatypes.h"

byte videoMemory[64 * 32 / 8];

byte *spritePtr;
byte  indexForY;
byte  spriteByte;
byte  x;
byte  y;
byte  index;
byte  bit;
byte  current;
byte  bitCounter;
byte  startingY;

void draw() {
  registers[0x0F] = 0;
  x = registers[secondNibble] + 1;
  startingY = registers[thirdNibble] + 1;

  spritePtr = (byte *)registerI;

  for (y = startingY; y < startingY + fourthNibble; y++) {
    spriteByte = *spritePtr++;
    indexForY = y * (64 / 8);

    for (bitCounter = 0; bitCounter < 8; bitCounter++) {
      if (spriteByte & 0x80) {

        index = indexForY + (x >> 3);
        bit = 1 << (x % 8);
        current = (videoMemory[index] ^= bit) & bit;

        if (current)
          sendDrawCommands("\033[%d;%dH\x1b[47;1m \x1b[40m", y, x);
        else {
          registers[0x0F] = 1;
          sendDrawCommands("\033[%d;%dH ", y, x);
        }
      }

      spriteByte = spriteByte << 1;
      x++;
    }
    x -= 8;
  }
}

void cls() { sendDrawCommands("\033[2J\033[0;0H"); }
