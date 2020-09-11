#include "instr_tms_high_output.h"
#include "datatypes.h"

void tmsHighDraw() {}

void tmsHighCls() {}
/*
static void buildPatternData(byte *pData) {
  for (byte i = 0; i < 16; i++) {
    byte top = 0;
    byte bottom = 0;

    if (i & 0x1) {
      top = 0xF0;
    }
    if (i & 0x2) {
      top |= 0x0F;
    }
    if (i & 0x4) {
      bottom = 0xF0;
    }
    if (i & 0x8) {
      bottom |= 0x0F;
    }

    *pData++ = top;
    *pData++ = top;
    *pData++ = top;
    *pData++ = top;
    *pData++ = bottom;
    *pData++ = bottom;
    *pData++ = bottom;
    *pData++ = bottom;
  }
}

void tmsInitPatterns() {
  byte          pixelData[8 * 16 * 4];
  tmsDataParams patternParams = {TMS_MD1_PATTERN_TABLE, sizeof(pixelData), pixelData};

  buildPatternData(pixelData);
  tmsWriteData(&patternParams);
}*/

void tmsHighVideoInit() {
  // tmsInit();
  // tmsRegisterColours(gameColours[0], gameColours[1]);
  // colourParams.data = (gameColours[1] << 4) + gameColours[0];
  // tmsSetMode1();
  // tmsClearData(&colourParams);
  // tmsInitPatterns();
  // tmsCls();
}
