#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"
#include "xstdio.h"

static uint16_t lastTimerTick;
uint16_t        currentTimerTick;

#define MEASURE_PERFORMANCE
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
  currentTimerTick = getSysTimer();

#ifdef MEASURE_PERFORMANCE
  instructionCount++;

  if (currentTimerTick > performanceMeasureTick) {
    xprintf("Count: '%d'\r\n", (instructionCount / 2));
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

#endif

  const byte diff = (byte)(currentTimerTick - lastTimerTick);

  tickSoundTimer(diff);
  tickDelayTimer(diff);

  lastTimerTick = currentTimerTick;
}
