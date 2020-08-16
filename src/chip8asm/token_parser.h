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
extern void     expectToBeOneOfVxOrIOrSt();
extern void     expectToBeSoundTimer();

#define currentIsIRegister()  (token.type == RegisterI)
#define currentIsSoundTimer() (token.type == SoundTimer)
#define currentIsVRegister()  (token.isVRegister)

#endif
