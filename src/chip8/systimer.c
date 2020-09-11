#include "systimer.h"
#include "hbios.h"
#include "systemstate.h"

static unsigned long result;

uint16_t getSysTimer() {
  if (useSimulatedTimer)
    return instructionCostCounter / 48;

  hbSysGetTimer(&result);
  return result;
}

bool sysTimerSearchDriver() {
  uint16_t t1 = getSysTimer();
  for (uint16_t i = 0; i < 30000; i++)
    ;
  uint16_t t2 = getSysTimer();

  useSimulatedTimer = t1 == t2;

  return !useSimulatedTimer;
}
