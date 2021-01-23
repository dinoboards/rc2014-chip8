#include "tokenreader.h"
#include "chartesters.h"
#include "cpm.h"
#include "datatypes.h"
#include "error.h"
#include "exit.h"
#include "filereader.h"

Token      token;
const char commentChar = ';';

char getNext() { return _getNext(token.currentLine); }

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

DEF_TOKEN_EQUALS(token.value)

static void testForInstructions() {
  token.isInstruction = true;

  tokenMap("ret", InstructionRet);
  tokenMap("ld", InstructionLd);
  tokenMap("db", InstructionDb);
  tokenMap("ds", InstructionDs);
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
  tokenMap("or", InstructionOr);
  tokenMap("shr", InstructionShr);
  tokenMap("shl", InstructionShl);
  tokenMap("sub", InstructionSub);
  tokenMap("xor", InstructionXor);
  tokenMap("bcd", InstructionBcd);
  tokenMap("ldf", InstructionLdf);
  tokenMap("key", InstructionKey);
  tokenMap("subn", InstructionSubn);
  tokenMap("high", InstructionHigh);
  tokenMap("plane", InstructionPlane);
  tokenMap("audio", InstructionAudio);
  tokenMap("scrl", InstructionScrl);
  tokenMap("down", TokenDown);
  tokenMap("up", TokenUp);
  tokenMap("left", TokenLeft);

  token.isInstruction = false;
  token.type = TokenAlphanumeric;
}

void tokeniseAlphaNumericString() {
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

static bool isRangeOperator() {
  if (tokenCurrentChar != '.')
    return false;

  tokenCurrentChar = getNext();
  if (tokenCurrentChar != '.')
    return false;

  tokenCurrentChar = getNext();

  token.value[0] = '.';
  token.value[1] = '.';
  token.value[2] = '\0';
  tokenTerminatorChar = tokenCurrentChar;
  token.type = TokenRangeOperator;

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

  if (isAlphaNumeric(token.value))
    return;

  if (isComma())
    return;

  if (isRangeOperator())
    return;

  logError("Unexpected token '%c'\r\n", tokenCurrentChar);
  errorExit();
}

void openTokenStream() {
  openFileStream();
  currentLineNumber = 1;

  char nextChar = getNext();
  tokenCurrentChar = nextChar;
  token.currentLine[0] = nextChar;
  token.currentLine[1] = '\0';
  currentLineIndex = 1;
}

void closeTokenStream() { closeFileStream(); }
