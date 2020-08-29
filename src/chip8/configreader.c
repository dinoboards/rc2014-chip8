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

int lineNumber;

bool newLineStarted = true;

static char getNextChar() {
  char c = CR;
  while (c == CR)
    c = getNextCharRaw();
  return c;
}

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
  if (!isCharAlpha(token.currentChar))
    return false;

  pTokenValue = token.value;

  isOnlyAlphaNumeric = true;
  isOnlyLetters = true;

  while (isCharExpression(token.currentChar)) {
    if (!isCharAlpha(token.currentChar))
      isOnlyAlphaNumeric = false;

    if (!isCharLetter(token.currentChar))
      isOnlyLetters = false;

    *pTokenValue++ = token.currentChar;
    token.currentChar = getNext();
  }

  *pTokenValue = '\0';
  token.terminatorChar = token.currentChar;
  tokeniseAlphaNumericString();

  return true;
}

static bool isEqual() {
  if (token.currentChar != '=')
    return false;

  token.currentChar = getNext();

  token.value[0] = ',';
  token.value[1] = '\0';
  token.terminatorChar = token.currentChar;
  token.type = TokenEquals;

  return true;
}

void getNextToken() {
  token.currentChar = skipWhiteSpace(token.currentChar);
  token.currentChar = skipComment(token.currentChar);

  token.value[0] = '\0';
  token.terminatorChar = '\0';
  token.type = TokenEnd;
  token.isColour = false;

  if (!token.currentChar) {
    token.terminatorChar = token.currentChar;
    return;
  }

  if (isEqual())
    return;

  if (isAlphaNumeric())
    return;

  unexpectedToken();
}

void getToLineEnd() {
  char c = token.currentChar;
  while (c && c != '\n') {
    c = getNext();
  }
}

void openTokenStream() {
  openFileStream();

  lineNumber = 1;

  char nextChar = getNext();
  token.currentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
  token.isColour = false;
}

void closeTokenStream() { closeFileStream(); }
