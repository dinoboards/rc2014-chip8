#include "chip8/stack.h"
#include "chip8/systemstate.h"
#include "datatypes.h"
#include "xstdio.h"

uint16_t stack[MAX_STACK];
byte     stackIndex = 0;

/**
 * @brief Push the current value of chip8PC onto the stack
 * @return (void)
 */
void pushPc() {
  if (stackIndex >= 16) {
    xprintf("Stack overflow\r\n");
    exit(1);
  }

  stack[stackIndex++] = (uint16_t)chip8PC;
}

/**
 * @brief pop from stack and update chip8PC pointer
 * @return true if stack is empty, and was unable to pop anything
 */
bool popPc() {
  if (stackIndex <= 0)
    return true;

  chip8PC = (uint16_t *)stack[--stackIndex];

  return false;
}
