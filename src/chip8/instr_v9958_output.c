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

// byte spriteRowData;
byte yAddOne;
byte xxTo;

extern void initDrawParams();
extern void drawRow(byte pSpriteData) __z88dk_fastcall;
extern void v9959DrawPlane(byte *pSpriteData) __z88dk_fastcall;
extern void v9959DrawDblPlane(byte *pSpriteData) __z88dk_fastcall;

void v9958Draw() {
  registers[15] = 0;

  xx = (registers[nibble2nd] * 2) & PIXEL_WIDTH_MASK;
  yy = (registers[nibble3rd] * 2) & PIXEL_HEIGHT_MASK;

  if (fourthNibble == 0) {
    fourthNibble = 16;
    if (_color == 3) {
      _color = 1;
      const uint8_t originalYY = yy;
      v9959DrawDblPlane((byte *)registerI);
      yy = originalYY;
      _color = 2;
      v9959DrawDblPlane((byte *)(registerI + 32));
      _color = 3;
    } else
      v9959DrawDblPlane((byte *)registerI);

    return;
  }

  if (_color == 3) {
    _color = 1;
    const uint8_t originalYY = yy;
    v9959DrawPlane((byte *)registerI);
    yy = originalYY;
    _color = 2;
    v9959DrawPlane((byte *)(registerI + fourthNibble));
    _color = 3;
  } else
    v9959DrawPlane((byte *)registerI);
}

void v9959DrawDblPlane(byte *pSpriteData) __z88dk_fastcall {
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

RGB palette[16] = {
    {0, 0, 0}, {0, 0, 7}, {0, 7, 0}, {7, 0, 0}, {0, 7, 7}, {7, 0, 7}, {7, 7, 0}, {7, 7, 7}, {15, 7, 15}, {0, 0, 15}, {0, 15, 0}, {0, 15, 15}, {15, 0, 0}, {15, 0, 15}, {15, 15, 0}, {15, 15, 15},
};

void v9958VideoInit() {
  setMode4(212, NTSC);
  setPalette(palette);
  clearScreenBank0(0);

  initDrawParams();
}
