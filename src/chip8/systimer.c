#include "systimer.h"
#include "hbios.h"
#include "systemstate.h"

uint16_t getSysTimer() {
  if (useSimulatedTimer)
    return instructionCostCounter / 48;

  return hbSysGetTimer16();
}

bool sysTimerSearchDriver() {
  uint16_t t1 = getSysTimer();
  for (uint16_t i = 0; i < 30000; i++)
    ;
  uint16_t t2 = getSysTimer();

  useSimulatedTimer = t1 == t2;

  return !useSimulatedTimer;
}
