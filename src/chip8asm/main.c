#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "filenames.h"
#include "filereader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char newFileName[MAX_FILE_NAME];

void main(int argc, char *argv[]) {

  if (argc != 2) {
    puts("Invalid arguments.  Usage: \r\n    chip8asm <filename>.cas\r\n");
    exit(1);
  }

  const char *fileName = argv[1];

  setFileName(fileName);

  initLabelStorage();

  assemble(1);

  assemble(2);

  printf("Total Byte Count %d\r\n", currentAddress);

  replaceExtension(newFileName, fileName, ".CH8");
  FILE *pFile = fopen(newFileName, "w");

  char *pBuffer    = (char *)programStorage;
  char *pBufferEnd = pBuffer + currentAddress;

  for (char *p = pBuffer; p < pBufferEnd; p++)
    putc(*p, pFile);

  const uint8_t cpmPage = 128 - ((uint8_t)(currentAddress % 128));

  for (uint8_t i = 0; i < cpmPage; i++)
    putc(0, pFile);

  printf("File written\r\n");
}
