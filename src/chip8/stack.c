#include "chip8/stack.h"
#include "chip8/systemstate.h"
#include "datatypes.h"
#include "xstdio.h"
#include <stdio.h>

/**
 * @brief Push the current value of chip8PC onto the stack
 * @return (void)
 */
void pushPc() {
  if (stackIndex >= MAX_STACK) {
    printf("Stack overflow\r\n");
    exit(1);
  }

  stack[stackIndex++] = getChip8PC();
}
