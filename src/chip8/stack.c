#include "chip8/stack.h"
#include "chip8/systemstate.h"
#include "datatypes.h"
#include "xstdio.h"
#include <stdio.h>

uint16_t stack[MAX_STACK];
byte     stackIndex = 0;

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

/**
 * @brief pop from stack and update chip8PC pointer
 * @return true if stack is empty, and was unable to pop anything
 */
bool popPc() {
  if (stackIndex <= 0)
    return true;

  setChip8PC(stack[--stackIndex]);

  return false;
}
