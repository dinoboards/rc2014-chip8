#include "chip8asm/error_reports.h"
#include "chip8asm/error.h"
#include "chip8asm/exit.h"
#include "chip8asm/tokenreader.h"

void expectedError(const char *msg) {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nExpected %s but found %s\r\n at line number %d\r\n", token.currentLine, msg, token.value, getCurrentLineNumber());
  errorExit();
}

void unknownLabelError(const char *label) {
  getToLineEnd();
  logError("\r\n%s\r\n\r\nUnknown label '%s' referenced at line number %d\r\n", token.currentLine, label, getCurrentLineNumber());
  errorExit();
}
