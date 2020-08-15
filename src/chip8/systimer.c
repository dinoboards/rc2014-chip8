#include "hbios.h"

long result;

long getSysTimer() {
  hbSysGetTimer(&result);
  return result;
}
