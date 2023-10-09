#include "token_parser.h"
#include "datatypes.h"
#include "error_reports.h"
#include "expr.h"
#include "tokenreader.h"
#include <stdio.h>

static inline byte getRegisterIndex(TokenType c) { return (byte)c - RegisterV0; }

byte expectToBeByte(void) {
  int x = evaluate(token.value);

  if (x < 0 || x > 255)
    expectedError("byte");

  return (byte)x;
}

uint16_t expectToBeInt16(void) { return evaluate(token.value); }

uint16_t expectToBeInt(void) {
  uint16_t x = evaluate(token.value);

  if (x > 4095) {
    errorIntTooLarge(x);
    return 0;
  }

  return x;
}

void expectToBeIRegister(void) {
  if (!currentIsIRegister()) {
    expectedError("I");
  }
}

byte expectToBeVRegister(void) {
  if (!currentIsVRegister()) {
    expectedError("Vx");
  }

  return getRegisterIndex(token.type);
}

void expectToBeIndexedI(void) {
  if (!currentIsIndexedI())
    expectedError("[I]");
}

void expectToBeOneOfVxOrIOrStOrDt(void) {
  if (currentIsVRegister())
    return;

  if (currentIsIRegister())
    return;

  if (currentIsST())
    return;

  if (currentIsDT())
    return;

  expectedError("one of Vx, I, ST or DT");
}

void expectToBeOneOfVxOrIOrIndexedIOrStOrDt(void) {
  if (currentIsVRegister())
    return;

  if (currentIsIRegister())
    return;

  if (currentIsST())
    return;

  if (currentIsDT())
    return;

  if (currentIsIndexedI())
    return;

  expectedError("one of Vx, I, [I], ST or DT");
}

void expectToBeVxOrIRegister(void) {
  if (currentIsVRegister())
    return;

  if (currentIsIRegister())
    return;

  expectedError("one of Vx or I");
}

void expectToBeST(void) {
  if (!currentIsST())
    expectedError("ST");
}

void expectToBeDT(void) {
  if (!currentIsDT())
    expectedError("DT");
}

void expectToBeComma(void) {
  if (!currentIsComma())
    expectedError(",");
}

byte expectToBeNibble(void) {
  int x = evaluate(token.value);

  if (x < 0 || x > 15) {
    printf("Expected a nibble.  Number out of range\r\n");
    exit(1);
  }

  return (byte)x;
}

void expectRangeOperator(void) {
  if (!currentIsRangeOperator())
    expectedError("..");
}

void expectToBeDown(void) {
  if (!currentIsDown())
    expectedError("one of DOWN, UP, LEFT or RIGHT");
}

void expectToBeUp(void) {
  if (!currentIsUp())
    expectedError("one of DOWN, UP, LEFT or RIGHT");
}

void expectToBeLeft(void) {
  if (!currentIsLeft())
    expectedError("one of DOWN, UP, LEFT or RIGHT");
}

void expectToBeRight(void) {
  if (!currentIsRight())
    expectedError("one of DOWN, UP, LEFT or RIGHT");
}
