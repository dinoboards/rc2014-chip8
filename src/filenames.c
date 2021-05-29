#include "filenames.h"
#include <string.h>

void replaceExtension(char *pResult, const char *pFileName, const char *newExtension) {
  char *p = strchr(pFileName, '.');

  if (!p) {
    strncpy(pResult, pFileName, MAX_FILE_NAME);
    strncat(pResult, newExtension, 4);
    return;
  }

  const int index = p - pFileName;
  strncpy(pResult, pFileName, index);
  strncpy(pResult + index, newExtension, 4);
}
