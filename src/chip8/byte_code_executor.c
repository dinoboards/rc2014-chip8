#include "datatypes.h"
#include "diagnostics.h"
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

#include "instr_output.h"
#include "instr_pc.h"
#include "instr_random.h"
#include "instr_registers.h"

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

void initSystemState() {
  memset(registers, 0, sizeof(registers));
  registerI = 0;
  chip8PC = (uint16_t *)programStorage;
}

static uint16_t currentInstruction;

bool executeSingleInstruction() {
#ifdef DIAGNOSTICS_ON
  updateMachineStateDisplay();
#endif

  currentInstruction = readInstruction(); // high/low bytes in inverted order

  for (uint16_t d = 0; d <= CommandSwitches.delayFactor; d++)
    ;

  manageTimers();
  checkForKeyPresses();

  switch (currentInstruction) {
  case 0xE000:
    cls();
    break;

  case 0xEE00:
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
    case 0x1: {
      jp();
      break;
    }

    case 0x2: {
      call();
      break;
    }

    case 0x3: {
      seVxByte();
      break;
    }

    case 0x4: {
      sneVxByte();
      break;
    }

    case 0x5: {
      seVxVy();
      break;
    }

    case 0x6:
      ldVxByte();
      break;

    case 0x7: {
      addVxByte();
      break;
    }

    case 0x8: {
      switch (fourthNibble) {
      case 0x0: {
        ldVxVy();
        break;
      }

      case 0x1: {
        orVxVy();
        break;
      }

      case 0x2: {
        andVxVy();
        break;
      }

      case 0x3: {
        xorVxVy();
        break;
      }

      case 0x4: {
        addVxVy();
        break;
      }

      case 0x5: {
        subVxVy();
        break;
      }

      case 0x6: {
        shrVxVy();
        break;
      }

      // case 0x7:

      case 0xE: {
        shlVxVy();
        break;
      }

      default:
        goto badInstruction;
      }
      break;
    }

    case 0x9: {
      sneVxVy();
      break;
    }

    case 0xA:
      ldIAddr();
      break;

    // case 0xB:

    case 0xC: {
      rnd();
      break;
    }

    case 0xD: {
      draw();
      break;
    }

    case 0xE: {
      switch (lowByte) {
      case 0x9E: {
        skpVx();
        break;
      }

      case 0xA1: {
        sknpVx();
        break;
      }

      default:
        goto badInstruction;
      }

      break;
    }

    case 0xF: {
      switch (lowByte) {

      case 0x07: {
        ldVxDt();
        break;
      }

      case 0x0A: {
        keyVx();
        break;
      }

      case 0x15: {
        ldDtVx();
        break;
      }

      case 0x18: {
        ldStVx();
        break;
      }

      case 0x1E: {
        addIVx();
        break;
      }

      case 0x29: {
        ldfIVx();
        break;
      }

      case 0x33: {
        bcdIVx();
        break;
      }

      case 0x55: {
        ldIVx();
        break;
      }

      case 0x65: {
        ldVxI();
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
