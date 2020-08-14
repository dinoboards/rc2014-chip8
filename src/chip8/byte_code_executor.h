
#ifndef __BYTE_CODE_EXECUTOR
#define __BYTE_CODE_EXECUTOR

#include "datatypes.h"

#define programStorage ((uint16_t *)0x200)
extern uint16_t invertByteOrder(uint16_t word) __naked __z88dk_fastcall;
extern bool     executeSingleInstruction();
extern void     initSystemState();

#endif
