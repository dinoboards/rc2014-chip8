#include "instr_output.h"
#include "error_reports.h"
#include "instr_tms_output.h"
#include "instr_v9958_output.h"
#include "systemstate.h"
#include "tms.h"
#include "vdp.h"
#include <stdio.h>

void cls() {
#ifndef CPM
  if (videoResMode == VideoResModeLow)
    tmsCls();
  else
    v9958Cls();
#endif
}

bool videoInit() {
#ifndef CPM
  const uint8_t vdpType = videoChipProbe();

  switch (vdpType) {
  case VDP_NONE:
    printf("Error: No VDP detected.\r\n");
    return false;
    break;

  case VDP_TMS:
    printf("TMS9918 VDP Detected.  Only lores supported.\r\n");
    break;

  case VDP_V9938:
    printf("V9938 VDP Detected.\r\n");
    break;

  case VDP_V9958:
    printf("V9958 VDP Detected.\r\n");
  }
#else
  const uint8_t vdpType = VDP_V9958;
#endif

  videoResMode = VideoResModeLow;
  drawFunctionPtr = tmsDraw;

  videoPixelWidth = 64;
  videoPixelHeight = 32;
  videoPixelWidthMask = 63;
  videoPixelHeightMask = 31;

#ifndef CPM
  tmsVideoInit();
#endif
  return true;
}

void videoClose() { tmsVideoClose(); }

void videoHigh() {
  videoResMode = VideoResModeHigh;
  drawFunctionPtr = _color == 3 ? v9958DrawPlane3 : v9958DrawSinglePlane;
  videoPixelWidth = 128;
  videoPixelHeight = 64;
  videoPixelWidthMask = 127;
  videoPixelHeightMask = 63;

#ifndef CPM
  v9958VideoInit();
#endif
}

void scrlDown() {
#ifndef CPM
  if (fourthNibble == 0)
    return;

  if (videoResMode != VideoResModeLow)
    v9958ScrollDown();
#endif
}

void scrlUp() {
#ifndef CPM
  if (fourthNibble == 0)
    return;

  if (videoResMode != VideoResModeLow)
    v9958ScrollUp();
#endif
}

void scrlLeft() {
#ifndef CPM
  if (videoResMode != VideoResModeLow) {
    v9958ScrollLeft();
  }
#endif
}

void scrlRight() {
#ifndef CPM
  if (videoResMode != VideoResModeLow) {
    v9958ScrollRight();
  }
#endif
}
