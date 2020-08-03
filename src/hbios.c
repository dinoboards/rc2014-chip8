#include "hbios.h"

void print(const char *str) __z88dk_fastcall {
  while (*str)
    hbCioOut(0, *str++);
}
