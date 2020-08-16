#ifndef __TOKEN_PARSER
#define __TOKEN_PARSER

#include "datatypes.h"

#define REGISTER_I 17
#define getNext()  getNextToken()

extern byte     expectToBeByte();
extern byte     expectToBeNibble();
extern byte     expectToBeVRegister();
extern uint16_t expectToBeInt();
extern void     expectToBeComma();
extern void     expectToBeIRegister();
extern void     expectToBeOneOfVxOrIOrStOrDt();
extern void     expectToBeVxOrIRegister();

extern void expectToBeST();
extern void expectToBeDT();

#define currentIsIRegister() (token.type == RegisterI)
#define currentIsST()        (token.type == RegisterST)
#define currentIsVRegister() (token.isVRegister)
#define currentIsDT()        (token.type == RegisterDT)

#endif
