#include "systimer.h"
#include "hbios.h"
#include "xstdio.h"

static unsigned long result;

uint16_t getSysTimer() {
  hbSysGetTimer(&result);
  return result;
}
