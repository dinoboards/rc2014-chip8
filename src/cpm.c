#include "cpm.h"
#include "hbios.h"
#include <string.h>

void captureCommandLine(char *pBuffer) __z88dk_fastcall {
  const char *pCli = commandLineTail->cli;

  for (int i = 0; i < commandLineTail->length; i++)
    *pBuffer++ = *pCli++;

  *pBuffer = '\0';
}

static void copySpaceTermString(char *spaceTerminatedString, const char *nullTerminatedString, uint8_t maxLength) {
  char *s = (char *)nullTerminatedString;

  uint8_t i = 0;
  do {
    *spaceTerminatedString++ = *s++;
    i++;
  } while (*s && *s != '.' && i < maxLength);

  for (; i < maxLength; i++)
    *spaceTerminatedString++ = ' ';
}

void resetFCB(const char *filename, const char *type, FCB *fcb) {
  memset(fcb, 0, sizeof(FCB));
  copySpaceTermString(fcb->filename, filename, 8);
  copySpaceTermString(fcb->type, type, 3);
}

void chkMsg(uint16_t result, const char *msg) {
  if (result == 0xFF) {
    print(msg);
    print("\r\n");
    exit(1);
  }
}
