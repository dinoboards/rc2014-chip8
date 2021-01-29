#ifndef __V9958
#define __V9958

#include "datatypes.h"
#include <stdbool.h>

#define PAL  1
#define NTSC 2

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} RGB;

extern void setPalette(RGB *) __z88dk_fastcall;
extern void setMode4(uint8_t lines, uint8_t mode);
extern void setMode6(uint8_t lines, uint8_t mode);
extern void setMode7(uint8_t lines, uint8_t mode);
extern void clearScreenBank0(uint8_t color) __z88dk_fastcall;
extern void clearScreenBank1(uint8_t color) __z88dk_fastcall;
extern void _writeRegister(uint16_t rd) __z88dk_fastcall;

#define writeRegister(a, b) _writeRegister(a * 256 + b)

#define REGISTER_COUNT 12

#define CMD_VDP_TO_VRAM 0xC0

void waitForCommandCompletion();
void setReadRegisterToZero();

extern uint8_t  _color;
extern uint16_t _toX;
extern uint16_t _toY;

#endif
