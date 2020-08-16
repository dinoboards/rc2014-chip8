// Bare bones scanner and parser for the following LL(1) grammar:
// expr -> term { [+-] term }     ; An expression is terms separated by add ops.
// term -> factor { [*/] factor } ; A term is factors separated by mul ops.
// factor -> unsigned_factor      ; A signed factor is a factor,
//         | - unsigned_factor    ;   possibly with leading minus sign
// unsigned_factor -> ( expr )    ; An unsigned factor is a parenthesized expression
//         | NUMBER               ;   or a number
//
// The parser returns the floating point value of the expression.

#include "expr.h"
#include "chartesters.h"
#include "datatypes.h"
#include "error.h"
#include "error_reports.h"
#include "exit.h"
#include "labels.h"
#include "xstdio.h"
#include <stdio.h>

#define MAX_WORKING_BUFFER 64
static char        token[MAX_WORKING_BUFFER];
static int         tokenIndex = 0;
static int         ch;
static const char *expressionPtr;

enum { ADD_OP, MUL_OP, LEFT_PAREN, RIGHT_PAREN, NUMBER, ALPHA, END_INPUT } lookAhead;

int    scan();
number expr(void);

// inline void advance() { scan(); }

inline void reset() {
  tokenIndex = 0;
  token[0] = '\0';
}

inline void ignore() { ch = *expressionPtr++; }

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
  while (ch)
    ignore();
  lookAhead = END_INPUT;
}

void read() {
  if (tokenIndex >= MAX_WORKING_BUFFER) {
    errorExpressionTooLong();
    abortEvaluation();
    return;
  }

  token[tokenIndex++] = ch;
  token[tokenIndex] = '\0';
  ch = *expressionPtr++;
}

int scan() {
  reset();
START:
  switch (ch) {
  case ' ':
  case '\t':
  case '\r':
    ignore();
    goto START;

  case '-':
  case '+':
    read();
    return lookAhead = ADD_OP;

  case '*':
  case '/':
    read();
    return lookAhead = MUL_OP;

  case '(':
    read();
    return lookAhead = LEFT_PAREN;

  case ')':
    read();
    return lookAhead = RIGHT_PAREN;

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
    ch = ' '; // delayed ignore()
    return lookAhead = END_INPUT;

  default:
    if (isCharLetter(ch) || ch == '_') {
      read();
      goto IN_LEADING_ALPHA;
    }
    errorUnexpectedCharacter(ch);
    abortEvaluation();
    return lookAhead = END_INPUT;
  }

IN_LEADING_DIGITS:
  switch (ch) {
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

  case '.':
    read();
    goto IN_TRAILING_DIGITS;

  default:
    return lookAhead = NUMBER;
  }

IN_LEADING_ALPHA:
  if (isCharAlpha(ch)) {
    read();
    goto IN_LEADING_ALPHA;
  } else
    return lookAhead = ALPHA;

IN_TRAILING_DIGITS:
  switch (ch) {
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
    goto IN_TRAILING_DIGITS;

  default:
    return lookAhead = NUMBER;
  }
}

number unsigned_factor() {
  number rtn = 0;
  switch (lookAhead) {
  case NUMBER:
    sscanf(token, "%d", &rtn);
    scan();
    break;

  case ALPHA:
    rtn = findLabelAddress(token);
    scan();
    break;

  case LEFT_PAREN:
    scan();
    rtn = expr();
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
  return rtn;
}

number factor() {
  number rtn = 0;
  // If there is a leading minus...
  if (lookAhead == ADD_OP && token[0] == '-') {
    scan();
    rtn = -unsigned_factor();
  } else
    rtn = unsigned_factor();
  return rtn;
}

number term() {
  number rtn = factor();
  while (lookAhead == MUL_OP) {
    switch (token[0]) {
    case '*':
      scan();
      rtn *= factor();
      break;

    case '/':
      scan();
      rtn /= factor();
      break;
    }
  }
  return rtn;
}

number expr() {
  number rtn = term();
  while (lookAhead == ADD_OP) {
    switch (token[0]) {
    case '+':
      scan();
      rtn += term();
      break;

    case '-':
      scan();
      rtn -= term();
      break;
    }
  }
  return rtn;
}
