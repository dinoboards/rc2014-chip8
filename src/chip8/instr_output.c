#include "instr_output.h"
#include "error_reports.h"
#include "instr_serial_output.h"
#include "instr_tms_output.h"
#include "instr_v9958_output.h"
#include "systemstate.h"
#include "tms.h"
#include "vdp.h"
#include "hbios.h"

void draw() {
  if (CommandSwitches.isSerial)
    serialDraw();
  else if (videoResMode == VideoResModeLow)
    tmsDraw();
  else
    v9958Draw();
}

void cls() {
  if (CommandSwitches.isSerial)
    serialCls();
  else if (videoResMode == VideoResModeLow)
    tmsCls();
  else
    v9958Cls();
}

void videoInit() {

  const uint8_t vdpType = videoChipProbe();

  switch(vdpType) {
    case VDP_NONE:
      print("Error: No VDP detected.\r\n");
      break;

    case VDP_TMS:
      print("TMS9918 VDP Detected.  Only lores supported.\r\n");
      break;

    case VDP_V9938:
      print("V9938 VDP Detected.\r\n");
      break;

    case VDP_V9958:
      print("V9958 VDP Detected.\r\n");
  }

  videoResMode = VideoResModeLow;
  videoPixelWidth = 64;
  videoPixelHeight = 32;
  videoPixelWidthMask = 63;
  videoPixelHeightMask = 31;

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

void videoClose() { tmsVideoClose(); }

void videoHigh() {
  videoResMode = VideoResModeHigh;
  videoPixelWidth = 128;
  videoPixelHeight = 64;
  videoPixelWidthMask = 127;
  videoPixelHeightMask = 63;

  if (CommandSwitches.isTms)
    v9958VideoInit();
}
