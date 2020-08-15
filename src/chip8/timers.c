#include "timers.h"
#include "chip8/tty.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"
#include "xstdio.h"

long lastTimerTick;
long currentTimerTick;

void initTimers() { lastTimerTick = getSysTimer(); }

void manageTimers() {
  if (soundTimer == 0) {
    return;
  }

  currentTimerTick = getSysTimer();

  byte diff = (byte)(currentTimerTick - lastTimerTick);

  if (diff > soundTimer)
    soundTimer = 0;
  else
    soundTimer -= diff;

  lastTimerTick = currentTimerTick;

  if (soundTimer & 1) {
    sendDrawCommands("\033[%d;%dHS\007", 1, 70);
  } else {
    sendDrawCommands("\033[%d;%dH ", 1, 70);
  }
}
