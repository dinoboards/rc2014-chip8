#include "audio.h"
#include "byte_code_executor.h"
#include "configuration_loader.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filenames.h"
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

// extern void *himem;

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

void main(int argc, char *argv[]) {
  parseCommandLine(argc, argv);

  if (CommandSwitches.isHelp) {
    print("Usage:\r\n   chip8 <filename> [-X <delay>]\r\n\r\n");
    return;
  }

  const char *pFileName = argv[1];

  applyConfiguration(pFileName);

  replaceExtension(gameFileName, pFileName, ".CH8");

  FILE *pFile = fopen(gameFileName, "r");
  if (pFile == NULL) {
    printf("Unable to open file %s\r\n", gameFileName);
    exit(1);
  }

  // printf("Total available memory: %u\r\n", (uint16_t)&himem - 0x200);

  if (!videoInit())
    return;

  audioInit();

  initSystemState();
  seedRandom();
  hbSndReset(0);

  uint16_t *ptr = programStorage;
  int       noMoreData = 0;

  fread(programStorage, 1, 20000, pFile);
  fclose(pFile);

  while (executeSingleInstruction())
    ;

  videoClose();
  soundOff();
}
