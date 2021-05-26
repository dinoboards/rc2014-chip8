#include "terminal_codes.h"
#include "tokenreader.h"

char *content;
bool  testErrored = false;

char getNextCharRaw() {
  const char b = *content;
  if (b)
    content++;
  return b;
}

void openFileStream() {}

void closeFileStream() {}

#define MAX_LOG_CAPTURE 256
char logBuffer[MAX_LOG_CAPTURE];

void logError(const char *msg, ...) {
  if (testErrored)
    return;

  va_list arg;
  va_start(arg, msg);
  vsnprintf(logBuffer, MAX_LOG_CAPTURE - 1, (char *)msg, arg);
  va_end(arg);
}

void errorExit() { testErrored = true; }

#define instructionEquals(loc, word) (programStorage[(loc)] != (byte)((word) >> 8) || programStorage[(loc) + 1] != (byte)((word)&0xFF))

void shouldAssemble(const char *source, uint16_t expectedWord) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;
  xbuffer[0] = '\0';
  logBuffer[0] = '\0';

  initLabelStorage();
  assemble(1);
  content = (char *)source;
  assemble(2);

  printf("%04X should be assembled from:\r\n  %s\r\n\r\n", expectedWord, source);

  //?????
  if (testErrored) {
    printf(RED "  Failed.  %s\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  if (instructionEquals(0, expectedWord)) {
    printf(RED "  Failure: translated to %02X%02X\r\n\r\n" RESET, (int)programStorage[0], (int)programStorage[1]);
    testFailure = true;
  }
}

void shouldAssembleDblWidth(const char *source, uint16_t expectedWord1, uint16_t expectedWord2) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;
  xbuffer[0] = '\0';
  logBuffer[0] = '\0';

  initLabelStorage();
  assemble(1);
  content = (char *)source;
  assemble(2);

  printf("%04X, %04X should be assembled from:\r\n  %s\r\n\r\n", expectedWord1, expectedWord2, source);

  //?????
  if (testErrored) {
    printf(RED "  Failed.  %s\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  if (instructionEquals(0, expectedWord1) && instructionEquals(2, expectedWord2)) {
    printf(RED "  Failure: translated to %02X%02X%02X%02X\r\n\r\n" RESET, (int)programStorage[0], (int)programStorage[1], (int)programStorage[2], (int)programStorage[3]);
    testFailure = true;
  }
}

void shouldAssembleDS(const char *source, uint16_t expectedPCCount) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;
  xbuffer[0] = '\0';
  logBuffer[0] = '\0';

  initLabelStorage();
  assemble(1);
  content = (char *)source;
  assemble(2);

  printf("%s should reserve %d bytes\r\n\r\n", source, expectedPCCount);

  if (testErrored) {
    printf(RED "  Failed.  %s\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  if (programPtr != (programStorage + expectedPCCount)) {
    printf(RED "  Failure: expected PC to be increment by %d - got %d\r\n\r\n" RESET, expectedPCCount, programPtr - programStorage);
    testFailure = true;
  }
}

void shouldError(const char *source, const char *errorMessage) {
  content = (char *)source;
  programStorage[0] = 0;
  programStorage[1] = 0;
  testErrored = false;
  logBuffer[0] = '\0';
  xbuffer[0] = '\0';

  initLabelStorage();
  assemble(1);
  content = (char *)source;
  assemble(2);

  printf("'%s' should be reported from:\r\n%s\r\n\r\n", errorMessage, source);

  if (testErrored) {
    if (strstr(logBuffer, errorMessage) != NULL)
      return;

    printf(RED "  Failed: incorrect error message of %s\r\n\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  printf(RED "  Failed.  no error message reported.\r\n" RESET);
}

void shouldEvaluate(const char *expression, int expectedValue) {
  testErrored = false;
  xbuffer[0] = '\0';
  logBuffer[0] = '\0';

  printf("%s should evaluate to %d\r\n", expression, expectedValue);
  int r = evaluate(expression);

  if (testErrored) {
    printf(RED "  Failed.  %s\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  expectEqualInts(r, expectedValue, "evaluation");
}

void shouldEvaluateError(const char *expression, const char *expectedErrorMessage) {
  testErrored = false;
  xbuffer[0] = '\0';
  logBuffer[0] = '\0';

  printf("%s should generate error of %s\r\n", expression, expectedErrorMessage);
  int r = evaluate(expression);
  expectEqualInts(r, 0, "evaluation");

  if (testErrored) {
    if (strstr(logBuffer, expectedErrorMessage) != NULL)
      return;

    printf(RED "  Failed: incorrect error message of %s\r\n\r\n" RESET, logBuffer);
    testFailure = true;
    return;
  }

  printf(RED "  Failed.  no error message reported.\r\n" RESET);
}

void setFileStream() {}
