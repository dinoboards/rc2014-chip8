#include "error_reports.h"
#include "configreader.h"
#include "xstdio.h"
#include <stdlib.h>

void expectedEqualSign() {
  getToLineEnd();
  xprintf("\r\n%s\r\n\r\nExpected '=' but found %s\r\n at line number %d\r\n", token.currentLine, token.value, currentLineNumber);
  exit(1);
}

void unexpectedToken() {
  getToLineEnd();
  xprintf("\r\n%s\r\nUnexpected token '%c' found\r\n at line number %d\r\n", token.currentLine, tokenCurrentChar, currentLineNumber);
  exit(1);
}

void unexpectedEntry() {
  getToLineEnd();
  xprintf("\r\n%s\r\nBad configuration found\r\n at line number %d\r\n", token.currentLine, currentLineNumber);
  exit(1);
}

void expectedColour() {
  getToLineEnd();
  xprintf("\r\n%s\r\nExpected a colour reference but found '%s'\r\n at line number %d\r\n", token.currentLine, token.value, currentLineNumber);
  exit(1);
}

void tmsDriverNotFound() {
  xprintf("TMS driver not found\r\n");
  exit(1);
}

void warnNoTimerFound() { xprintf("Warning: No timer found.\r\n"); }

uint16_t expectedError(const char *p) __z88dk_fastcall {
  getToLineEnd();
  xprintf("\r\n%s\r\n\r\nExpected '%s' but found %s\r\n at line number %d\r\n", p, token.currentLine, token.value, currentLineNumber);
  exit(1);
  return 0;
}
