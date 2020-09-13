#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "instr_sound.h"
#include "systemstate.h"
#include "systimer.h"

#undef MEASURE_PERFORMANCE

#ifdef MEASURE_PERFORMANCE
#include "xstdio.h"
#endif

static uint16_t lastTimerTick;
uint16_t        currentTimerTick;

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

  if (diff >= soundTimer) {
    soundTimer = 0;
    soundOff();
  } else
    soundTimer -= diff;
}

inline void tickDelayTimer(byte diff) {
  if (delayTimer == 0)
    return;

  if (diff >= delayTimer)
    delayTimer = 0;
  else
    delayTimer -= diff;
}

void manageTimers() {
#ifdef MEASURE_PERFORMANCE
  instructionCount++;
#endif

  timerSkipFlag++;
  if (timerSkipFlag % 16)
    return;

  currentTimerTick = getSysTimer();

#ifdef MEASURE_PERFORMANCE
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
