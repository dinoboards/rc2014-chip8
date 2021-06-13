#include "instr_v9958_output.h"
#include "datatypes.h"
#include "instr_output.h"
#include "systemstate.h"
#include "v9958.h"

byte        xx;
byte        yy;
static byte spriteHeight;

#define PIXEL_WIDTH_MASK  255
#define PIXEL_HEIGHT_MASK 127
byte yAddOne;
byte xxTo;

extern void waitAndClearCommandRegisters();
extern void drawRow(byte pSpriteData) __z88dk_fastcall;
extern void v9958DrawPlane(byte *pSpriteData) __z88dk_fastcall;
extern void v9958DrawDblPlane(byte *pSpriteData) __z88dk_fastcall;

void v9958DrawDblPlane(byte *pSpriteData) __z88dk_fastcall {
  for (byte row = 16; row > 0; row--) {
    yAddOne = (yy + 1) & PIXEL_HEIGHT_MASK;

    drawRow(*pSpriteData++);

    xx += 16;

    drawRow(*pSpriteData++);
    xx -= 16;

    yy = (yAddOne + 1) & PIXEL_HEIGHT_MASK;
  }
}

void v9958Cls() { clearScreenBank0(0); }

RGB palette[16] = {{0, 0, 0}, {0xF, 0x0, 0}, {0x0, 0xF, 0}, {0, 0, 0xF}, {7, 0, 7}, {7, 7, 0}, {7, 7, 7}, {15, 7, 15}, {0, 0, 15}, {0, 15, 0}, {0, 15, 15}, {15, 0, 0}, {15, 0, 15}, {15, 15, 0}, {15, 15, 15}, {15, 15, 15}};

void v9958VideoInit() {
  setMode4(212, NTSC);
  setPalette(palette);
  clearScreenBank0(0);

  waitAndClearCommandRegisters();
}
