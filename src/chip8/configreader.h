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
  TokenColour0,
  TokenColour1,
  TokenKey,
  TokenIdentifier,
  TokenDash
} TokenType;

#define MAX_TOKEN_LENGTH 32

typedef struct TokenStruct {
  char      currentLine[MAX_TOKEN_LENGTH];
  char      value[MAX_TOKEN_LENGTH];
  TokenType type;
  bool      isColour : 1;
} Token;

extern Token token;

#endif
