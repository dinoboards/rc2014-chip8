#include "hbios.h"
#include "xstdio.h"

long result;

long getSysTimer() {
  hbSysGetTimer(&result);
  return result;
}

void wait() {
  for (uint16_t i = 0; i < 6000; i++)
    ;
}
