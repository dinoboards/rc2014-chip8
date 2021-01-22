#include "configreader.h"
#include "charconstants.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filereader.h"
#include "systemstate.h"
#include "tms.h"

#include "xstdlib.h"

Token      token;
const char commentChar = '#';

char getNext() { return _getNext(token.currentLine); }

DEF_TOKEN_EQUALS(token.value)

#define tokenDirectionMap(v, d)  \
  if (tokenEquals(v)) {          \
    token.type = TokenDirection; \
    token.number = d;            \
    return;                      \
  }

void tokeniseAlphaNumericString() {
  tokenMap("color", TokenColour);

  token.isColour = true;
  tokenMap("black", COL_BLACK);
  tokenMap("green", COL_MEDGREEN);
  tokenMap("lightgreen", COL_LIGGREEN);
  tokenMap("blue", COL_DRKBLUE);
  tokenMap("lightblue", COL_LIGBLUE);
  tokenMap("darkred", COL_DRKRED);
  tokenMap("cyan", COL_CYAN);
  tokenMap("red", COL_MEDRED);
  tokenMap("lightred", COL_LIGRED);
  tokenMap("yellow", COL_DRKYELLOW);
  tokenMap("lightyellow", COL_LIGYELLOW);
  tokenMap("darkgreen", COL_DRKGREEN);
  tokenMap("magenta", COL_MAGENTA);
  tokenMap("grey", COL_GREY);
  tokenMap("white", COL_WHITE);
  token.isColour = false;

  tokenMap("key", TokenKey);
  tokenMap("ctrl", TokenCtrl);
  tokenMap("btn", TokenBtn);

  if (strlen(token.value) == 1) {
    const char c = token.value[0];
    if ((c >= 'a' && c <= 'z')) {
      token.type = TokenIdentifier;
      return;
    }

    if ((c >= 'A' && c <= 'Z')) {
      token.type = TokenIdentifier;
      token.value[0] = c + ('a' - 'A'); // to lower it
      return;
    }
  }

  tokenDirectionMap("up", CONTROLLER_DIRECTION_UP);
  tokenDirectionMap("down", CONTROLLER_DIRECTION_DOWN);
  tokenDirectionMap("left", CONTROLLER_DIRECTION_LEFT);
  tokenDirectionMap("right", CONTROLLER_DIRECTION_RIGHT);

  if (tokenEquals("space")) {
    token.type = TokenIdentifier;
    token.value[0] = ' ';
    token.value[1] = 0;
    return;
  }

  if (tokenEquals("cr")) {
    token.type = TokenIdentifier;
    token.value[0] = 13;
    token.value[1] = 0;
    return;
  }

  if (isOnlyDigits) {
    token.type = TokenNumber;
    token.number = xstrtol(token.value, (char *)0, 10);
    return;
  }

  token.type = TokenUnknown;
}

static bool isEqual() {
  if (tokenCurrentChar != '=')
    return false;

  tokenCurrentChar = getNext();

  token.value[0] = '=';
  token.value[1] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = TokenEquals;

  return true;
}

static bool isDash() {
  if (tokenCurrentChar != '-')
    return false;

  tokenCurrentChar = getNext();

  token.value[0] = '-';
  token.value[1] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = TokenDash;

  return true;
}

static bool isComma() {
  if (tokenCurrentChar != ',')
    return false;

  tokenCurrentChar = getNext();

  token.value[0] = ',';
  token.value[1] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = TokenComma;

  return true;
}

void getNextToken() {
  tokenCurrentChar = skipWhiteSpace(tokenCurrentChar);
  tokenCurrentChar = skipComment(tokenCurrentChar);

  token.value[0] = '\0';
  tokenTerminatorChar = '\0';
  token.type = TokenEnd;
  token.isColour = false;

  if (!tokenCurrentChar || tokenCurrentChar == CTRL_Z) {
    tokenTerminatorChar = tokenCurrentChar;
    return;
  }

  if (isEqual())
    return;

  if (isDash())
    return;

  if (isComma())
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
