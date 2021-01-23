#include "instr_output.h"
#include "error_reports.h"
#include "hbios.h"
#include "instr_tms_output.h"
#include "instr_v9958_output.h"
#include "systemstate.h"
#include "tms.h"
#include "vdp.h"

void draw() {
  if (videoResMode == VideoResModeLow)
    tmsDraw();
  else
    v9958Draw();
}

void cls() {
  if (videoResMode == VideoResModeLow)
    tmsCls();
  else
    v9958Cls();
}

bool videoInit() {
  const uint8_t vdpType = videoChipProbe();

  switch (vdpType) {
  case VDP_NONE:
    print("Error: No VDP detected.\r\n");
    return false;
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

  tmsVideoInit();
  return true;
}

void videoClose() { tmsVideoClose(); }

void videoHigh() {
  videoResMode = VideoResModeHigh;
  videoPixelWidth = 128;
  videoPixelHeight = 64;
  videoPixelWidthMask = 127;
  videoPixelHeightMask = 63;

  v9958VideoInit();
}

void scrlDown() {
  if (fourthNibble == 0)
    return;

  if (videoResMode != VideoResModeLow)
    v9958ScrollDown();
}

void scrlUp() {
  if (fourthNibble == 0)
    return;

  if (videoResMode != VideoResModeLow)
    v9958ScrollUp();
}

#include "xstdio.h"

void scrlLeft() {
  if (videoResMode != VideoResModeLow) {
    xprintf("scroll left %d\r\n", _color);
    v9958ScrollLeft();
  }
}
