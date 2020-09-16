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
  if (timerMode != UNSET_TIMER_MODE)
    return timerMode;

  uint16_t t1 = hbSysGetTimer16();
  for (uint16_t i = 0; i < 50000; i++)
    ;
  uint16_t t2 = hbSysGetTimer16();

  if (t1 == t2)
    return timerMode = SIMULATED_TIMER_MODE;

  return timerMode = HBIOS_TIMER_MODE;
}
