#pragma output noprotectmsdos

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "hello.h"

uint8_t data;

void assemble(uint8_t pc) __z88dk_fastcall { data = pc; }

void main() {
  assemble(2);
  printf("Hello World I am at location: %p\r\n", &main);
}
