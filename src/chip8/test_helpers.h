#include "keys.h"
#include "test_expectations.h"

bool appRunning = false;

#define assert(a)                                  \
  {                                                \
    xprintf(#a "\r\n");                            \
    soundTimer = 0;                                \
    resetCaptureCommands();                        \
    resetKeySimulator();                           \
    initSystemState();                             \
    setup_##a();                                   \
    appRunning = executeSingleInstruction();       \
    expectEqualBytes(appRunning, 1, "appRunning"); \
    verify_##a();                                  \
    xprintf("\r\n");                               \
  }

#define assertTerminates(a)                  \
  {                                          \
    xprintf(#a "\r\n");                      \
    soundTimer = 0;                          \
    resetCaptureCommands();                  \
    resetKeySimulator();                     \
    initSystemState();                       \
    setup_##a();                             \
    appRunning = executeSingleInstruction(); \
    verify_##a();                            \
    xprintf("\r\n");                         \
  }

#define MAX_CAPTURE_TEXT 256 * 10
char buffer[MAX_CAPTURE_TEXT];

void resetCaptureCommands() { buffer[0] = '\0'; }

void sendDrawCommands(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  char *pBuffer = buffer + strlen(buffer);
  int   max = MAX_CAPTURE_TEXT - strlen(buffer);
  vsnprintf(pBuffer, max - 1, (char *)msg, arg);
  va_end(arg);
}

bool simulateKeyReady;
char simulatedKeyValue;

inline void resetKeySimulator() {
  simulateKeyReady = false;
  simulatedKeyValue = 0;
  currentPressedKey = '\0';
  keyPressed = false;
  currentKeyTimeout = 0;
}

byte keyReady() { return simulateKeyReady; }

char getKey() { return simulatedKeyValue; }

uint16_t simulatedNextTimerTick = 0;

inline void simulateKey(const char k) {
  simulateKeyReady = true;
  simulatedKeyValue = k;
  simulatedNextTimerTick += 4;
  timerSkipFlag = 255;
}

uint16_t getSysTimer() { return simulatedNextTimerTick; }

inline void simulateTimerTick() {
  simulatedNextTimerTick++;
  timerSkipFlag = 255;
  manageTimers();
}

byte nextRandomNumber = 0;

byte chip8Rand() { return nextRandomNumber; }

inline void simulateRandomNumber(byte b) { nextRandomNumber = b; }

void updateMachineStateDisplay() {}

void tmsDriverNotFound() {}
