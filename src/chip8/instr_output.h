
#ifndef __INSTR_OUTPUT
#define __INSTR_OUTPUT

#include "v9958.h"

extern void draw();
extern void cls();
extern void videoInit();
extern void videoClose();
extern void videoHigh();

extern void drawFrame();

#define serialDrawFrame()       \
  if (CommandSwitches.isSerial) \
  drawFrame()
#endif

#define videoPlane() _color = nibble2nd
