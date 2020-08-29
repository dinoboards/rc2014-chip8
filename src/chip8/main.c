#include "byte_code_executor.h"
#include "configuration_loader.h"
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

MainArguments *mainArguments;

bool strFind(const char *searchString) __z88dk_fastcall {
  char **p = mainArguments->argv;
  for (uint8_t i = 0; i < mainArguments->argc; i++) {
    if (strstr(*p++, searchString))
      return true;
  }
  return false;
}

void parseCommandLine() {

  CommandSwitches.isHelp = strFind("-?") || strFind("-H") || strFind("--HELP");
  CommandSwitches.isSerial = !CommandSwitches.isHelp && (strFind("-S") || strFind("--SERIAL"));
  CommandSwitches.isTms = !CommandSwitches.isHelp && (strFind("-T") || strFind("--TMS"));

  if (CommandSwitches.isSerial && CommandSwitches.isTms) {
    xprintf("Error: Must dual output not supported.  Only one of --SERIAL (-S) or --TMS (-T) supported\r\n");
    exit(0);
  }

  if (!CommandSwitches.isSerial && !CommandSwitches.isTms && !CommandSwitches.isHelp)
    CommandSwitches.isSerial = true;
}

void main(MainArguments *pargs) __z88dk_fastcall {
  mainArguments = pargs;
  parseCommandLine();

  applyConfiguration(pargs->argv[0]);

  videoInit();

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
}
