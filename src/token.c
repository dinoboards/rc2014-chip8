#include "token.h"
#include "charconstants.h"
#include "chartesters.h"
#include "filereader.h"

int  currentLineNumber;
char tokenCurrentChar;
char tokenTerminatorChar;
bool isOnlyAlphaNumeric;
bool isOnlyLetters;
bool isOnlyDigits;
byte currentLineIndex;

static char *pTokenValue;
static bool  newLineStarted = true;

char getNextChar(void) {
  char c = CR;
  while (c == CR)
    c = getNextCharRaw();
  return c;
}

char _getNext(char *currentLine) __z88dk_fastcall {
  const char result = getNextChar();

  currentLine[currentLineIndex] = '\0';

  if (!result)
    return '\0';

  if (newLineStarted) {
    currentLineNumber++;
    currentLine[0] = '\0';
    currentLineIndex = 0;
    newLineStarted = false;
  }

  if (result != '\r' && result != '\n') {
    currentLine[currentLineIndex++] = result;
    currentLine[currentLineIndex] = '\0';
  }

  newLineStarted = result == '\n';
  return result;
}

char skipWhiteSpace(char nextChar) __z88dk_fastcall {
  if (!nextChar && nextChar != SPACE && nextChar != NEWLINE)
    return nextChar;

  while (nextChar && (nextChar == SPACE || nextChar == NEWLINE)) {
    nextChar = getNext();
  }

  return nextChar;
}

char skipComment(char nextChar) __z88dk_fastcall {
  if (nextChar != commentChar)
    return nextChar;

  do {
    while (nextChar && nextChar != NEWLINE) {
      nextChar = getNext();
    }

    nextChar = skipWhiteSpace(nextChar);
  } while (nextChar == commentChar);

  return nextChar;
}

bool isAlphaNumeric(const char *p) __z88dk_fastcall {
  if (!isCharAlpha(tokenCurrentChar))
    return false;

  pTokenValue = (char *)p;

  isOnlyAlphaNumeric = true;
  isOnlyLetters = true;
  isOnlyDigits = true;

  while (isCharExpression(tokenCurrentChar)) {
    if (!isCharAlpha(tokenCurrentChar))
      isOnlyAlphaNumeric = false;

    if (!isCharLetter(tokenCurrentChar))
      isOnlyLetters = false;

    if (!isDigit(tokenCurrentChar))
      isOnlyDigits = false;

    *pTokenValue++ = tokenCurrentChar;
    tokenCurrentChar = getNext();
  }

  *pTokenValue = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  tokeniseAlphaNumericString();

  return true;
}

void getToLineEnd(void) {
  char c = tokenCurrentChar;
  while (c && c != '\n') {
    c = getNext();
  }
}
