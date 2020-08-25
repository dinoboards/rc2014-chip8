#include "cpm.h"

void captureCommandLine(char *pBuffer) __z88dk_fastcall {
  const char *pCli = commandLineTail->cli;

  for (int i = 0; i < commandLineTail->length; i++)
    *pBuffer++ = *pCli++;

  *pBuffer = '\0';
}
