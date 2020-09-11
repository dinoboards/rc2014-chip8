#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"
#include "xstdio.h"

static uint16_t lastTimerTick;
uint16_t        currentTimerTick;

#undef MEASURE_PERFORMANCE
#ifdef MEASURE_PERFORMANCE
uint16_t performanceMeasureTick = 0;
uint16_t instructionCount = 0;
#endif

void initTimers() {
  manageTimers();
  lastTimerTick = getSysTimer();
}

inline void tickSoundTimer(byte diff) {
  if (soundTimer == 0)
    return;

  if (diff > soundTimer)
    soundTimer = 0;
  else
    soundTimer -= diff;
}

inline void tickDelayTimer(byte diff) {
  if (delayTimer == 0)
    return;

  if (diff > delayTimer)
    delayTimer = 0;
  else
    delayTimer -= diff;
}

void manageTimers() {
  currentTimerTick = getSysTimer();

#ifdef MEASURE_PERFORMANCE
  instructionCount++;

  if (currentTimerTick >= performanceMeasureTick) {
    xprintf("Count: '%d'\r\n", (instructionCount / 2));
    instructionCount = 0;
    performanceMeasureTick = currentTimerTick + 120;
  }
#endif

  if (soundTimer == 0 && delayTimer == 0)
    return;

  const byte diff = (byte)(currentTimerTick - lastTimerTick);

  tickSoundTimer(diff);
  tickDelayTimer(diff);

  lastTimerTick = currentTimerTick;
}
