// Bare bones scanner and parser for the following LL(1) grammar:

#include "expr.h"
#include "chartesters.h"
#include "datatypes.h"
#include "error.h"
#include "error_reports.h"
#include "exit.h"
#include "labels.h"
#include "xstdlib.h"
#include <stdio.h>

#define MAX_WORKING_BUFFER 64
static char        token[MAX_WORKING_BUFFER];
static int         tokenIndex = 0;
static char        currentChar;
static const char *expressionPtr;

typedef enum { UNKNOWN, ADD_SUB_OP, MUL_DIV_REM_OP, OP_OR, AND_OP, LEFT_PAREN, RIGHT_PAREN, NUMBER, ALPHA, END_INPUT } LookAheadTokens;

static LookAheadTokens lookAhead;

void   scan();
number expr(void);

inline void reset() {
  tokenIndex = 0;
  token[0] = '\0';
}

inline void ignore() { currentChar = *expressionPtr++; }

number evaluate(const char *myexpression) {
  expressionPtr = (char *)myexpression;

  reset();
  ignore();
  scan();

  number val = expr();

  if (lookAhead != END_INPUT && lookAhead != '\0') {
    errorUnexpectedContent();
    return 0;
  }

  return val;
}

inline void abortEvaluation() {
  while (currentChar)
    ignore();
  lookAhead = END_INPUT;
}

void read() {
  if (tokenIndex >= MAX_WORKING_BUFFER) {
    errorExpressionTooLong();
    abortEvaluation();
    return;
  }

  token[tokenIndex++] = currentChar;
  token[tokenIndex] = '\0';
  currentChar = *expressionPtr++;
}

void scan() {
  reset();
START:
  switch (currentChar) {
  case ' ':
  case '\t':
  case '\r':
    ignore();
    goto START;

  case '-':
  case '+':
    read();
    lookAhead = ADD_SUB_OP;
    return;

  case '*':
  case '/':
  case '%':
    read();
    lookAhead = MUL_DIV_REM_OP;
    return;

  case '|':
    read();
    lookAhead = OP_OR;
    return;

  case '&':
    read();
    lookAhead = AND_OP;
    return;

  case '(':
    read();
    lookAhead = LEFT_PAREN;
    return;

  case ')':
    read();
    lookAhead = RIGHT_PAREN;
    return;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    read();
    goto IN_LEADING_DIGITS;

  case '\n':
  case '\0':
    currentChar = ' '; // delayed ignore()
    lookAhead = END_INPUT;
    return;

  default:
    if (isCharLetter(currentChar) || currentChar == '_') {
      read();
      goto IN_LEADING_ALPHA;
    }
    errorUnexpectedCharacter(currentChar);
    abortEvaluation();
    lookAhead = END_INPUT;
    return;
  }

IN_LEADING_DIGITS:
  if (currentChar >= '0' && currentChar < '9') {
    read();
    goto IN_LEADING_DIGITS;
  }
  lookAhead = NUMBER;
  return;

IN_LEADING_ALPHA:
  if (isCharAlpha(currentChar)) {
    read();
    goto IN_LEADING_ALPHA;
  } else {
    lookAhead = ALPHA;
    return;
  }
}

number unsigned_factor() {
  number result = 0;
  switch (lookAhead) {
  case NUMBER:
    result = xstrtol(token, (char *)0, 10);
    scan();
    break;

  case ALPHA:
    result = findLabelAddress(token);
    scan();
    break;

  case LEFT_PAREN:
    scan();
    result = expr();
    if (lookAhead != RIGHT_PAREN) {
      errorExpectedChar(')');
      abortEvaluation();
      return 0;
    }
    scan();
    break;

  default:
    errorBadExpression(token);
    abortEvaluation();
    return 0;
  }
  return result;
}

number factor() {
  if (lookAhead == ADD_SUB_OP && token[0] == '-') {
    scan();
    return -unsigned_factor();
  }
  return unsigned_factor();
}

number operatorMulDivRem() {
  number result = factor();
  while (lookAhead == MUL_DIV_REM_OP) {
    switch (token[0]) {
    case '*':
      scan();
      result *= factor();
      break;

    case '/':
      scan();
      result /= factor();
      break;

    case '%':
      scan();
      result %= factor();
      break;
    }
  }
  return result;
}

number operatorAddSub() {
  number result = operatorMulDivRem();
  while (lookAhead == ADD_SUB_OP) {
    switch (token[0]) {
    case '+':
      scan();
      result += operatorMulDivRem();
      break;

    case '-':
      scan();
      result -= operatorMulDivRem();
      break;
    }
  }
  return result;
}

number operatorAnd() {
  number result = operatorAddSub();
  while (lookAhead == AND_OP) {
    switch (token[0]) {
    case '&':
      scan();
      result &= operatorAddSub();
      break;
    }
  }
  return result;
}

number expr() { // Operator |
  number result = operatorAnd();
  while (lookAhead == OP_OR) {
    switch (token[0]) {
    case '|':
      scan();
      result |= operatorAnd();
      break;
    }
  }
  return result;
}
