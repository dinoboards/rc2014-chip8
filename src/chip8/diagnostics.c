#include "diagnostics.h"
#include "byte_code_executor.h"
#include "datatypes.h"
#include "systemstate.h"
#include "xstdio.h"

#include "keys.h"

#ifdef DIAGNOSTICS_ON

void updateMachineStateDisplay() {

  byte i;

  xprintf("\033[%d;%dHPC: %p", 3, 70, chip8PC);

  for (i = 0; i < 16; i++)
    xprintf("\033[%d;%dHV%X: %p", 4 + i, 70, i, registers[i]);

  xprintf("\033[%d;%dHI: %p", 4 + 16, 71, registerI);

  xprintf("\033[%d;%dHPC: %p", 4 + 16 + 3, 69, chip8PC);

  // if (currentInstruction == 0xE9A1) {
  //   startCounting = true;
  //   drawCount = 0;
  // }

  if (drawCount >= 4) {
    xprintf("\033[%d;%dH I", 3, 80);
    for (i = 0; i < 16; i++)
      xprintf("\033[%d;%dH%p: %02X", 4 + i, 80, registerI + i, ((byte *)programStorage)[registerI + i]);

    xprintf("\033[%d;%dHCode", 3, 90);

    for (signed char x = -8; x < 8; x++) {
      if (x != 0)
        xprintf("\033[%d;%dH%04X", 12 + x, 90, invertByteOrder(chip8PC[x]));
      else
        xprintf("\033[%d;%dH\033[46m%04X\033[0m", 12 + x, 90, invertByteOrder(chip8PC[x]));
    }

    char r;
    getKey(&r);

    // exit(1);
  }
}

#endif
