#include "token.h"
#include "charconstants.h"
#include "filereader.h"

int lineNumber;

char tokenCurrentChar;
char tokenTerminatorChar;

char getNextChar() {
  char c = CR;
  while (c == CR)
    c = getNextCharRaw();
  return c;
}
