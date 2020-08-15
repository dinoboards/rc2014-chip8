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
    xprintf("%s\r\n", msg);
    exit(1);
  }
}

#define BF_CIO       $00
#define BF_CIOIN     BF_CIO + 0 /* CHARACTER INPUT */
#define BF_CIOOUT    BF_CIO + 1 /* CHARACTER OUTPUT */
#define BF_CIOIST    BF_CIO + 2 /* CHARACTER INPUT STATUS */
#define BF_CIOOST    BF_CIO + 3 /* CHARACTER OUTPUT STATUS */
#define BF_CIOINIT   BF_CIO + 4 /* INIT/RESET DEVICE/LINE CONFIG */
#define BF_CIOQUERY  BF_CIO + 5 /* REPORT DEVICE/LINE CONFIG */
#define BF_CIODEVICE BF_CIO + 6 /* REPORT DEVICE INFO */

byte result = 'X';

void main() {
  print("running ch8 app\r\n");

  // while (!hbCioIst(0))
  //   ;

  // hbCioIn(0, &result);

  // xprintf("xxx %c xxx\r\n", result);

  cls();

  initSystemState();

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
