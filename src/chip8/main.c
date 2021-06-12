#include "audio.h"
#include "byte_code_executor.h"
#include "configuration_loader.h"
#include "cpm.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filenames.h"
#include "instr_output.h"
#include "instr_sound.h"
#include "msx_keyboard.h"
#include "msxdos.h"
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

bool strFind(int argc, char *argv[], const char *searchString) {
  char **p = argv;
  for (uint8_t i = 0; i < argc; i++) {
    if (strstr(*p++, searchString)) {
      return 1;
    }
  }

  return 0;
}

void parseCommandLine(int argc, char *argv[]) {
  CommandSwitches.isHelp = strFind(argc, argv, "-?") || strFind(argc, argv, "-H") || strFind(argc, argv, "--HELP");

  uint8_t i = strFind(argc, argv, "-X");
  if (i) {
    char *endptr;
    CommandSwitches.delayFactor = xstrtol(argv[i + 2], &endptr, 10);
  } else
    CommandSwitches.delayFactor = 0;
}

char gameFileName[MAX_FILE_NAME];

uint16_t txtNamBackup;

void main(int argc, char *argv[]) {
  parseCommandLine(argc, argv);

  if (CommandSwitches.isHelp) {
    printf("Usage:\r\n   chip8 <filename> [-X <delay>]\r\n\r\n");
    return;
  }

  const char *pFileName = argv[1];

  applyConfiguration(pFileName);

  replaceExtension(gameFileName, pFileName, ".CH8");

  txtNamBackup = TXTNAM;

  if (!videoInit())
    return;

  audioInit();

  initSystemState();
  seedRandom();

  uint16_t *ptr = programStorage;
  int       noMoreData = 0;

  uint16_t total = 0;

  if (defaultFCB->type[0] == ' ' && defaultFCB->type[1] == ' ' && defaultFCB->type[2] == ' ') {
    defaultFCB->type[0] = 'C';
    defaultFCB->type[1] = 'H';
    defaultFCB->type[2] = '8';
  }
  chkMsg(fOpen(defaultFCB), "Unable to open file");

  while (!noMoreData) {
    fDmaOff(ptr);
    ptr += 64;
    total += 128;
    noMoreData = fRead(defaultFCB);
  }
  chkMsg(fClose(defaultFCB), "Unable to close");

  executeROM();

  videoClose();
  soundOff();

  msxbiosInitxt();
  msxbiosInitPalette();
}
