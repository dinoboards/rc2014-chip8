#include "instr_v9958_output.h"
#include "datatypes.h"
#include "instr_output.h"
#include "systemstate.h"
#include "v9958.h"

byte        xx;
byte        yy;

byte yAddOne;

extern void waitAndClearCommandRegisters();

void v9958Cls() { clearScreenBank0(0); }

RGB palette[16] = {{0, 0, 0}, {0xF, 0x0, 0}, {0x0, 0xF, 0}, {0, 0, 0xF}, {7, 0, 7}, {7, 7, 0}, {7, 7, 7}, {15, 7, 15}, {0, 0, 15}, {0, 15, 0}, {0, 15, 15}, {15, 0, 0}, {15, 0, 15}, {15, 15, 0}, {15, 15, 15}, {15, 15, 15}};

void v9958VideoInit() {
  setMode4(212, NTSC);
  setPalette(palette);
  clearScreenBank0(0);

  waitAndClearCommandRegisters();
}
