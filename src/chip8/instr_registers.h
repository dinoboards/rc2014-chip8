
#ifndef __INSTR_REGISTERS
#define __INSTR_REGISTERS

#define ldVxByte()  registers[secondNibble] = lowByte
#define ldIAddr()   registerI = addr
#define addVxByte() registers[secondNibble] += lowByte

#endif
