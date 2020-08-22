#ifndef __TOKENREADER
#define __TOKENREADER

#include <stdbool.h>

typedef enum {
  TokenEnd,
  TokenAlphanumeric,
  TokenComma,
  TokenLabel,
  TokenExpression,

  RegisterV0,
  RegisterV1,
  RegisterV2,
  RegisterV3,
  RegisterV4,
  RegisterV5,
  RegisterV6,
  RegisterV7,
  RegisterV8,
  RegisterV9,
  RegisterVA,
  RegisterVB,
  RegisterVC,
  RegisterVD,
  RegisterVE,
  RegisterVF,
  RegisterI,
  RegisterST,
  RegisterDT,
  RegisterIndexedI,

  InstructionRet,
  InstructionLd,
  InstructionDb,
  InstructionDrw,
  InstructionCall,
  InstructionCls,
  InstructionAdd,
  InstructionSe,
  InstructionSne,
  InstructionJp,
  InstructionSkp,
  InstructionSknp,
  InstructionRnd,
  InstructionAnd,
  InstructionShr
} TokenType;

#define MAX_TOKEN_LENGTH 256

typedef struct TokenStruct {
  char      currentChar;
  char      currentLine[MAX_TOKEN_LENGTH];
  char      value[MAX_TOKEN_LENGTH];
  char      terminatorChar;
  TokenType type;
  bool      isInstruction;
  bool      isVRegister;
} Token;

extern void getNextToken();
extern void openTokenStream();
extern void closeTokenStream();
extern void getToLineEnd();

extern Token token;
extern int   lineNumber;

#define getCurrentLineNumber() lineNumber

#endif
