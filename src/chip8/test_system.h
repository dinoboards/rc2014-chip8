#ifndef __TEST_SYSTEM
#define __TEST_SYSTEM

#include "datatypes.h"
#include "xstdio.h"

extern bool appRunning;

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
    printf(#a "\r\n");                      \
    soundTimer = 0;                          \
    resetCaptureCommands();                  \
    resetKeySimulator();                     \
    initSystemState();                       \
    setup_##a();                             \
    appRunning = executeSingleInstruction(); \
    verify_##a();                            \
    printf("\r\n");                         \
  }

#define MAX_CAPTURE_TEXT 128

extern void resetCaptureCommands();
extern void resetKeySimulator();
extern void simulateKey(const char k);
extern void simulateTimerTick();
extern void simulateRandomNumber(byte b);

#endif
