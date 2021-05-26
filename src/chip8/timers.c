#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "instr_sound.h"
#include "systemstate.h"
#include "systimer.h"

#ifdef _MEASURE_PERFORMANCE
#include "xstdio.h"
#endif

uint16_t lastTimerTick;

#ifdef _MEASURE_PERFORMANCE
uint16_t performanceMeasureTick = 0;
uint16_t instructionCount = 0;
#endif

void initTimers() {
  manageTimers();
  lastTimerTick = getSysTimer();
}

#ifdef _MEASURE_PERFORMANCE
void reportPerformance() {
  instructionCount++;

  if (timerTick >= performanceMeasureTick) {
    printf("Count: '%d'\r\n", (instructionCount / 2));
    instructionCount = 0;
    performanceMeasureTick = timerTick + 120;
  }
}
#endif
