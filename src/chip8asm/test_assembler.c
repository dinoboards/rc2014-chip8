#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "cpm.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *content;

char getNextCharRaw() { return *content++; }

void openFileStream() {}

void closeFileStream() {}

#define MAX_MESSAGE_TEXT 256
char pbuffer[MAX_MESSAGE_TEXT];

void logError(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  vsnprintf(pbuffer, MAX_MESSAGE_TEXT - 1, (char *)msg, arg);
  va_end(arg);
  // print(pbuffer);
}

bool testFailure = false;
bool testErrored = false;

void errorExit() { testErrored = true; }

void shouldAssemble(const char *source, uint16_t expectedWord) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;

  initLabelStorage();
  assemble(1);

  xprintf("%s should assemble to %04X\r\n", source, expectedWord);

  if (testErrored) {
    xprintf("  Failed.  %s\r\n", pbuffer);
    testFailure = true;
    return;
  }

  if (programStorage[0] != (byte)(expectedWord >> 8) || programStorage[1] != (byte)(expectedWord & 0xFF)) {
    xprintf("   Failure: translated to %02X%02X\r\n", (int)programStorage[0], (int)programStorage[1]);
    testFailure = true;
  }
}

void shouldError(const char *source, const char *errorMessage) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;

  initLabelStorage();
  assemble(1);

  xprintf("%s should report error '%s'\r\n", source, errorMessage);

  if (testErrored) {
    if (strstr(pbuffer, errorMessage) != NULL)
      return;

    xprintf("  Failed: incorrect error message of %s\r\n", pbuffer);
    testFailure = true;
    return;
  }

  xprintf("  Failed.  no error message reported.\r\n");
}

void main() {
  shouldAssemble("LD V3, va", 0x83A0);
  shouldAssemble("LD V1, 10", 0x610A);
  shouldAssemble("LD I, 1234", 0xA4D2);
  shouldAssemble("DRW V2, V3, 11", 0xD23B);
  shouldAssemble("DB 2, 255", 0x02FF);
  shouldAssemble("CALL 1025", 0x2401);
  shouldAssemble("LABL: CALL LABL", 0x2200);
  shouldAssemble("CLS", 0x00E0);
  shouldAssemble("RET", 0x00EE);
  shouldAssemble("ADD VE, 1", 0x7E01);
  shouldAssemble("SE V4, 15", 0x340F);
  shouldAssemble("SE V4, v9", 0x5490);
  shouldError("BAD INSTRUCTION", "Expected Instruction but found BAD");

  xprintf(testFailure ? "Tests Failed\r\n" : "All Done\r\n");
}
