#ifndef __TOKEN_PARSER
#define __TOKEN_PARSER

#include "datatypes.h"

#define REGISTER_I 17
#define getNext()  getNextToken()

extern byte     expectToBeByte();
extern uint16_t expectToBeInt();
extern void     expectToBeIRegister();
extern byte     expectToBeVRegister();
extern void     expectToBeSoundTimer();
extern void     expectToBeComma();
extern byte     expectToBeNibble();

#define currentIsIRegister()  (token.type == RegisterI)
#define currentIsVRegister()  (token.isVRegister)
#define currentIsSoundTimer() (token.type == SoundTimer)
#endif
