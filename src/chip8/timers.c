#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"
#include "xstdio.h"

unsigned long lastTimerTick;
unsigned long currentTimerTick;

#undef MEASURE_PERFORMANCE
#ifdef MEASURE_PERFORMANCE
unsigned long performanceMeasureTick = 0;
unsigned long instructionCount = 0;
#endif

void initTimers() {
  manageTimers();
  lastTimerTick = getSysTimer();
}

inline byte getTimerTicks() {
  currentTimerTick = getSysTimer();

  return (byte)(currentTimerTick - lastTimerTick);
}

inline void tickSoundTimer(byte diff) {
  if (soundTimer == 0)
    return;

  if (diff > soundTimer)
    soundTimer = 0;
  else
    soundTimer -= diff;

  // if (soundTimer & 1) {
  //   sendDrawCommands("\033[%d;%dHS\007", 1, 70);
  // } else {
  //   sendDrawCommands("\033[%d;%dH ", 1, 70);
  // }
}

inline void tickDelayTimer(byte diff) {
  if (delayTimer == 0)
    return;

  if (diff > delayTimer)
    delayTimer = 0;
  else
    delayTimer -= diff;

  // if (delayTimer & 1) {
  //   sendDrawCommands("\033[%d;%dHD\007", 1, 71);
  // } else {
  //   sendDrawCommands("\033[%d;%dH ", 1, 71);
  // }
}

void manageTimers() {
#ifdef MEASURE_PERFORMANCE
  byte diff = getTimerTicks();
  instructionCount++;

  if (currentTimerTick > performanceMeasureTick) {
    xprintf("Count: '%ld'\r\n", (instructionCount / 2));
    instructionCount = 0;
    performanceMeasureTick = currentTimerTick + 120;
  }

  if (soundTimer == 0 && delayTimer == 0) {
    return;
  }
#else
  if (soundTimer == 0 && delayTimer == 0) {
    return;
  }

  byte diff = getTimerTicks();
#endif

  tickSoundTimer(diff);
  tickDelayTimer(diff);

  lastTimerTick = currentTimerTick;
}
