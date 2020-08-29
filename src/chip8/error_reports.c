#include "error_reports.h"
#include "configreader.h"
#include "xstdio.h"
#include <stdlib.h>

void expectedEqualSign() {
  getToLineEnd();
  xprintf("\r\n%s\r\n\r\nExpected '=' but found %s\r\n at line number %d\r\n", token.currentLine, token.value, getCurrentLineNumber());
  exit(1);
}

void unexpectedToken() {
  getToLineEnd();
  xprintf("\r\n%s\r\nUnexpected token '%c' found\r\n at line number %d\r\n", token.currentLine, token.currentChar, getCurrentLineNumber());
  exit(1);
}

void unexpectedEntry() {
  getToLineEnd();
  xprintf("\r\n%s\r\nBad configuration found\r\n at line number %d\r\n", token.currentLine, lineNumber);
  exit(1);
}

void expectedColour() {
  getToLineEnd();
  xprintf("\r\n%s\r\nExpected a colour reference but found '%s'\r\n at line number %d\r\n", token.currentLine, token.value, lineNumber);
  exit(1);
}
