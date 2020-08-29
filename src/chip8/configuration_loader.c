
#include "configreader.h"
#include "cpm.h"
#include "error_reports.h"
#include "filereader.h"
#include "systemstate.h"

static void applyConfigColour(const uint8_t colourIndex) __z88dk_fastcall;

inline void applySingleConfig() {
  switch (token.type) {
  case TokenColour0:
    applyConfigColour(0);
    break;

  case TokenColour1:
    applyConfigColour(1);
    break;

  default:
    unexpectedEntry();
  }
}

void applyConfiguration(const char *pName) {
  FCB configFCB;
  resetFCB(pName, "CFG", &configFCB);

  uint8_t exists = fOpen(&configFCB);

  if (exists == 0xFF)
    return;

  fClose(&configFCB);

  setFileStream(&configFCB);
  openTokenStream();
  getNextToken();

  while (token.type != TokenEnd) {
    applySingleConfig();
    getNextToken();
  }

  closeTokenStream();
}

static void applyConfigColour(const uint8_t colourIndex) __z88dk_fastcall {
  getNextToken();
  if (token.type != TokenEquals)
    expectedEqualSign();

  getNextToken();
  gameColours[colourIndex] = token.type;
}
