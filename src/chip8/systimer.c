#include "systimer.h"
#include "hbios.h"

static unsigned long result;

uint16_t getSysTimer() {
  hbSysGetTimer(&result);
  return result;
}

bool sysTimerSearchDriver() {
  uint16_t t1 = getSysTimer();
  for (uint16_t i = 0; i < 30000; i++)
    ;
  uint16_t t2 = getSysTimer();

  return (t1 != t2);
}
