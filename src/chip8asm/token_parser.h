#ifndef __TOKEN_PARSER
#define __TOKEN_PARSER

#include "datatypes.h"

#define REGISTER_I 17

extern byte     expectToBeByte(void);
extern byte     expectToBeNibble(void);
extern byte     expectToBeVRegister(void);
extern uint16_t expectToBeInt(void);
extern uint16_t expectToBeInt16(void);
extern void     expectToBeComma(void);
extern void     expectToBeIRegister(void);
extern void     expectToBeIndexedI(void);
extern void     expectToBeOneOfVxOrIOrStOrDt(void);
extern void     expectToBeOneOfVxOrIOrIndexedIOrStOrDt(void);
extern void     expectToBeVxOrIRegister(void);
extern void     expectRangeOperator(void);

extern void expectToBeST(void);
extern void expectToBeDT(void);

extern void expectToBeDown(void);
extern void expectToBeUp(void);
extern void expectToBeLeft(void);
extern void expectToBeRight(void);

#define currentIsIRegister()     (token.type == RegisterI)
#define currentIsST()            (token.type == RegisterST)
#define currentIsVRegister()     (token.isVRegister)
#define currentIsDT()            (token.type == RegisterDT)
#define currentIsIndexedI()      (token.type == RegisterIndexedI)
#define currentIsComma()         (token.type == TokenComma)
#define currentIsBCD()           (token.type == TokenBCD)
#define currentIsRangeOperator() (token.type == TokenRangeOperator)
#define currentIsDown()          (token.type == TokenDown)
#define currentIsUp()            (token.type == TokenUp)
#define currentIsLeft()          (token.type == TokenLeft)
#define currentIsRight()         (token.type == TokenRight)

#endif
