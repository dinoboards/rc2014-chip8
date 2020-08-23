#ifndef __TOKEN_PARSER
#define __TOKEN_PARSER

#include "datatypes.h"

#define REGISTER_I 17

extern byte     expectToBeByte();
extern byte     expectToBeNibble();
extern byte     expectToBeVRegister();
extern uint16_t expectToBeInt();
extern void     expectToBeComma();
extern void     expectToBeIRegister();
extern void     expectToBeIndexedI();
extern void     expectToBeOneOfVxOrIOrStOrDt();
extern void     expectToBeOneOfVxOrIOrIndexedIOrStOrDt();
extern void     expectToBeVxOrIRegister();

extern void expectToBeST();
extern void expectToBeDT();

#define currentIsIRegister() (token.type == RegisterI)
#define currentIsST()        (token.type == RegisterST)
#define currentIsVRegister() (token.isVRegister)
#define currentIsDT()        (token.type == RegisterDT)
#define currentIsIndexedI()  (token.type == RegisterIndexedI)
#define currentIsComma()     (token.type == TokenComma)
#define currentIsBCD()       (token.type == TokenBCD)

#endif
