
#ifndef __INSTR_OUTPUT
#define __INSTR_OUTPUT

#include "v9958.h"

extern void draw();
extern void cls();
extern bool videoInit();
extern void videoClose();
extern void videoHigh();

extern void drawFrame();

#define videoPlane() _color = nibble2nd

#endif
