#include "chip8asm/tokenreader.h"
#include "charconstants.h"
#include "chartesters.h"
#include "cpm.h"
#include "datatypes.h"
#include "error.h"
#include "exit.h"
#include "filereader.h"
#include "hbios.h"
#include <string.h>

Token        token;
static char *pTokenValue;
static bool  isOnlyAlphaNumeric;
static bool  isOnlyLetters;
static byte  currentLineIndex;

static bool newLineStarted = true;

static inline char getNext() {
  const char result = getNextChar();

  token.currentLine[currentLineIndex] = '\0';

  if (!result)
    return '\0';

  if (newLineStarted) {
    lineNumber++;
    token.currentLine[0] = '\0';
    currentLineIndex = 0;
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
  char nextChar = tokenCurrentChar;

  if (!isDigit(nextChar))
    return false;

  char *pTokenValue = token.value;

  while (isCharExpression(nextChar)) {
    *pTokenValue++ = nextChar;
    nextChar = getNext();
  }

  *pTokenValue = '\0';
  tokenTerminatorChar = nextChar;
  tokenCurrentChar = nextChar;
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
  tokenMap("sne", InstructionSne);
  tokenMap("jp", InstructionJp);
  tokenMap("skp", InstructionSkp);
  tokenMap("sknp", InstructionSknp);
  tokenMap("rnd", InstructionRnd);
  tokenMap("and", InstructionAnd);
  tokenMap("shr", InstructionShr);
  tokenMap("shl", InstructionShl);
  tokenMap("sub", InstructionSub);
  tokenMap("xor", InstructionXor);
  tokenMap("bcd", InstructionBcd);
  tokenMap("ldf", InstructionLdf);
  tokenMap("key", InstructionKey);

  token.isInstruction = false;
  token.type = TokenAlphanumeric;
}

static void tokeniseAlphaNumericString() {
  if (tokenCurrentChar == ':' && isOnlyAlphaNumeric) {
    token.type = TokenLabel;
    tokenCurrentChar = getNext();
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

static bool isIndexedI() {
  if (tokenCurrentChar != '[')
    return false;

  tokenCurrentChar = getNextChar();
  tokenCurrentChar = skipWhiteSpace(tokenCurrentChar);

  if (tokenCurrentChar != 'I' && tokenCurrentChar != 'i')
    return false;

  tokenCurrentChar = getNextChar();
  tokenCurrentChar = skipWhiteSpace(tokenCurrentChar);

  const bool b = tokenCurrentChar == ']';

  tokenCurrentChar = getNextChar();

  token.value[0] = '[';
  token.value[1] = 'I';
  token.value[2] = ']';
  token.value[3] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = RegisterIndexedI;

  return b;
}

static bool isAlphaNumeric() {
  if (!isCharAlpha(tokenCurrentChar))
    return false;

  pTokenValue = token.value;

  isOnlyAlphaNumeric = true;
  isOnlyLetters = true;

  while (isCharExpression(tokenCurrentChar)) {
    if (!isCharAlpha(tokenCurrentChar))
      isOnlyAlphaNumeric = false;

    if (!isCharLetter(tokenCurrentChar))
      isOnlyLetters = false;

    *pTokenValue++ = tokenCurrentChar;
    tokenCurrentChar = getNext();
  }

  *pTokenValue = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  tokeniseAlphaNumericString();

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
  token.isInstruction = false;
  token.isVRegister = false;

  if (!tokenCurrentChar) {
    tokenTerminatorChar = tokenCurrentChar;
    return;
  }

  if (isDecimalNumber())
    return;

  if (isIndexedI())
    return;

  if (isAlphaNumeric())
    return;

  if (isComma())
    return;

  logError("Unexpected token '%c'\r\n", tokenCurrentChar);
  errorExit();
}

void getToLineEnd() {
  char c = tokenCurrentChar;
  while (c && c != '\n') {
    c = getNext();
  }
}

void openTokenStream() {
  openFileStream();
  lineNumber = 1;

  char nextChar = getNext();
  tokenCurrentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
}

void closeTokenStream() { closeFileStream(); }
