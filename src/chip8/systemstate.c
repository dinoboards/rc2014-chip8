#include "systemstate.h"
#include "datatypes.h"
#include "tms.h"
#include <stdbool.h>

uint16_t *chip8PC;
uint16_t  registerI;
uint8_t   fourthNibble;

byte soundTimer;
byte delayTimer;

CliSwitches CommandSwitches;

byte             gameColours[4] = {COL_BLACK, COL_DRKGREEN, COL_DRKRED, COL_GREY};
KeyConfiguration gameKeys[64] = {{.hexCode = 0, .type = 0, .matrixRow = 0, .matrixMask = 1 << 0},  {.hexCode = 1, .type = 0, .matrixRow = 0, .matrixMask = 1 << 1},  {.hexCode = 2, .type = 0, .matrixRow = 0, .matrixMask = 1 << 2},
                                 {.hexCode = 3, .type = 0, .matrixRow = 0, .matrixMask = 1 << 3},  {.hexCode = 4, .type = 0, .matrixRow = 0, .matrixMask = 1 << 4},  {.hexCode = 5, .type = 0, .matrixRow = 0, .matrixMask = 1 << 5},
                                 {.hexCode = 6, .type = 0, .matrixRow = 0, .matrixMask = 1 << 6},  {.hexCode = 7, .type = 0, .matrixRow = 0, .matrixMask = 1 << 7},  {.hexCode = 8, .type = 0, .matrixRow = 1, .matrixMask = 1 << 0},
                                 {.hexCode = 9, .type = 0, .matrixRow = 1, .matrixMask = 1 << 1},  {.hexCode = 10, .type = 0, .matrixRow = 2, .matrixMask = 1 << 6}, {.hexCode = 11, .type = 0, .matrixRow = 2, .matrixMask = 1 << 7},
                                 {.hexCode = 12, .type = 0, .matrixRow = 3, .matrixMask = 1 << 0}, {.hexCode = 13, .type = 0, .matrixRow = 3, .matrixMask = 1 << 1}, {.hexCode = 14, .type = 0, .matrixRow = 3, .matrixMask = 1 << 2},
                                 {.hexCode = 15, .type = 0, .matrixRow = 3, .matrixMask = 1 << 3}};

uint8_t gameKeyCount = 16;

byte videoResMode = VideoResModeLow;
byte videoPixelWidth;
byte videoPixelHeight;
byte videoPixelWidthMask;
byte videoPixelHeightMask;

bool        audioActive = false;
uint16_t    audioPeriod;
audioSystem installedAudioSystem = AS_NONE;
bool        isYm2149;

uint16_t currentInstruction;
