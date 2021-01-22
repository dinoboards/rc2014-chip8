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

byte             gameColours[4] = {COL_BLACK, COL_DRKGREEN, COL_DRKRED, COL_GREY};
KeyConfiguration gameKeys[64] = {{0, 0, '0'}, {1, 0, '1'}, {2, 0, '2'},  {3, 0, '3'},  {4, 0, '4'},  {5, 0, '5'},  {6, 0, '6'},  {7, 0, '7'},
                                 {8, 0, '8'}, {9, 0, '9'}, {10, 0, 'a'}, {11, 0, 'b'}, {12, 0, 'c'}, {13, 0, 'd'}, {14, 0, 'e'}, {15, 0, 'f'}};

uint8_t gameKeyCount = 16;

byte videoResMode = VideoResModeLow;
byte videoPixelWidth;
byte videoPixelHeight;
byte videoPixelWidthMask;
byte videoPixelHeightMask;

bool        audioActive = false;
uint16_t    audioPeriod;
audioSystem installedAudioSystem = AS_NONE;

uint16_t currentInstruction;
