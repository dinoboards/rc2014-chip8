#include "datatypes.h"
#include "diagnostics.h"
#include "instr_output.h"
#include "instr_pc.h"
#include "instr_random.h"
#include "instr_registers.h"
#include "key_monitor.h"
#include "stack.h"
#include "systemstate.h"
#include "systimer.h"
#include "timers.h"
#include "tty.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const uint16_t *programStorage = (uint16_t *)0x200;

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

inline uint16_t readInstruction() {
  const uint16_t r = *chip8PC;

  xtracef("Read instruction %04X @ %p\r\n", invertByteOrder(r), chip8PC);

  chip8PC++;
  return r;
}

#define CH8_JP_NIB          0x1
#define CH8_CALL_NIB        0x2
#define CH8_SE_VX_BYTE_NIB  0x3
#define CH8_SNE_VX_BYTE_NIB 0x4
#define CH8_SE_VX_VY_NIB    0x5
#define CH8_LD_VX_BYTE_NIB  0x6
#define CH8_ADDVX_NIB       0x7
#define CH8_0X8_SERIES_NIB  0x8
#define CH8_SNE_VX_VY_NIB   0x9
#define CH8_LD_I_ADDR_NIB   0xA
#define CH8_RND_VX_NIB      0xC
#define CH8_DRW_NIB         0xD
#define CH8_0XE_SERIES_NIB  0xE
#define CH8_CLS             0xE000
#define CH8_RET             0xEE00
#define CH8_0XF_SERIES_NIB  0xF

#define CH8_SKP_VX_LOW_BYTE   0x9E
#define CH8_SKNP_VX_LOW_BYTE  0xA1
#define CH8_LD_ST_VX_LOW_BYTE 0x18
#define CH8_LD_DT_VX_LOW_BYTE 0x15
#define CH8_LD_VX_DT_LOW_BYTE 0x07
#define CH8_ADD_I_VX_LOW_BYTE 0x1E
#define CH8_LD_VX_I_LOW_BYTE  0x65

#define CH8_LD_VX_VY_NIB  0x0
#define CH8_AND_VX_VY_NIB 0x2
#define CH8_ADD_VX_VY_NIB 0x4
#define CH8_SHR_VX_VY_NIB 0x6

void initSystemState() {
  memset(registers, 0, sizeof(registers));
  registerI = 0;
  chip8PC = (uint16_t *)programStorage;
}

bool executeSingleInstruction() {
#ifdef DIAGNOSTICS_ON
  updateMachineStateDisplay();
#endif

  uint16_t currentInstruction = readInstruction(); // high/low bytes in inverted order

  checkForKeyPresses();
  manageTimers();

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
    case CH8_0X8_SERIES_NIB: {
      switch (fourthNibble) {
      case CH8_LD_VX_VY_NIB: {
        ldVxVy();
        break;
      }

      case CH8_AND_VX_VY_NIB: {
        andVxVy();
        break;
      }

      case CH8_SHR_VX_VY_NIB: {
        shrVxVy();
        break;
      }

      case CH8_ADD_VX_VY_NIB: {
        addVxVy();
        break;
      }
      default:
        goto badInstruction;
      }
      break;
    }

    case CH8_LD_VX_BYTE_NIB:
      ldVxByte();
      break;

    case CH8_LD_I_ADDR_NIB:
      ldIAddr();
      break;

    case CH8_DRW_NIB: {
      draw();
#ifdef DIAGNOSTICS_ON
      if (startCounting)
        drawCount++;
#endif
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

    case CH8_SNE_VX_BYTE_NIB: {
      sneVxByte();
      break;
    }

    case CH8_SNE_VX_VY_NIB: {
      sneVxVy();
      break;
    }

    case CH8_JP_NIB: {
      jp();
      break;
    }

    case CH8_RND_VX_NIB: {
      rnd();
      break;
    }

    case CH8_0XF_SERIES_NIB: {
      switch (lowByte) {
      case CH8_LD_ST_VX_LOW_BYTE: {
        ldStVx();
        break;
      }

      case CH8_LD_DT_VX_LOW_BYTE: {
        ldDtVx();
        break;
      }

      case CH8_LD_VX_DT_LOW_BYTE: {
        ldVxDt();
        break;
      }

      case CH8_ADD_I_VX_LOW_BYTE: {
        addIVx();
        break;
      }

      case CH8_LD_VX_I_LOW_BYTE: {
        ldVxI();
        break;
      }

      default:
        goto badInstruction;
      }
      break;
    }

    case CH8_0XE_SERIES_NIB: {
      switch (lowByte) {
      case CH8_SKP_VX_LOW_BYTE: {
        skpVx();
        break;
      }

      case CH8_SKNP_VX_LOW_BYTE: {
        sknpVx();
        break;
      }

      default:
        goto badInstruction;
      }

      break;
    }

    default:
    badInstruction:
      xprintf("Bad instruction %04X at %p\r\n", invertByteOrder(currentInstruction), chip8PC - 1);
      return false;
    }
  }
  }

  return true;
}
