#include "error_reports.h"
#include "configreader.h"
#include "xstdio.h"
#include <stdio.h>
#include <stdlib.h>

void expectedEqualSign() {
  getToLineEnd();
  printf("\r\n%s\r\n\r\nExpected '=' but found %s\r\n at line number %d\r\n", token.currentLine, token.value, currentLineNumber);
  exit(1);
}

void unexpectedToken() {
  getToLineEnd();
  printf("\r\n%s\r\nUnexpected token '%c' found\r\n at line number %d\r\n", token.currentLine, tokenCurrentChar, currentLineNumber);
  exit(1);
}

void unexpectedEntry() {
  getToLineEnd();
  printf("\r\n%s\r\nBad configuration found\r\n at line number %d\r\n", token.currentLine, currentLineNumber);
  exit(1);
}

void expectedColour() {
  getToLineEnd();
  printf("\r\n%s\r\nExpected a colour reference but found '%s'\r\n at line number %d\r\n", token.currentLine, token.value, currentLineNumber);
  exit(1);
}

void tmsDriverNotFound() {
  printf("TMS driver not found\r\n");
  exit(1);
}

void warnNoTimerFound() { printf("Warning: No timer found.\r\n"); }

uint16_t expectedError(const char *p) __z88dk_fastcall {
  getToLineEnd();
  printf("\r\n%s\r\n\r\nExpected '%s' but found %s\r\n at line number %d\r\n", p, token.currentLine, token.value, currentLineNumber);
  exit(1);
  return 0;
}
