#include "datatypes.h"
#include "instr_output.h"
#include "instr_pc.h"
#include "instr_registers.h"
#include "stack.h"
#include "systemstate.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const uint16_t *programStorage = (uint16_t *)0x200;

inline uint16_t readInstruction() {
  const uint16_t r = *chip8PC;

  xtracef("Read instruction %04X @ %p\r\n", r, chip8PC);

  chip8PC++;
  return r;
}

#define CH8_LD_VX_BYTE_NIB 6
#define CH8_CLS            0xE000
#define CH8_RET            0xEE00
#define CH8_LD_I_ADDR_NIB  0xA
#define CH8_DRW_NIB        0xD
#define CH8_CALL_NIB       0x2
#define CH8_ADDVX_NIB      0x7
#define CH8_SE_VX_BYTE_NIB 0x3
#define CH8_SE_VX_VY_NIB   0x5

uint16_t invertByteOrder(uint16_t word) __naked __z88dk_fastcall {
  (void)word;
  // clang-format off
  __asm
    ld a, h
    ld h, l
    ld l, a
    ret
  __endasm;
  // clang-format on
}

void initSystemState() {
  memset(registers, 0, sizeof(registers));
  registerI = 0;
  chip8PC = (uint16_t *)programStorage;
}

bool executeSingleInstruction() {
  uint16_t currentInstruction = readInstruction(); // high/low bytes in inverted order

  switch (currentInstruction) {
  case CH8_CLS:
    cls();
    break;

  case CH8_RET:
    if (ret())
      return false;
    break;

  default: {
    firstNibble = (uint8_t)(currentInstruction >> 4) & 0xF;
    secondNibble = (uint8_t)currentInstruction & 0xF;
    thirdNibble = (uint8_t)(currentInstruction >> 12);
    lowByte = (currentInstruction >> 8) & 0xFF;
    fourthNibble = lowByte & 0xF;
    addr = (currentInstruction >> 8) + (((int)secondNibble) << 8);

    switch (firstNibble) {
    case CH8_LD_VX_BYTE_NIB:
      ldVxByte();
      break;

    case CH8_LD_I_ADDR_NIB:
      ldIAddr();
      break;

    case CH8_DRW_NIB: {
      draw();
      break;
    }

    case CH8_CALL_NIB: {
      call();
      break;
    }

    case CH8_ADDVX_NIB: {
      addVxByte();
      break;
    }

    case CH8_SE_VX_BYTE_NIB: {
      seVxByte();
      break;
    }

    case CH8_SE_VX_VY_NIB: {
      seVxVy();
      break;
    }
    default:
      xprintf("Bad instruction %04X at %p\r\n", invertByteOrder(currentInstruction), chip8PC);
      return false;
    }
  }
  }

  return true;
}
