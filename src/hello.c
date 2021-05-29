#pragma output noprotectmsdos

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "hello.h"

uint8_t data;

void assemble(uint8_t pc) __z88dk_fastcall {
  data = pc;
}

void main() {
  assemble(2);
  printf("Hello World I am at location: %p\r\n", &main);
}
