#ifndef __MSX_KEYBOARD
#define __MSX_KEYBOARD

#include <stdlib.h>

typedef struct {
  const char *code;
  uint8_t     rowIndex;
  uint8_t     bitMask;
} MatrixMapping;

extern MatrixMapping MatrixMappings[];

extern MatrixMapping *codeToMatrix(const char *keyCode) __z88dk_fastcall;

#define IS_KEY_PRESSED(i, m) (!((m)&msxNewKey[(i)]))

#endif
