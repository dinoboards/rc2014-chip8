#include "chip8asm/error_reports.h"
#include "chip8asm/error.h"
#include "chip8asm/exit.h"
#include "chip8asm/tokenreader.h"

void expectedError(const char *msg) __z88dk_fastcall {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nExpected %s but found %s\r\n at line number %d\r\n", token.currentLine, msg, token.value, getCurrentLineNumber());
  errorExit();
}

void unknownLabelError(const char *label) __z88dk_fastcall {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nUnknown label '%s' referenced at line number %d\r\n", token.currentLine, label, getCurrentLineNumber());
  errorExit();
}

void errorBadExpression(const char *expression) __z88dk_fastcall {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nBad expression '%s' at line number %d\r\n", token.currentLine, expression, getCurrentLineNumber());
  errorExit();
}

void errorUnexpectedCharacter(char ch) __z88dk_fastcall {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nExpression had unexpected character '%c' at line number %d\r\n", token.currentLine, ch, getCurrentLineNumber());
  errorExit();
}

void errorExpectedChar(char ch) __z88dk_fastcall {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nExpression expected character '%c' at line number %d\r\n", token.currentLine, ch, getCurrentLineNumber());
  errorExit();
}

void errorUnexpectedContent() {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nUnexpected content after expression at line number %d\r\n", token.currentLine, getCurrentLineNumber());
  errorExit();
}

void errorExpressionTooLong() {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nExpression item too long at line number %d\r\n", token.currentLine, getCurrentLineNumber());
  errorExit();
}
