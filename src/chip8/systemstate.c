#include "systemstate.h"
#include "datatypes.h"
#include "tms.h"
#include <stdbool.h>

uint16_t *chip8PC;
uint16_t  registerI;

uint8_t firstNibble;
uint8_t lowByte;
uint8_t fourthNibble;

byte     currentPressedKey;
uint16_t currentKeyTimeout;
bool     keyPressed;

byte soundTimer;
byte delayTimer;

CliSwitches CommandSwitches;

byte gameColours[2] = {COL_BLACK, COL_DRKGREEN};

byte videoResMode = VideoResModeLow;
byte videoPixelWidth;
byte videoPixelHeight;
byte videoPixelWidthMask;
byte videoPixelHeightMask;

bool     useSimulatedTimer = false;
uint16_t instructionCostCounter;

bool     audioActive = false;
uint16_t audioPeriod;

uint16_t currentInstruction;

uint8_t timerSkipFlag = 0;

#ifdef DIAGNOSTICS_ON
/* diagnostic state */
bool startCounting = false;
int  drawCount = 0;
bool startDebugging = true;
#endif
