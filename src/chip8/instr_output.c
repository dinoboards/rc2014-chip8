#include "instr_output.h"
#include "error_reports.h"
#include "instr_serial_output.h"
#include "instr_tms_output.h"
#include "systemstate.h"
#include "tms.h"

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
  if (CommandSwitches.isSerial) {
    serialVideoInit();
    return;
  }

  if (tmsSearchDriver()) {
    CommandSwitches.isTms = true;
    tmsVideoInit();
    return;
  }

  if (CommandSwitches.isTms)
    tmsDriverNotFound();

  CommandSwitches.isSerial = true;

  serialVideoInit();
}
