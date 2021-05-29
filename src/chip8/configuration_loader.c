#include "configreader.h"
#include "error_reports.h"
#include "filenames.h"
#include "filereader.h"
#include "systemstate.h"
#include "v9958.h"
#include <stdio.h>

static void applyConfigColour();
static void applyConfigKey();

void applySingleConfig() {
  switch (token.type) {
  case TokenColour:
    applyConfigColour();
    break;

  case TokenKey:
    applyConfigKey();

  case TokenEnd:
    return;

  default:
    unexpectedEntry();
  }
}

void parseConfiguration(const char *pConfigFileName) __z88dk_fastcall {
  setFileName(pConfigFileName);
  openTokenStream();
  getNextToken();

  while (token.type != TokenEnd) {
    applySingleConfig();
    getNextToken();
  }

  closeTokenStream();
}

void applyConfiguration(const char *pName) __z88dk_fastcall {
  char configFileName[MAX_FILE_NAME];

  replaceExtension(configFileName, pName, ".CFG");

  FILE *pFile = fopen(configFileName, "r");

  if (pFile == NULL)
    return;

  fclose(pFile);

  parseConfiguration(configFileName);
}

uint8_t expectToBeHexCharIdentifier() {
  if (token.type == TokenNumber) {
    if (token.number >= 0 && token.number <= 9)
      return (uint8_t)token.number;

    expectedError("hex key code");
  }

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
    return (char *)expectedError("key identifier");

  return token.value;
}

void expectToBeEquals() {
  if (token.type != TokenEquals)
    expectedError("=");
}

void expectToBeKey() {
  if (token.type != TokenKey)
    expectedError("one of KEY-, CTRL- or BTN-");
}

void expectToBeDash() {
  if (token.type != TokenDash)
    expectedError("-");
}

void expectToBeComma() {
  if (token.type != TokenComma)
    expectedError(",");
}

const ControllerDirection expectToBeDirection() {
  if (token.type != TokenDirection)
    return (ControllerDirection)expectedError("direction (UP, DOWN, LEFT, RIGHT, or BTN-<number>)");

  return token.number;
}

const ControllerDirection expectToBeSubDirection(ControllerDirection d) __z88dk_fastcall {
  if (d == CONTROLLER_DIRECTION_UP)
    switch (token.number) {
    case CONTROLLER_DIRECTION_RIGHT:
      return CONTROLLER_DIRECTION_UP_RIGHT;

    case CONTROLLER_DIRECTION_LEFT:
      return CONTROLLER_DIRECTION_UP_LEFT;
    }
  else if (d == CONTROLLER_DIRECTION_DOWN)
    switch (token.number) {
    case CONTROLLER_DIRECTION_RIGHT:
      return CONTROLLER_DIRECTION_DOWN_RIGHT;

    case CONTROLLER_DIRECTION_LEFT:
      return CONTROLLER_DIRECTION_DOWN_LEFT;
    }

  return (ControllerDirection)expectedError("UP-xxx or DOWN-xxx");
}

uint8_t expectToBeNumberUp(uint8_t upper) {
  if (token.type != TokenNumber)
    expectedError("number");

  const uint8_t n = (uint8_t)token.number;

  if (n > upper)
    expectedError("number too large");

  return n;
}

static void applyConfigColour() {
  getNextToken();
  expectToBeDash();

  getNextToken();
  const uint8_t b = expectToBeNumberUp(3);

  getNextToken();
  expectToBeEquals();

  getNextToken();

  if (token.type == TokenNumber) {
    // SET RGB FOR V9958
    const uint8_t red = expectToBeNumberUp(15);
    getNextToken();
    expectToBeComma();
    getNextToken();
    const uint8_t green = expectToBeNumberUp(15);
    getNextToken();
    expectToBeComma();
    getNextToken();
    const uint8_t blue = expectToBeNumberUp(15);

    palette[b].red = red;
    palette[b].green = green;
    palette[b].blue = blue;

    return;
  }

  gameColours[b] = token.type;
}

bool haveAppliedAKeyConfig = false;

static void applyConfigKey() {
  if (!haveAppliedAKeyConfig) {
    gameKeyCount = 0;
    haveAppliedAKeyConfig = true;
  }
  getNextToken();
  expectToBeDash();

  getNextToken();
  const uint8_t pSourceKey = expectToBeHexCharIdentifier();

  getNextToken();
  expectToBeEquals();

loop:
  getNextToken();

  if (token.type == TokenCtrl) {
    gameKeys[gameKeyCount].hexCode = pSourceKey;

    getNextToken();
    expectToBeDash();

    getNextToken();

    if (token.type == TokenNumber) {
      const uint8_t controllerId = expectToBeNumberUp(2);
      gameKeys[gameKeyCount].controllerId = controllerId - 1;

      getNextToken();
      expectToBeDash();
      getNextToken();
    } else
      gameKeys[gameKeyCount].controllerId = 0;

    if (token.type == TokenBtn) {
      getNextToken();
      expectToBeDash();

      getNextToken();
      const uint8_t direction = expectToBeNumberUp(3);

      gameKeys[gameKeyCount].type = KC_CTRL_BTNS;
      gameKeys[gameKeyCount].controllerButtons = direction;

      gameKeyCount++;
    } else {
      const uint8_t direction = expectToBeDirection();

      if (tokenTerminatorChar == '-') {
        getNextToken(); // Dash

        getNextToken(); // sub-direction
        const uint8_t subDirection = expectToBeSubDirection(direction);
        gameKeys[gameKeyCount].type = KC_CTRL_DIR;
        gameKeys[gameKeyCount].controllerDirection = subDirection;

        gameKeyCount++;
        return;
      }

      gameKeys[gameKeyCount].type = KC_CTRL_DIR;
      gameKeys[gameKeyCount].controllerDirection = direction;

      gameKeyCount++;
    }
  } else {
    expectToBeKey();

    getNextToken();
    expectToBeDash();

    getNextToken();
    const char *c = expectToBeKeyIdentifier();

    gameKeys[gameKeyCount].hexCode = pSourceKey;
    gameKeys[gameKeyCount].asciiKeyChar = c[0];
    gameKeys[gameKeyCount].type = KC_ASCII;
    gameKeyCount++;
  }

  if (tokenTerminatorChar == ',') {
    getNextToken();
    goto loop;
  }
}
