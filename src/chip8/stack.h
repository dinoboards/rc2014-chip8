
#ifndef __STACK
#define __STACK
#include "datatypes.h"

extern void pushPc();
extern bool popPc();

/* only required public to allow tests */
#define MAX_STACK 16
extern uint16_t stack[MAX_STACK];
extern byte     stackIndex;

#endif
