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
#include "labels.h"
#include "xstdio.h"
#include <stdio.h>

// The token buffer. We never check for overflow! Do so in production code.
char buf[1024];
int  n = 0;

// The current character.
int ch;

// The look-ahead token.  This is the 1 in LL(1).
enum { ADD_OP, MUL_OP, LEFT_PAREN, RIGHT_PAREN, NUMBER, ALPHA, END_INPUT } look_ahead;

// Forward declarations.
void   init(void);
void   advance(void);
number expr(void);
void   error(const char *msg) __z88dk_fastcall;

// const char *myexpression = "1+2*4+bob\r\n\0\0\0\0";
char *expressionPtr;

// Parse expressions, one per line.
number evaluate(const char *myexpression) {
  expressionPtr = (char *)myexpression;
  init();

  number val = expr();

  if (look_ahead != END_INPUT && look_ahead != '\0') {
    xtracef("  %02X %s\r\n", (int)look_ahead, look_ahead);
    error("junk after expression");
  }

  return val;
}

// Just die on any error.
void error(const char *msg) __z88dk_fastcall {
  xprintf("Error: %s. I quit.\r\n", msg);
  // exit(1);
}

// Buffer the current character and read a new one.
void read() {
  buf[n++] = ch;
  buf[n] = '\0'; // Terminate the string.
  // xtracef("read  %c %p\r\n", *expressionPtr, expressionPtr);

  ch = *expressionPtr++;
}

// Ignore the current character.
void ignore() {
  // xtracef("read  %c %p\r\n", *expressionPtr, expressionPtr);
  ch = *expressionPtr++;
}

// Reset the token buffer.
void reset() {
  n = 0;
  buf[0] = '\0';
}

// The scanner.  A tiny deterministic finite automaton.
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
    return ADD_OP;

  case '*':
  case '/':
    read();
    return MUL_OP;

  case '(':
    read();
    return LEFT_PAREN;

  case ')':
    read();
    return RIGHT_PAREN;

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
    return END_INPUT;

  default:
    if (isCharLetter(ch) || ch == '_') {
      read();
      goto IN_LEADING_ALPHA;
    }
    error("bad character");
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
    return NUMBER;
  }

IN_LEADING_ALPHA:
  if (isCharAlpha(ch)) {
    read();
    goto IN_LEADING_ALPHA;
  } else
    return ALPHA;

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
    return NUMBER;
  }
}

// To advance is just to replace the look-ahead.
void advance() { look_ahead = scan(); }

// Clear the token buffer and read the first look-ahead.
void init() {
  reset();
  ignore(); // junk current character
  advance();
}

number unsigned_factor() {
  number rtn = 0;
  switch (look_ahead) {
  case NUMBER:
    sscanf(buf, "%d", &rtn);
    advance();
    break;

  case ALPHA:
    // xtracef("ALPHA: %s\r\n", buf);
    rtn = findLabelAddress(buf);
    // xtracef("= %d\r\n", rtn);
    advance();
    break;

  case LEFT_PAREN:
    advance();
    rtn = expr();
    if (look_ahead != RIGHT_PAREN)
      error("missing ')'");
    advance();
    break;

  default:
    error("unexpected token");
  }
  return rtn;
}

number factor() {
  number rtn = 0;
  // If there is a leading minus...
  if (look_ahead == ADD_OP && buf[0] == '-') {
    advance();
    rtn = -unsigned_factor();
  } else
    rtn = unsigned_factor();
  return rtn;
}

number term() {
  number rtn = factor();
  while (look_ahead == MUL_OP) {
    switch (buf[0]) {
    case '*':
      advance();
      rtn *= factor();
      break;

    case '/':
      advance();
      rtn /= factor();
      break;
    }
  }
  return rtn;
}

number expr() {
  number rtn = term();
  while (look_ahead == ADD_OP) {
    switch (buf[0]) {
    case '+':
      advance();
      rtn += term();
      break;

    case '-':
      advance();
      rtn -= term();
      break;
    }
  }
  return rtn;
}
