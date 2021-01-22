#ifndef __CONFIGREADER
#define __CONFIGREADER

#include "token.h"
#include <stdbool.h>

typedef enum {
  /* first 15 are colour values as per tms.h */
  TokenEquals = 32,
  TokenAlphanumeric,
  TokenUnknown,
  TokenEnd,
  TokenColour,
  TokenKey,
  TokenIdentifier,
  TokenDash,
  TokenComma,
  TokenNumber,
  TokenCtrl,
  TokenBtn,
  TokenDirection
} TokenType;

#define MAX_LINE_LENGTH 128

typedef struct TokenStruct {
  char      currentLine[MAX_LINE_LENGTH];
  char      value[MAX_LINE_LENGTH];
  TokenType type;
  long      number;
  bool      isColour : 1;
} Token;

extern Token token;

#endif
