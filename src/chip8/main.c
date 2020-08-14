#include "chip8/byte_code_executor.h"
#include "chip8/instr_output.h"
#include "cpm.h"
#include "datatypes.h"
#include "hbios.h"
#include "systemstate.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void chkMsg(int result, const char *msg) {
  if (result) {
    xtracef(msg);
    xtracef("\r\n");
    exit(1);
  }
}

void main() {
  print("running ch8 app\r\n");

  cls();

  initSystemState();

  // TODO: Read all the file
  chkMsg(fOpen(defaultFCB), "Unable to open file");
  chk(fDmaOff(programStorage));
  chkMsg(fRead(defaultFCB), "unable to read");
  chkMsg(fClose(defaultFCB), "unable to close");

  while (executeSingleInstruction())
    ;

  xprintf("\033[33;0Hprogram end\r\n");

  for (int i = 0; i < 16; i++) {
    xprintf("  V%X = %02X\r\n", i, (int)registers[i]);
  }

  xprintf("  I = %04X\r\n", registerI);
}
