#include "configreader.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filereader.h"
#include "tms.h"

Token      token;
const char commentChar = '#';

inline char getNext() { return _getNext(token.currentLine); }

DEF_TOKEN_EQUALS(token.value)

void tokeniseAlphaNumericString() {
  tokenMap("color0", TokenColour0);
  tokenMap("color1", TokenColour1);

  token.isColour = true;
  tokenMap("darkblue", COL_DRKBLUE);
  tokenMap("black", COL_BLACK);
  token.isColour = false;

  token.type = TokenUnknown;
}

static bool isEqual() {
  if (tokenCurrentChar != '=')
    return false;

  tokenCurrentChar = getNext();

  token.value[0] = ',';
  token.value[1] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = TokenEquals;

  return true;
}

void getNextToken() {
  tokenCurrentChar = skipWhiteSpace(tokenCurrentChar);
  tokenCurrentChar = skipComment(tokenCurrentChar);

  token.value[0] = '\0';
  tokenTerminatorChar = '\0';
  token.type = TokenEnd;
  token.isColour = false;

  if (!tokenCurrentChar) {
    tokenTerminatorChar = tokenCurrentChar;
    return;
  }

  if (isEqual())
    return;

  if (isAlphaNumeric(token.value))
    return;

  unexpectedToken();
}

void openTokenStream() {
  openFileStream();

  currentLineNumber = 1;

  char nextChar = getNext();
  tokenCurrentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
  token.isColour = false;
}

void closeTokenStream() { closeFileStream(); }
