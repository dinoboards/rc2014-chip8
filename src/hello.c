#pragma output noprotectmsdos

#include <stdio.h>

#include "chip8/msx.h"
// #include <stdio.h>
// #include <string.h>

#include "hello.h"

// uint8_t data;

// void assemble(uint8_t pc) __z88dk_fastcall { data = pc; }

void main(void) {
  // assemble(2);
  printf("!!Hello World I am at location: %p with stack at %p\r\n", &main, getStack());

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
