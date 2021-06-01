#include "instr_output.h"
#include "error_reports.h"
#include "instr_tms_output.h"
#include "instr_v9958_output.h"
#include "systemstate.h"
#include "tms.h"
#include "vdp.h"
#include <stdio.h>

void draw() {
  // int xx = (registers[nibble2nd] * 2) & 255; //PIXEL_WIDTH_MASK;
  // int yy = (registers[nibble3rd] * 2) & 127 ;//PIXEL_HEIGHT_MASK;

  // printf("  Draw(%d, %d, %d), PLANE: %d, I: %04X", xx, yy, fourthNibble, _color, registerI);
  if (videoResMode == VideoResModeLow)
    tmsDraw();
  else
    v9958Draw();
}

void cls() {
  // printf("cls\r\n");
  if (videoResMode == VideoResModeLow)
    tmsCls();
  else
    v9958Cls();
}

bool videoInit() {
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

void scrlLeft() {
  if (videoResMode != VideoResModeLow) {
    v9958ScrollLeft();
  }
}

void scrlRight() {
  if (videoResMode != VideoResModeLow) {
    v9958ScrollRight();
  }
}
