#include "configreader.h"
#include "charconstants.h"
#include "chartesters.h"
#include "cpm.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filereader.h"
#include "hbios.h"
#include "tms.h"
#include <string.h>

Token        token;
static char *pTokenValue;
static bool  isOnlyAlphaNumeric;
static bool  isOnlyLetters;
static byte  currentLineIndex;

static bool newLineStarted = true;

static inline char getNext() {
  const char result = getNextChar();

  token.currentLine[currentLineIndex] = '\0';

  if (!result)
    return '\0';

  if (newLineStarted) {
    lineNumber++;
    token.currentLine[0] = '\0';
    currentLineIndex = 0;
    newLineStarted = false;
  }

  if (result != '\r' && result != '\n') {
    token.currentLine[currentLineIndex++] = result;
    token.currentLine[currentLineIndex] = '\0';
  }

  newLineStarted = result == '\n';
  return result;
}

static char skipWhiteSpace(char nextChar) {
  if (!nextChar && nextChar != SPACE && nextChar != NEWLINE)
    return nextChar;

  while (nextChar && (nextChar == SPACE || nextChar == NEWLINE)) {
    nextChar = getNext();
  }

  return nextChar;
}

static char skipComment(char nextChar) {
  if (nextChar != '#')
    return nextChar;

  do {
    while (nextChar && nextChar != NEWLINE) {
      nextChar = getNext();
    }

    nextChar = skipWhiteSpace(nextChar);
  } while (nextChar == '#');

  return nextChar;
}

static bool tokenEquals(const char *pTest) __z88dk_fastcall { return strcasecmp(token.value, pTest) == 0; }

#define tokenMap(a, b)  \
  if (tokenEquals(a)) { \
    token.type = b;     \
    return;             \
  }

static void tokeniseAlphaNumericString() {
  tokenMap("color0", TokenColour0);
  tokenMap("color1", TokenColour1);

  token.isColour = true;
  tokenMap("darkblue", COL_DRKBLUE);
  tokenMap("black", COL_BLACK);
  token.isColour = false;

  token.type = TokenUnknown;
}

static bool isAlphaNumeric() {
  if (!isCharAlpha(tokenCurrentChar))
    return false;

  pTokenValue = token.value;

  isOnlyAlphaNumeric = true;
  isOnlyLetters = true;

  while (isCharExpression(tokenCurrentChar)) {
    if (!isCharAlpha(tokenCurrentChar))
      isOnlyAlphaNumeric = false;

    if (!isCharLetter(tokenCurrentChar))
      isOnlyLetters = false;

    *pTokenValue++ = tokenCurrentChar;
    tokenCurrentChar = getNext();
  }

  *pTokenValue = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  tokeniseAlphaNumericString();

  return true;
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

  if (isAlphaNumeric())
    return;

  unexpectedToken();
}

void getToLineEnd() {
  char c = tokenCurrentChar;
  while (c && c != '\n') {
    c = getNext();
  }
}

void openTokenStream() {
  openFileStream();

  lineNumber = 1;

  char nextChar = getNext();
  tokenCurrentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
  token.isColour = false;
}

void closeTokenStream() { closeFileStream(); }
