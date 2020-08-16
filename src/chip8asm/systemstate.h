#ifndef __SYSTEMSTATE
#define __SYSTEMSTATE

#include "datatypes.h"

#define MAX_PROGRAM_SIZE 6144
extern byte     programStorage[MAX_PROGRAM_SIZE];
extern uint16_t currentAddress;
extern byte *   programPtr;
extern byte     parseCount;

#endif
