#include "filereader.h"
#include "charconstants.h"
#include "datatypes.h"
#include <stdio.h>

static const char *pFileName;
static FILE *      pFile;

void setFileName(const char *fileName) { pFileName = fileName; }

void openFileStream() { pFile = fopen(pFileName, "r"); }

void closeFileStream() { fclose(pFile); }

char getNextCharRaw() {
  const char b = getc(pFile);

  if (feof(pFile))
    return '\0';

  return b;
}
