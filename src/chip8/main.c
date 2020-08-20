#include "byte_code_executor.h"
#include "cpm.h"
#include "datatypes.h"
#include "hbios.h"
#include "instr_output.h"
#include "random.h"
#include "systemstate.h"
#include "tms.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void chkMsg(uint16_t result, const char *msg) {
  if (result == 0xFF) {
    xprintf("%s\r\n", msg);
    exit(1);
  }
}

void main() {
  print("running ch8 app!!!!!\r\n");

  captureTmsRegisters();

  cls();

  initSystemState();
  seedRandom();

  uint16_t *ptr = programStorage;
  int       noMoreData = 0;

  chkMsg(fOpen(defaultFCB), "Unable to open file");

  while (!noMoreData) {
    chkMsg(fDmaOff(ptr), "dma");
    ptr += 64;
    noMoreData = fRead(defaultFCB);
  }
  chkMsg(fClose(defaultFCB), "unable to close");

  while (executeSingleInstruction())
    ;

  xprintf("\033[33;0Hprogram end\r\n");

  for (int i = 0; i < 16; i++) {
    xprintf("  V%X = %02X\r\n", i, (int)registers[i]);
  }

  xprintf("  I = %04X\r\n", registerI);
}
