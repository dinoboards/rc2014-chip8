
#ifndef __TMS
#define __TMS

#include "datatypes.h"

typedef struct {
  uint8_t data;
  uint8_t command;
} tms9918Registers;

extern tms9918Registers tmsRegisters;

extern void captureTmsRegisters();

#endif
