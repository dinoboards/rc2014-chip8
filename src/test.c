#pragma output noprotectmsdos

#include "chip8/msx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <stdarg.h>
// #include <stdio.h>
// #include <string.h>

// #include "hello.h"

// uint8_t data;

// void assemble(uint8_t pc) __z88dk_fastcall { data = pc; }

// #define MAX_FILE_NAME 32

// char newFileName[MAX_FILE_NAME];

// void replaceExtension(const char *pFileName, const char *newExtension, char *pResult) {
//   char *p = strchr(pFileName, '.');

//   if (!p) {
//     strncpy(pResult, pFileName, MAX_FILE_NAME);
//     strncat(pResult, newExtension, 4);
//     return;
//   }

//   const int index = p - pFileName;
//   strncpy(pResult, pFileName, index);
//   strncpy(pResult + index, newExtension, 4);
// }

void main(int argc, char *argv[]) {
  // assemble(2);
  printf("Hello World I am at location: %p\r\n", &main);

  printf("TIME: %d\r\n", JIFFY);

  // if (argc != 2) {
  //   puts("Invalid arguments.  Usage: \r\n    chip8asm <filename>.ch8\r\n");
  //   exit(1);
  // }

  // const char *fileName = argv[1];

  // replaceExtension(fileName, ".BOB", newFileName);

  // printf("new name: %d, %s\r\n", index, newFileName);

  // const char* p = strchr(fileName, '.');

  // if (!p) {
  //   puts("Invalid filename - must end in .ch8\r\n");
  //   exit(1);
  // }

  // int index = p - fileName + 1;
  // strncpy(newFileName, fileName, index);
  // strncpy(newFileName+index, "xyz", 4);
  // printf("new name: %d, %s\r\n", index, newFileName);

  // int c;

  // printf("argc %d %s", argc, argv[1]);

  // FILE* f = fopen(argv[0], "r");

  //  while(1) {
  //     c = fgetc(f);
  //     if( feof(f) ) {
  //        break ;
  //     }
  //     printf("%c", c);
  //  }
  //  fclose(f);
}
