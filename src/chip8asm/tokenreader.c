#include "chip8asm/tokenreader.h"
#include "charconstants.h"
#include "chartesters.h"
#include "chip8asm/filereader.h"
#include "cpm.h"
#include "datatypes.h"
#include "error.h"
#include "exit.h"
#include "hbios.h"
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
  if (newLineStarted) {
    lineNumber++;
    token.currentLine[0] = '\0';
    currentLineIndex = 0;
  } else {
    if (result != '\r' && result != '\n') {
      token.currentLine[currentLineIndex++] = result;
      token.currentLine[currentLineIndex] = '\0';
    }
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
  if (nextChar != ';')
    return nextChar;

  do {
    while (nextChar && nextChar != NEWLINE) {
      nextChar = getNext();
    }

    nextChar = skipWhiteSpace(nextChar);
  } while (nextChar == ';');

  return nextChar;
}

static bool isDecimalNumber() {
  char nextChar = token.currentChar;

  if (!isDigit(nextChar))
    return false;

  char *pTokenValue = token.value;

  while (isCharExpression(nextChar)) {
    *pTokenValue++ = nextChar;
    nextChar = getNext();
  }

  *pTokenValue = '\0';
  token.terminatorChar = nextChar;
  token.currentChar = nextChar;
  token.type = TokenExpression;

  return true;
}

static bool tokenEquals(const char *pTest) __z88dk_fastcall { return strcasecmp(token.value, pTest) == 0; }

#define tokenMap(a, b)  \
  if (tokenEquals(a)) { \
    token.type = b;     \
    return;             \
  }

static void testForInstructions() {
  token.isInstruction = true;

  tokenMap("ret", InstructionRet);
  tokenMap("ld", InstructionLd);
  tokenMap("db", InstructionDb);
  tokenMap("drw", InstructionDrw);
  tokenMap("call", InstructionCall);
  tokenMap("cls", InstructionCls);
  tokenMap("add", InstructionAdd);
  tokenMap("se", InstructionSe);
  tokenMap("jp", InstructionJp);
  tokenMap("skp", InstructionSkp);

  token.isInstruction = false;
  token.type = TokenAlphanumeric;
}

static void tokeniseAlphaNumericString() {
  if (token.currentChar == ':' && isOnlyAlphaNumeric) {
    token.type = TokenLabel;
    token.currentChar = getNext();
    return;
  }

  if (isOnlyLetters) {
    tokenMap("st", RegisterST);
    tokenMap("dt", RegisterDT);
    token.isVRegister = true;
    tokenMap("va", RegisterVA);
    tokenMap("vb", RegisterVB);
    tokenMap("vc", RegisterVC);
    tokenMap("vd", RegisterVD);
    tokenMap("ve", RegisterVE);
    tokenMap("vf", RegisterVF);
    token.isVRegister = false;

    tokenMap("i", RegisterI);
    testForInstructions();
    return;
  }

  if (isOnlyAlphaNumeric) {
    token.isVRegister = true;
    tokenMap("v0", RegisterV0);
    tokenMap("v1", RegisterV1);
    tokenMap("v2", RegisterV2);
    tokenMap("v3", RegisterV3);
    tokenMap("v4", RegisterV4);
    tokenMap("v5", RegisterV5);
    tokenMap("v6", RegisterV6);
    tokenMap("v7", RegisterV7);
    tokenMap("v8", RegisterV8);
    tokenMap("v9", RegisterV9);
    token.isVRegister = false;

    token.type = TokenAlphanumeric;
    return;
  }

  token.type = TokenExpression;
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

static bool isComma() {
  if (token.currentChar != ',')
    return false;

  token.currentChar = getNext();

  token.value[0] = ',';
  token.value[1] = '\0';
  token.terminatorChar = token.currentChar;
  token.currentChar = token.currentChar;
  token.type = TokenComma;

  return true;
}

void getNextToken() {
  token.currentChar = skipWhiteSpace(token.currentChar);
  token.currentChar = skipComment(token.currentChar);

  token.value[0] = '\0';
  token.terminatorChar = '\0';
  token.type = TokenEnd;
  token.isInstruction = false;
  token.isVRegister = false;

  if (!token.currentChar) {
    token.terminatorChar = token.currentChar;
    return;
  }

  if (isDecimalNumber())
    return;

  if (isAlphaNumeric())
    return;

  if (isComma())
    return;

  logError("Unexpected token '%c'\r\n", token.currentChar);
  errorExit();
}

void getToLineEnd() {
  char c = token.currentChar;
  while (c != '\n')
    c = getNext();
}

void openTokenStream() {
  openFileStream();
  lineNumber = 0;

  char nextChar = getNext();
  token.currentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
}

void closeTokenStream() { closeFileStream(); }
