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

typedef struct {
  const char *code;
  uint8_t     rowIndex;
  uint8_t     bitMask;
} MatrixMapping;

MatrixMapping MatrixMappings[] = {
    {"7", 0, 1 << 7},       {"6", 0, 1 << 6},        {"5", 0, 1 << 5},           {"4", 0, 1 << 4},       {"3", 0, 1 << 3},       {"2", 0, 1 << 2},        {"1", 0, 1 << 1},       {"0", 0, 1 << 0},

    {";", 1, 1 << 7},       {"]", 1, 1 << 6},        {"[", 1, 1 << 5},           {"\\", 1, 1 << 4},      {"=", 1, 1 << 3},       {"-", 1, 1 << 2},        {"9", 1, 1 << 1},       {"8", 1, 1 << 0},

    {"B", 2, 1 << 7},       {"A", 2, 1 << 6},        {"{DEAD}", 2, 1 << 5},      {"/", 2, 1 << 4},       {".", 2, 1 << 3},       {",", 2, 1 << 2},        {"`", 2, 1 << 1},       {"\'", 2, 1 << 0},

    {"J", 3, 1 << 7},       {"I", 3, 1 << 6},        {"H", 3, 1 << 5},           {"G", 3, 1 << 4},       {"F", 3, 1 << 3},       {"E", 3, 1 << 2},        {"D", 3, 1 << 1},       {"C", 3, 1 << 0},

    {"R", 4, 1 << 7},       {"Q", 4, 1 << 6},        {"P", 4, 1 << 5},           {"O", 4, 1 << 4},       {"N", 4, 1 << 3},       {"M", 4, 1 << 2},        {"L", 4, 1 << 1},       {"K", 4, 1 << 0},

    {"Z", 5, 1 << 7},       {"Y", 5, 1 << 6},        {"X", 5, 1 << 5},           {"W", 5, 1 << 4},       {"V", 5, 1 << 3},       {"U", 5, 1 << 2},        {"T", 5, 1 << 1},       {"S", 5, 1 << 0},

    {"{F3}", 6, 1 << 7},    {"{F2}", 6, 1 << 6},     {"{F1}", 6, 1 << 5},        {"{CODE}", 6, 1 << 4},  {"{CAPS}", 6, 1 << 3},  {"{GRAPH}", 6, 1 << 2},  {"{CTRL}", 6, 1 << 1},  {"{SHIFT}", 6, 1 << 0},

    {"{RET}", 7, 1 << 7},   {"{SELECT}", 7, 1 << 6}, {"{BACKSPACE}", 7, 1 << 5}, {"{STOP}", 7, 1 << 4},  {"{TAB}", 7, 1 << 3},   {"{ESC}", 7, 1 << 2},    {"{F5}", 7, 1 << 1},    {"{F4}", 7, 1 << 0},

    {"{RIGHT}", 8, 1 << 7}, {"{DOWN}", 8, 1 << 6},   {"{UP}", 8, 1 << 5},        {"{LEFT}", 8, 1 << 4},  {"{DEL}", 8, 1 << 3},   {"{INSERT}", 8, 1 << 2}, {"{HOME}", 8, 1 << 1},  {"{SPACE}", 8, 1 << 0},

    {"{NUM4}", 9, 1 << 7},  {"{NUM3}", 9, 1 << 6},   {"{NUM2}", 9, 1 << 5},      {"{NUM1}", 9, 1 << 4},  {"{NUM0}", 9, 1 << 3},  {"{NUM/}", 9, 1 << 2},   {"{NUM+}", 9, 1 << 1},  {"{NUM*}", 9, 1 << 0},

    {"{NUM.}", 10, 1 << 7}, {"{NUM,}", 10, 1 << 6},  {"{NUM-}", 10, 1 << 5},     {"{NUM9}", 10, 1 << 4}, {"{NUM8}", 10, 1 << 3}, {"{NUM7}", 10, 1 << 2},  {"{NUM6}", 10, 1 << 1}, {"{NUM5}", 10, 1 << 0}, {0, 0, 0}};

#define MATRIX_MAPPING_LENGTH (sizeof(MatrixMappings) / sizeof(MatrixMapping))

MatrixMapping *codeToMatrix(const char *keyCode) __z88dk_fastcall {
  MatrixMapping *p = MatrixMappings;
  while (1) {
    const char *code = p->code;
    if (strcmp(code, keyCode) == 0 || *code == 0)
      return p;

    p++;
  }
}

#define IS_KEY_PRESSED(i, m) (!((m)&msxNewKey[(i)]))

void main(int argc, char *argv[]) {
  // assemble(2);
  printf("Hello World I am at location: %p\r\n", &main);

  printf("TIME: %d\r\n", msxJiffy);

  const MatrixMapping *x = codeToMatrix("{SPACE}");
  printf("%d, %d\r\n", x->rowIndex, x->bitMask);

  while (!IS_KEY_PRESSED(x->rowIndex, x->bitMask)) {
    printf("ROW %d %02X\r\n", x->rowIndex, msxNewKey[x->rowIndex]);
  }

  // msxGetPtr = msxPutPtr;

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
