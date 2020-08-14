#include "token_parser.h"
#include "datatypes.h"
#include "error_reports.h"
#include "expr.h"
#include "hbios.h"
#include "tokenreader.h"
#include "xstdio.h"
#include <string.h>

static inline byte getRegisterIndex(TokenType c) { return (byte)c - RegisterV0; }

byte expectToBeByte() {
  int x = evaluate(token.value);

  if (x < 0 || x > 255)
    expectedError("byte");

  return (byte)x;
}

uint16_t expectToBeInt() {
  uint16_t x = evaluate(token.value);

  if (x > 4095) {
    xprintf("Expected a expression upto 4095.  Number out of range\r\n");
    exit(1);
  }

  return x;
}

void expectToBeIRegister() {
  if (!currentIsIRegister()) {
    expectedError("I");
  }
}

byte expectToBeVRegister() {
  if (!currentIsVRegister()) {
    expectedError("Vx");
  }

  return getRegisterIndex(token.type);
}

void expectToBeComma() {
  if (strcmp(token.value, ",") != 0) {
    expectedError(",");
  }
}

byte expectToBeNibble() {
  int x = evaluate(token.value);

  if (x < 0 || x > 15) {
    xprintf("Expected a nibble.  Number out of range\r\n");
    exit(1);
  }

  return (byte)x;
}
