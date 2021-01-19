#include "test_configuration.h"
#include "configreader.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "systemstate.h"
#include "test_expectations.h"
#include "tms.h"

#include "xstdio.h"
// typedef _lastError {  } lastError;

void unexpectedEntry() {}
void expectedEqualSign() {}
void unexpectedToken() {}

const char *colourConfiguration = "COLOR0 = lightblue\r\n"
                                  "COLOR1 = black\r\n"
                                  "\x1a";

char *pNextChar;
char *pConfigurationUnderTest;

extern void setFileStream(FCB *_fcb) { (void)_fcb; }
extern void openFileStream() { pNextChar = pConfigurationUnderTest; }

extern void closeFileStream() {}

extern char getNextCharRaw() { return *pNextChar++; }

void testConfigurtionParser() {
  pConfigurationUnderTest = (char *)colourConfiguration;

  FCB fcb;

  parseConfiguration(&fcb);

  xprintf("Configuration Processing:\r\n");

  expectEqualBytes(gameColours[0], COL_LIGBLUE, "COLOR0 = LIGHT BLUE");
  expectEqualBytes(gameColours[1], COL_BLACK, "COLOR1 = BLACK");

  xprintf(testFailure ? RED "Tests Failed\r\n\n" RESET : BRIGHT_WHITE "All Passed\r\n\n" RESET);
}
