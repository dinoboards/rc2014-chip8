#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "cpm.h"
#include "test_opcodes.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *content;

char getNextCharRaw() { return *content++; }

void openFileStream() {}

void closeFileStream() {}

#define MAX_LOG_CAPTURE 256
char logBuffer[MAX_LOG_CAPTURE];

void logError(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  vsnprintf(logBuffer, MAX_LOG_CAPTURE - 1, (char *)msg, arg);
  va_end(arg);
}

bool testFailure = false;
bool testErrored = false;

void errorExit() { testErrored = true; }

void shouldAssemble(const char *source, uint16_t expectedWord) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;
  xbuffer[0] = '\0';

  initLabelStorage();
  assemble(1);
  content = (char *)source;
  assemble(2);

  xprintf("%04X should be assembled from:\r\n  %s\r\n\r\n", expectedWord, source);

  if (testErrored) {
    xprintf("  Failed.  %s\r\n", logBuffer);
    testFailure = true;
    return;
  }

  if (programStorage[0] != (byte)(expectedWord >> 8) || programStorage[1] != (byte)(expectedWord & 0xFF)) {
    xprintf("  Failure: translated to %02X%02X\r\n\r\n", (int)programStorage[0], (int)programStorage[1]);
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
  content = (char *)source;
  assemble(2);

  xprintf("'%s' should be reported from:\r\n%s\r\n\r\n", errorMessage, source);

  if (testErrored) {
    if (strstr(xbuffer, errorMessage) != NULL)
      return;

    xprintf("  Failed: incorrect error message of %s\r\n\r\n", xbuffer);
    testFailure = true;
    return;
  }

  xprintf("  Failed.  no error message reported.\r\n");
}

void main() {
  shouldAssemble("LD V3, va", LD_V3_VA);
  shouldAssemble("LD V1, 10", LD_V1_10);
  shouldAssemble("LD I, 1234", LD_I_1234);
  shouldAssemble("DRW V2, V3, 11", DRAW_V2_V3_11);
  shouldAssemble("DRW V2, V3, 1", DRAW_V2_V3_1);
  shouldAssemble("DB 2, 255", 0x02FF);
  shouldAssemble("CALL 1025", CALL_1025);
  shouldAssemble("CALL LABL\r\n  LABL: RET", 0x2202);
  shouldAssemble("CLS", CLS);
  shouldAssemble("RET", RET);
  shouldAssemble("ADD VE, 1", ADD_VE_1);
  shouldAssemble("SE V4, 15", SE_V4_15);
  shouldAssemble("SE V4, v9", SE_V4_V9);
  shouldAssemble("JP 1026", JP_1026);
  shouldError("BAD INSTRUCTION", "Expected Instruction but found BAD");

  xprintf(testFailure ? "Tests Failed\r\n" : "All Done\r\n");
}
