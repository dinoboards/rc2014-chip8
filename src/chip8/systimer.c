#include "systimer.h"
#include "hbios.h"
#include "systemstate.h"

uint16_t getSysTimer() {
  if (timerMode == TMS_TIMER_MODE)
    return timerTick;

  if (timerMode == HBIOS_TIMER_MODE)
    return hbSysGetTimer16();

  return instructionCostCounter / 48;
}

uint8_t sysTimerSearchDriver() {
  if (timerMode != 0)
    return timerMode;

  uint16_t t1 = getSysTimer();
  for (uint16_t i = 0; i < 30000; i++)
    ;
  uint16_t t2 = getSysTimer();

  if (t1 == t2)
    return timerMode = SIMULATED_TIMER_MODE;

  return timerMode = HBIOS_TIMER_MODE;
}
