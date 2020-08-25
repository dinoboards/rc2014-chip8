
#ifndef __SYSTEMSTATE
#define __SYSTEMSTATE

#include "datatypes.h"

extern uint16_t *chip8PC;
extern byte      registers[16];
extern uint16_t  registerI;
extern uint8_t   firstNibble;
extern uint8_t   secondNibble;
extern uint8_t   thirdNibble;
extern uint8_t   lowByte;
extern uint8_t   fourthNibble;
extern uint16_t  addr;

extern byte     currentPressedKey;
extern uint16_t currentKeyTimeout;
extern bool     keyPressed;

extern byte soundTimer;
extern byte delayTimer;

typedef struct {
  bool isSerial : 1;
  bool isTms : 1;
  bool isHelp : 1;
} CliSwitches;

extern CliSwitches CommandSwitches;

#ifdef DIAGNOSTICS_ON
extern bool startCounting;
extern int  drawCount;
extern bool startDebugging;
#endif

#endif
