#include "instr_output.h"
#include "dinotron.h"
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
    dinotronSwitchSerial();
    serialVideoInit();
    return;
  }

  if (tmsSearchDriver()) {
    CommandSwitches.isTms = true;
    tmsVideoInit();
    dinotronSwitchTms();
    return;
  }

  if (CommandSwitches.isTms)
    tmsDriverNotFound();

  CommandSwitches.isSerial = true;

  dinotronSwitchSerial();
  serialVideoInit();
}

void videoClose() { dinotronSwitchSerial(); }
