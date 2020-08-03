#include "systemstate.h"
#include "datatypes.h"

#define MAX_PROGRAM_SIZE 6144
extern byte programStorage[MAX_PROGRAM_SIZE];
uint16_t    currentAddress;
byte *      programPtr;
