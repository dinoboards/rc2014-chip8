#include "instr_output.h"
#include "instr_serial_output.h"
#include "instr_tms_output.h"
#include "systemstate.h"

void draw() {
  if (CommandSwitches.isSerial)
    serialDraw();
  else
    tmsDraw();
}

void cls() {
  if (CommandSwitches.isSerial)
    serialCls();
  else
    tmsCls();
}

void videoInit() {
  if (CommandSwitches.isSerial)
    serialVideoInit();
  else
    tmsVideoInit();
}
