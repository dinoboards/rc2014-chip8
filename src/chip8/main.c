#include "byte_code_executor.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "datatypes.h"
#include "error_reports.h"
#include "hbios.h"
#include "instr_output.h"
#include "random.h"
#include "systemstate.h"
#include "systimer.h"
#include "tms.h"
#include "xstdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

MainArguments *mainArguments;

bool strFind(const char *searchString) __z88dk_fastcall {
  char **p = mainArguments->argv;
  for (uint8_t i = 0; i < mainArguments->argc; i++) {
    if (strstr(*p++, searchString))
      return i;
  }
  return 0;
}

void parseCommandLine() {
  CommandSwitches.isHelp = strFind("-?") || strFind("-H") || strFind("--HELP");
  CommandSwitches.isSerial = !CommandSwitches.isHelp && (strFind("-S") || strFind("--SERIAL"));
  CommandSwitches.isTms = !CommandSwitches.isHelp && (strFind("-T") || strFind("--TMS"));

  uint8_t i = strFind("-X");
  if (i) {
    char *endptr;
    CommandSwitches.delayFactor = xstrtol(mainArguments->argv[i + 1], &endptr, 10);
  } else
    CommandSwitches.delayFactor = 0;

  if (CommandSwitches.isSerial && CommandSwitches.isTms)
    abortConflictTmsAndSerial();
}

void main(MainArguments *pargs) __z88dk_fastcall {
  if (!sysTimerSearchDriver())
    warnNoTimerFound();

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
    fDmaOff(ptr);
    ptr += 64;
    noMoreData = fRead(defaultFCB);
  }
  chkMsg(fClose(defaultFCB), "unable to close");

  while (executeSingleInstruction())
    ;
}
