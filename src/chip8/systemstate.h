
#ifndef __SYSTEMSTATE
#define __SYSTEMSTATE

#include "datatypes.h"

#define registers  ((byte *)(0x100))
#define _registers 0x100

extern uint16_t *chip8PC;
extern uint16_t  registerI;
extern uint8_t   firstNibble;
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

#define UNSET_TIMER_MODE     0
#define SIMULATED_TIMER_MODE 1
#define TMS_TIMER_MODE       2
#define HBIOS_TIMER_MODE     3
extern byte     timerMode;
extern uint16_t timerTick;

extern uint16_t instructionCostCounter;

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
#define nibble3rd ((uint8_t)(currentInstruction >> 12))

#endif
