#include "hbios.h"

static hbCioParams cioParams;

void print(const char *str) __z88dk_fastcall {
  cioParams.driver = 0;
  while (*str) {
    cioParams.chr = *str++;
    hbCioOut(&cioParams);
  }
}
