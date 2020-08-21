#include "hbios.h"
#include "xstdio.h"

unsigned long result;

unsigned long getSysTimer() {
  hbSysGetTimer(&result);
  return result;
}
