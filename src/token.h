#ifndef __TOKEN
#define __TOKEN

#include "datatypes.h"
#include <string.h>

extern char       tokenCurrentChar;
extern char       tokenTerminatorChar;
extern int        lineNumber;
extern bool       isOnlyAlphaNumeric;
extern bool       isOnlyLetters;
extern byte       currentLineIndex;
extern const char commentChar;

#define getCurrentLineNumber() lineNumber

extern void getNextToken();
extern void openTokenStream();
extern void closeTokenStream();
extern void getToLineEnd();
extern char getNext();
extern char _getNext(char *currentLine) __z88dk_fastcall;
extern void tokeniseAlphaNumericString();
extern void getToLineEnd();

extern char getNextChar();
extern char skipWhiteSpace(char nextChar) __z88dk_fastcall;
extern char skipComment(char nextChar) __z88dk_fastcall;
extern bool tokenEquals(const char *pTest) __z88dk_fastcall;
extern bool isAlphaNumeric(const char *p) __z88dk_fastcall;

#define tokenMap(a, b)  \
  if (tokenEquals(a)) { \
    token.type = b;     \
    return;             \
  }

#define DEF_TOKEN_EQUALS(tokenValue) \
  bool tokenEquals(const char *pTest) __z88dk_fastcall { return strcasecmp(tokenValue, pTest) == 0; }

#endif
