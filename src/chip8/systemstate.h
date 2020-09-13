
#ifndef __SYSTEMSTATE
#define __SYSTEMSTATE

#include "datatypes.h"

extern uint16_t *chip8PC;
extern byte      registers[16];
extern uint16_t  registerI;
extern uint8_t   firstNibble;
extern uint8_t   thirdNibble;
extern uint8_t   lowByte;
extern uint8_t   fourthNibble;

extern byte     currentPressedKey;
extern uint16_t currentKeyTimeout;
extern bool     keyPressed;

extern byte soundTimer;
extern byte delayTimer;

typedef struct {
  bool     isSerial : 1;
  bool     isTms : 1;
  bool     isHelp : 1;
  uint16_t delayFactor;
} CliSwitches;

extern CliSwitches CommandSwitches;

extern byte gameColours[2];

extern uint16_t delayFactor;

#define VideoResModeLow  0
#define VideoResModeHigh 1

extern byte videoResMode;
extern byte videoPixelWidth;
extern byte videoPixelHeight;
extern byte videoPixelWidthMask;
extern byte videoPixelHeightMask;

extern uint16_t instructionCostCounter;
extern bool     useSimulatedTimer;

extern bool     audioActive;
extern uint16_t audioPeriod;

extern uint16_t currentInstruction;

#ifdef DIAGNOSTICS_ON
extern bool startCounting;
extern int  drawCount;
extern bool startDebugging;
#endif

#define nibble2nd ((uint8_t)currentInstruction & 0xF)
#define addr      ((currentInstruction >> 8) + (((int)nibble2nd) << 8))

#endif
