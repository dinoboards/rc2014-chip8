#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"
#include "xstdio.h"

unsigned long lastTimerTick;
unsigned long currentTimerTick;

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
  if (soundTimer == 0 && delayTimer == 0) {
    return;
  }

  byte diff = getTimerTicks();

  tickSoundTimer(diff);
  tickDelayTimer(diff);

  lastTimerTick = currentTimerTick;
}
