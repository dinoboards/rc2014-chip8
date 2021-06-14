#include "datatypes.h"
#include "key_monitor.h"
#include "msx_keyboard.h"
#include "stack.h"
#include "systemstate.h"
#include "systimer.h"
#include "timers.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "instr_output.h"
#include "instr_registers.h"
#include "instr_sound.h"

const uint16_t *programStorage = (uint16_t *)0x200;

#include "byte_code_executor.h"

void initSystemState() {
  memset(registers, 0, 16);
  registerI = 0;

  // clang-format off
  __asm
  exx
  ld hl, 0x200
  exx
  __endasm;
  // clang-format on
}

void executeROM() {
  while (executeSingleInstruction())
    ;
}
