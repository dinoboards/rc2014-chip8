#ifndef __TEST_SYSTEM
#define __TEST_SYSTEM

#include "datatypes.h"
#include "xstdio.h"

extern bool appRunning;

#define assert(a)                                  \
  {                                                \
    xprintf(#a "\r\n");                            \
    soundTimer = 0;                                \
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
    printf(#a "\r\n");                       \
    soundTimer = 0;                          \
    resetKeySimulator();                     \
    initSystemState();                       \
    setup_##a();                             \
    appRunning = executeSingleInstruction(); \
    verify_##a();                            \
    printf("\r\n");                          \
  }

#define MAX_CAPTURE_TEXT 128

#define KEY_ROW_A  2
#define KEY_MASK_A ((uint8_t)(1 << 6))
#define KEY_ROW_B  2
#define KEY_MASK_B ((uint8_t)(1 << 7))
#define KEY_ROW_2  0
#define KEY_MASK_2 ((uint8_t)(1 << 2))

extern void resetKeySimulator(void);
extern void simulateKey(const uint8_t, const uint8_t);
extern void simulateTimerTick(void);
extern void simulateRandomNumber(byte b);

#endif
