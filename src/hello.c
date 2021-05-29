#pragma output noprotectmsdos

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "hello.h"

void main() {
  printf("Hello World I am at location: %p\r\n", &main);
}
