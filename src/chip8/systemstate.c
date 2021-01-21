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

byte     soundTimer;
byte     delayTimer;
uint16_t timerTick = 0;

CliSwitches CommandSwitches;

byte gameColours[4] = {COL_BLACK, COL_DRKGREEN, COL_DRKRED, COL_GREY};
char gameKeys[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

byte videoResMode = VideoResModeLow;
byte videoPixelWidth;
byte videoPixelHeight;
byte videoPixelWidthMask;
byte videoPixelHeightMask;

bool        audioActive = false;
uint16_t    audioPeriod;
audioSystem installedAudioSystem = AS_NONE;

uint16_t currentInstruction;
