#include "systemstate.h"
#include "datatypes.h"
#include <stdbool.h>

uint16_t *chip8PC;
byte      registers[16];
uint16_t  registerI;

uint8_t  firstNibble;
uint8_t  secondNibble;
uint8_t  thirdNibble;
uint8_t  lowByte;
uint8_t  fourthNibble;
uint16_t addr;

byte     currentPressedKey;
uint16_t currentKeyTimeout;
bool     keyPressed;
