#include "instr_output.h"
#include "dinotron.h"
#include "error_reports.h"
#include "instr_serial_output.h"
#include "instr_tms_output.h"
#include "instr_tms_high_output.h"
#include "systemstate.h"
#include "tms.h"

void draw() {
  if (CommandSwitches.isSerial)
    serialDraw();
  else
    if (videoResMode == VideoResModeLow)
      tmsDraw();
    else
      tmsHighDraw();
}

void cls() {
  if (CommandSwitches.isSerial)
    serialCls();
  else
    if (videoResMode == VideoResModeLow)
      tmsCls();
    else
      tmsHighCls();
}

void videoInit() {
  videoResMode = VideoResModeLow;
  videoPixelWidth = 64;
  videoPixelHeight = 32;
  videoPixelWidthMask = 63;
  videoPixelHeightMask = 31;

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

void videoHigh() {
  videoResMode = VideoResModeHigh;
  videoPixelWidth = 128;
  videoPixelHeight = 64;
  videoPixelWidthMask = 127;
  videoPixelHeightMask = 63;
  tmsHighVideoInit();
}
