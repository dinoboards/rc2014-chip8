#include "timers.h"
#include "instr_sound.h"
#include "msx.h"
#include "systemstate.h"
#include "systimer.h"

#ifdef _MEASURE_PERFORMANCE
#include "xstdio.h"
#endif

uint8_t lastTimerTick;

#ifdef _MEASURE_PERFORMANCE
uint16_t performanceMeasureTick = 0;
uint16_t instructionCount = 0;
#endif

void initTimers(void) {
  manageTimers();
  lastTimerTick = (uint8_t)getSysTimer();
}

#ifdef _MEASURE_PERFORMANCE
void reportPerformance(void) {
  instructionCount++;

  if (JIFFY >= performanceMeasureTick) {
    printf("Count: '%d'\r\n", (instructionCount / 2));
    instructionCount = 0;
    performanceMeasureTick = JIFFY + 120;
  }
}
#endif
