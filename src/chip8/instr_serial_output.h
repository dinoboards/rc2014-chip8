
#ifndef __INSTR_SERIAL_OUTPUT
#define __INSTR_SERIAL_OUTPUT

#include "datatypes.h"

// required for testing
extern byte videoMemory[128 * 64 / 8];
extern void serialDraw();
extern void serialCls();
extern void serialVideoInit();

extern void drawFrame();

#endif
