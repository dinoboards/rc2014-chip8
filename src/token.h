#ifndef __TOKEN
#define __TOKEN

#include "datatypes.h"
#include <string.h>

extern char       tokenCurrentChar;
extern char       tokenTerminatorChar;
extern int        currentLineNumber;
extern bool       isOnlyAlphaNumeric;
extern bool       isOnlyLetters;
extern bool       isOnlyDigits;
extern byte       currentLineIndex;
extern const char commentChar;

extern void getNextToken(void);
extern void openTokenStream(void);
extern void closeTokenStream(void);
extern void getToLineEnd(void);
extern char getNext(void);
extern char _getNext(char *currentLine) __z88dk_fastcall;
extern void tokeniseAlphaNumericString(void);
extern void getToLineEnd(void);

extern char getNextChar(void);
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
