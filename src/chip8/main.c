#include "audio.h"
#include "byte_code_executor.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "datatypes.h"
#include "error_reports.h"
#include "hbios.h"
#include "instr_output.h"
#include "instr_sound.h"
#include "random.h"
#include "systemstate.h"
#include "systimer.h"
#include "timers.h"
#include "tms.h"
#include "xstdio.h"
#include "xstdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern void *himem;

MainArguments *mainArguments;

bool strFind(const char *searchString) __z88dk_fastcall {
  char **p = mainArguments->argv;
  for (uint8_t i = 0; i < mainArguments->argc; i++) {
    if (strstr(*p++, searchString)) {
      return 1;
    }
  }

  return 0;
}

void parseCommandLine() {
  CommandSwitches.isHelp = strFind("-?") || strFind("-H") || strFind("--HELP");

  uint8_t i = strFind("-X");
  if (i) {
    char *endptr;
    CommandSwitches.delayFactor = xstrtol(mainArguments->argv[i + 1], &endptr, 10);
  } else
    CommandSwitches.delayFactor = 0;
}

// #include "ym2149.h"

void main(MainArguments *pargs) __z88dk_fastcall {
  // (void)pargs;

  // audioInit();

  // uint8_t oldd = 255, oldb = 255;

  // while(true) {
  //   const uint8_t d = getControllerDirection(1);

  //   const uint8_t b = getControllerButton(1) | (getControllerButton(3) << 1);

  //   if(oldd != d || oldb != b)
  //     xprintf("Direction %02X, buttons: %02X\r\n", d, b);

  //   oldd = d;
  //   oldb = b;
  // }

  mainArguments = pargs;
  parseCommandLine();

  if (CommandSwitches.isHelp) {
    print("Usage:\r\n   chip8 <filename> [-X <delay>]\r\n\r\n");
    return;
  }

  applyConfiguration(pargs->argv[0]);

  chkMsg(fOpen(defaultFCB), "Unable to open file");

  xprintf("Total available memory: %u\r\n", (uint16_t)&himem - 0x200);

  if (!videoInit())
    return;

  audioInit();

  initSystemState();
  seedRandom();
  hbSndReset(0);

  uint16_t *ptr = programStorage;
  int       noMoreData = 0;

  while (!noMoreData) {
    fDmaOff(ptr);
    ptr += 64;
    noMoreData = fRead(defaultFCB);
  }
  chkMsg(fClose(defaultFCB), "unable to close");

  while (executeSingleInstruction())
    ;

  videoClose();
  soundOff();
}
