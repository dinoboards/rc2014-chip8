#include "configreader.h"
#include "cpm.h"
#include "error_reports.h"
#include "filereader.h"
#include "systemstate.h"

static void applyConfigColour(const uint8_t colourIndex) __z88dk_fastcall;
static void applyConfigKey();

inline void applySingleConfig() {
  switch (token.type) {
  case TokenColour0:
    applyConfigColour(0);
    break;

  case TokenColour1:
    applyConfigColour(1);
    break;

  case TokenKey:
    applyConfigKey();

  case TokenEnd:
    return;

  default:
    unexpectedEntry();
  }
}

void parseConfiguration(FCB *configFCB) __z88dk_fastcall {
  setFileStream(configFCB);
  openTokenStream();
  getNextToken();

  while (token.type != TokenEnd) {
    applySingleConfig();
    getNextToken();
  }

  closeTokenStream();
}

void applyConfiguration(const char *pName) __z88dk_fastcall {
  FCB configFCB;

  resetFCB(pName, "CFG", &configFCB);

  uint8_t exists = fOpen(&configFCB);

  if (exists == 0xFF)
    return;

  fClose(&configFCB);

  parseConfiguration(&configFCB);
}

static void applyConfigColour(const uint8_t colourIndex) __z88dk_fastcall {
  getNextToken();
  if (token.type != TokenEquals)
    expectedEqualSign();

  getNextToken();
  gameColours[colourIndex] = token.type;
}

uint8_t expectToBeHexCharIdentifier() {
  if (token.type != TokenIdentifier)
    expectedError("hex key code");

  const char c = token.value[0];
  if (strlen(token.value) != 1)
    expectedError("hex key code");

  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  expectedError("hex key code");

  return 0;
}

const char *expectToBeKeyIdentifier() {
  if (token.type != TokenIdentifier)
    expectedError("hex key code");

  return token.value;
}

void expectToBeEquals() {
  if (token.type != TokenEquals)
    expectedError("=");
}

void expectToBeKey() {
  if (token.type != TokenKey)
    expectedError("KEY-");
}

void expectToBeDash() {
  if (token.type != TokenDash)
    expectedError("-");
}

static void applyConfigKey() {
  getNextToken();
  expectToBeDash();

  getNextToken();
  const uint8_t pSourceKey = expectToBeHexCharIdentifier();

  getNextToken();
  expectToBeEquals();

  getNextToken();
  expectToBeKey();

  getNextToken();
  expectToBeDash();

  getNextToken();
  const char *c = expectToBeKeyIdentifier();

  gameKeys[pSourceKey] = c[0];
}
