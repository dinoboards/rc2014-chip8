#pragma output noprotectmsdos

#include <stdio.h>

// #include <stdarg.h>
// #include <stdio.h>
// #include <string.h>

// #include "hello.h"

// uint8_t data;

// void assemble(uint8_t pc) __z88dk_fastcall { data = pc; }

void main() {
  // assemble(2);
  printf("Hello World I am at location: %p\r\n", &main);

  // int c;

  // FILE* f = fopen("test.txt", "r");

  //  while(1) {
  //     c = fgetc(f);
  //     if( feof(f) ) {
  //        break ;
  //     }
  //     printf("%c", c);
  //  }
  //  fclose(f);
}
