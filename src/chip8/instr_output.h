
#ifndef __INSTR_OUTPUT
#define __INSTR_OUTPUT

#include "v9958.h"

extern void draw(void);
extern void cls(void);
extern bool videoInit(void);
extern void videoClose(void);
extern void videoHigh(void);
extern void scrlDown(void);
extern void scrlUp(void);
extern void scrlLeft(void);
extern void scrlRight(void);

extern void drawFrame(void);

#define videoPlane() _color = nibble2nd

#endif
