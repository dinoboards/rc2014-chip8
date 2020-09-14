
#ifndef __INSTR_OUTPUT
#define __INSTR_OUTPUT

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
