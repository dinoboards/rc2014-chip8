#include "test_configuration.h"
#include "configreader.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "systemstate.h"
#include "test_expectations.h"
#include "tms.h"

#include "xstdio.h"
// typedef _lastError {  } lastError;

bool unexpectedTokenInvoked;
bool expectedErrorInvoked;

void unexpectedEntry() {}
void expectedEqualSign() {}
void unexpectedToken() {
  xprintf("unexpected token type: %d, value: %s\r\n", token.type, token.value);
  unexpectedTokenInvoked = true;
}
void expectedError(const char *p) __z88dk_fastcall {
  (void)p;
  xprintf("expected error %s\r\n", p);
  expectedErrorInvoked = true;
}

char *pNextChar;
char *pConfigurationUnderTest;

void setFileStream(FCB *_fcb) { (void)_fcb; }
void openFileStream() { pNextChar = pConfigurationUnderTest; }
void closeFileStream() {}
char getNextCharRaw() { return *pNextChar++; }

#define assert(config)                                    \
  {                                                       \
    pConfigurationUnderTest = (char *)testConfig##config; \
    unexpectedTokenInvoked = false;                       \
    expectedErrorInvoked = false;                         \
    parseConfiguration(&fcb);                             \
    assert##config();                                     \
  }

const char *testConfigColours = "COLOR-0 = lightblue\r\n"
                                "COLOR-1 = black\r\n"
                                "COLOR-3 = cyan\r\n"
                                "\x1a";
void assertColours() {
  expectEqualBytes(gameColours[0], COL_LIGBLUE, "COLOR-0 = LIGHT BLUE");
  expectEqualBytes(gameColours[1], COL_BLACK, "COLOR-1 = BLACK");
  expectEqualBytes(gameColours[3], COL_CYAN, "COLOR-14 = CYAN");
}

const char *testConfigColoursBadIndex = "COLOR-6 = lightblue\r\n"
                                        "\x1a";
void assertColoursBadIndex() { expectTrue(expectedErrorInvoked, "expectedError"); }

const char *testConfigSingleKey = "KEY-1 = KEY-a\r\n"
                                  "\x1a";

void assertSingleKey() {
  expectEqualChars(gameKeys[1], 'a', "KEY-1");
  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigKeySpace = "KEy-a = KEY-SPACE\r\n"
                                 "\x1a";

void assertKeySpace() {
  expectEqualChars(gameKeys[10], ' ', "KEY-SPACE");
  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigKeyCr = "KEy-f = KEY-CR\r\n"
                              "\x1a";

void assertKeyCr() { expectEqualChars(gameKeys[15], 13, "KEY-CR"); }

void testConfigurtionParser() {
  FCB fcb;
  xprintf("Configuration Processing:\r\n");

  assert(Colours);
  assert(ColoursBadIndex);

  assert(SingleKey);
  assert(KeySpace);
  assert(KeyCr);

  xprintf(testFailure ? RED "Tests Failed\r\n\n" RESET : BRIGHT_WHITE "All Passed\r\n\n" RESET);
}
