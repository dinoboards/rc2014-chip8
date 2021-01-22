#include "test_configuration.h"
#include "configreader.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "systemstate.h"
#include "test_expectations.h"
#include "tms.h"

#include "xstdio.h"

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
  xprintf("expected error %s. type: %d, value: %s\r\n", p, token.type, token.value);
  expectedErrorInvoked = true;
}

char *pNextChar;
char *pConfigurationUnderTest;

void setFileStream(FCB *_fcb) { (void)_fcb; }
void openFileStream() { pNextChar = pConfigurationUnderTest; }
void closeFileStream() {}
char getNextCharRaw() { return *pNextChar++; }

extern bool haveAppliedAKeyConfig;

#define assert(config)                                    \
  {                                                       \
    haveAppliedAKeyConfig = 0;                            \
    pConfigurationUnderTest = (char *)testConfig##config; \
    unexpectedTokenInvoked = false;                       \
    expectedErrorInvoked = false;                         \
    xprintf(#config ": \r\n");                            \
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
  expectEqualChars(gameKeys[0].asciiKeyChar, 'a', "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[0].hexCode, 1, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_ASCII, "gameKeys[0].type");
  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigTwoMappedKeys = "KEY-1 = KEY-a\r\n"
                                      "KEY-a = KEY-z\r\n"
                                      "\x1a";

void assertTwoMappedKeys() {
  expectEqualChars(gameKeys[0].asciiKeyChar, 'a', "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[0].hexCode, 1, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_ASCII, "gameKeys[0].type");

  expectEqualChars(gameKeys[1].asciiKeyChar, 'z', "gameKeys[1].asciiKeyChar");
  expectEqualBytes(gameKeys[1].hexCode, 10, "gameKeys[1].hexCode");
  expectEqualBytes(gameKeys[1].type, KC_ASCII, "gameKeys[1].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigControllerDirection = "KEY-2 = CTRL-UP\r\n"
                                            "\x1a";

void assertControllerDirection() {
  expectEqualBytes(gameKeys[0].controllerDirection, CONTROLLER_DIRECTION_UP, "gameKeys[0].controllerDirection");
  expectEqualBytes(gameKeys[0].hexCode, 2, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_CTRL_DIR, "gameKeys[0].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigMultipleKeyMapping = "KEY-1 = KEY-CR,CTRL-DOWN\r\n"
                                           "KEY-2 = KEY-SPACE\r\n"
                                           "\x1a";

void assertMultipleKeyMapping() {
  expectEqualBytes(gameKeys[0].asciiKeyChar, 13, "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[0].hexCode, 1, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_ASCII, "gameKeys[0].type");

  expectEqualBytes(gameKeys[1].controllerDirection, CONTROLLER_DIRECTION_DOWN, "gameKeys[1].controllerDirection");
  expectEqualBytes(gameKeys[1].hexCode, 1, "gameKeys[1].hexCode");
  expectEqualBytes(gameKeys[1].type, KC_CTRL_DIR, "gameKeys[1].type");

  expectEqualBytes(gameKeys[2].asciiKeyChar, ' ', "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[2].hexCode, 2, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[2].type, KC_ASCII, "gameKeys[0].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigControllerMultipleDirection = "KEY-2 = CTRL-UP-RIGHT\r\n"
                                                    "\x1a";

void assertControllerMultipleDirection() {
  expectEqualBytes(gameKeys[0].controllerDirection, CONTROLLER_DIRECTION_UP_RIGHT, "gameKeys[0].controllerDirection");
  expectEqualBytes(gameKeys[0].hexCode, 2, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_CTRL_DIR, "gameKeys[0].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigKeySpace = "KEy-f = KEY-SPACE\r\n"
                                 "\x1a";

void assertKeySpace() {
  expectEqualChars(gameKeys[0].asciiKeyChar, ' ', "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[0].hexCode, 15, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_ASCII, "gameKeys[0].type");
  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigControllerSubDirection = "KEY-1 = CTRL-UP-LEFT\r\n"
                                               "\x1a";

void assertControllerSubDirection() {
  expectEqualBytes(gameKeys[0].controllerDirection, CONTROLLER_DIRECTION_UP_LEFT, "gameKeys[0].controllerDirection");
  expectEqualBytes(gameKeys[0].hexCode, 1, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_CTRL_DIR, "gameKeys[0].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigControllerButtons = "KEY-1 = CTRL-UP-LEFT\r\n"
                                          "KEY-2 = CTRL-BTN-1\r\n"
                                          "\x1a";

void assertControllerButtons() {
  expectEqualBytes(gameKeys[0].controllerDirection, CONTROLLER_DIRECTION_UP_LEFT, "gameKeys[0].controllerDirection");
  expectEqualBytes(gameKeys[0].hexCode, 1, "gameKeys[0].hexCode");
  expectEqualChars(gameKeys[0].type, KC_CTRL_DIR, "gameKeys[0].type");

  expectTrue(gameKeys[1].controllerButton1, "gameKeys[1].controllerButton1");
  expectFalse(gameKeys[1].controllerButton2, "gameKeys[1].controllerButton2");
  expectEqualBytes(gameKeys[1].hexCode, 2, "gameKeys[1].hexCode");
  expectEqualBytes(gameKeys[1].type, KC_CTRL_BTNS, "gameKeys[1].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

const char *testConfigInvadersConfiguration = "KEY-4 = KEY-A,CTRL-LEFT\r\n"
                                              "KEY-5 = KEY-SPACE,CTRL-BTN-1,CTRL-BTN-2\r\n"
                                              "KEY-6 = KEY-D,CTRL-RIGHT\r\n"
                                              "\x1a";

void assertInvadersConfiguration() {
  expectEqualChars(gameKeys[0].asciiKeyChar, 'a', "gameKeys[0].asciiKeyChar");
  expectEqualBytes(gameKeys[0].hexCode, 4, "gameKeys[0].hexCode");
  expectEqualBytes(gameKeys[0].type, KC_ASCII, "gameKeys[0].type");

  expectEqualBytes(gameKeys[1].controllerDirection, CONTROLLER_DIRECTION_LEFT, "gameKeys[1].controllerDirection");
  expectEqualBytes(gameKeys[1].hexCode, 4, "gameKeys[1].hexCode");
  expectEqualBytes(gameKeys[1].type, KC_CTRL_DIR, "gameKeys[1].type");

  expectEqualChars(gameKeys[2].asciiKeyChar, ' ', "gameKeys[2].asciiKeyChar");
  expectEqualBytes(gameKeys[2].hexCode, 5, "gameKeys[2].hexCode");
  expectEqualBytes(gameKeys[2].type, KC_ASCII, "gameKeys[2].type");

  expectTrue(gameKeys[3].controllerButton1, "gameKeys[3].controllerButton1");
  expectFalse(gameKeys[3].controllerButton2, "gameKeys[3].controllerButton2");
  expectEqualBytes(gameKeys[3].hexCode, 5, "gameKeys[3].hexCode");
  expectEqualBytes(gameKeys[3].type, KC_CTRL_BTNS, "gameKeys[3].type");

  expectFalse(gameKeys[4].controllerButton1, "gameKeys[4].controllerButton1");
  expectTrue(gameKeys[4].controllerButton2, "gameKeys[4].controllerButton2");
  expectEqualBytes(gameKeys[4].hexCode, 5, "gameKeys[4].hexCode");
  expectEqualBytes(gameKeys[4].type, KC_CTRL_BTNS, "gameKeys[4].type");

  expectEqualChars(gameKeys[5].asciiKeyChar, 'd', "gameKeys[5].asciiKeyChar");
  expectEqualBytes(gameKeys[5].hexCode, 6, "gameKeys[5].hexCode");
  expectEqualBytes(gameKeys[5].type, KC_ASCII, "gameKeys[5].type");

  expectEqualBytes(gameKeys[6].controllerDirection, CONTROLLER_DIRECTION_RIGHT, "gameKeys[6].controllerDirection");
  expectEqualBytes(gameKeys[6].hexCode, 6, "gameKeys[6].hexCode");
  expectEqualBytes(gameKeys[6].type, KC_CTRL_DIR, "gameKeys[6].type");

  expectFalse(unexpectedTokenInvoked, "unexpectedToken");
  expectFalse(expectedErrorInvoked, "expectedError");
}

void testConfigurtionParser() {
  FCB fcb;
  xprintf("Configuration Processing:\r\n");

  assert(Colours);
  assert(ColoursBadIndex);

  assert(SingleKey);
  assert(TwoMappedKeys);
  assert(ControllerDirection);
  assert(ControllerMultipleDirection);
  assert(MultipleKeyMapping);
  assert(KeySpace);
  assert(ControllerSubDirection);
  assert(ControllerButtons);
  assert(InvadersConfiguration);

  xprintf(testFailure ? RED "Tests Failed\r\n\n" RESET : BRIGHT_WHITE "All Passed\r\n\n" RESET);
}
