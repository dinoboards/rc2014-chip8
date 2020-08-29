#ifndef __CONFIGREADER
#define __CONFIGREADER

#include <stdbool.h>

typedef enum {
  /* first 15 are colour values as per tms.h */
  TokenEquals = 32,
  TokenAlphanumeric,
  TokenUnknown,
  TokenEnd,
  TokenColour0,
  TokenColour1
} TokenType;

#define MAX_TOKEN_LENGTH 32

typedef struct TokenStruct {
  char      currentChar;
  char      currentLine[MAX_TOKEN_LENGTH];
  char      value[MAX_TOKEN_LENGTH];
  char      terminatorChar;
  TokenType type;
  bool      isColour : 1;
} Token;

extern void getNextToken();
extern void openTokenStream();
extern void closeTokenStream();
extern void getToLineEnd();

extern Token token;
extern int   lineNumber;

#define getCurrentLineNumber() lineNumber

#endif
