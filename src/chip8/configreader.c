#include "configreader.h"
#include "datatypes.h"
#include "error_reports.h"
#include "filereader.h"
#include "tms.h"

Token      token;
const char commentChar = '#';

char getNext() { return _getNext(token.currentLine); }

DEF_TOKEN_EQUALS(token.value)

void tokeniseAlphaNumericString() {
  tokenMap("color0", TokenColour0);
  tokenMap("color1", TokenColour1);

  token.isColour = true;
  tokenMap("black", COL_BLACK);
  tokenMap("green", COL_MEDGREEN    );
  tokenMap("lightgreen", COL_LIGGREEN    );
  tokenMap("blue", COL_DRKBLUE     );
  tokenMap("lightblue", COL_LIGBLUE     );
  tokenMap("darkred", COL_DRKRED      );
  tokenMap("cyan", COL_CYAN        );
  tokenMap("red", COL_MEDRED      );
  tokenMap("lightred", COL_LIGRED      );
  tokenMap("yellow", COL_DRKYELLOW   );
  tokenMap("lightyellow", COL_LIGYELLOW   );
  tokenMap("darkgreen", COL_DRKGREEN    );
  tokenMap("magenta", COL_MAGENTA     );
  tokenMap("grey", COL_GREY        );
  tokenMap("white", COL_WHITE       );
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
