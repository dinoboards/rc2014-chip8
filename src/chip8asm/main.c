#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "cpm.h"
#include "xstdio.h"

void main() {
  initLabelStorage();

  assemble(1);

  defaultFCB->r = 0;
  defaultFCB->r2 = 0;
  defaultFCB->cr = 0;
  defaultFCB->rc = 0;
  defaultFCB->s1 = 0;
  defaultFCB->s2 = 0;
  defaultFCB->dr = 0;
  defaultFCB->ex = 0;

  assemble(2);

  defaultFCB->type[0] = 'c';
  defaultFCB->type[1] = 'h';
  defaultFCB->type[2] = '8';
  defaultFCB->r = 0;
  defaultFCB->r2 = 0;
  defaultFCB->cr = 0;
  defaultFCB->rc = 0;
  defaultFCB->s1 = 0;
  defaultFCB->s2 = 0;
  defaultFCB->dr = 0;
  defaultFCB->ex = 0;

  xprintf("Attempting to write to file\r\n");
  chk(fMake(defaultFCB));
  chk(fDmaOff(programStorage));
  chk(fWrite(defaultFCB));
  chk(fClose(defaultFCB));
  xprintf("File written\r\n");
}
