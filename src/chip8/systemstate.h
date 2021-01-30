
#ifndef __SYSTEMSTATE
#define __SYSTEMSTATE

#include "datatypes.h"

#define registers  ((byte *)(0x100))
#define _registers 0x100

extern uint16_t *chip8PC;
extern uint16_t  registerI;
extern uint8_t   fourthNibble;

#define lowByte  (*(((uint8_t *)&currentInstruction) + 1))
#define highByte (*(((uint8_t *)&currentInstruction)))

#define firstNibble (highByte >> 4)
#define nibble2nd   (highByte & 0xF)
#define nibble3rd   (lowByte >> 4)

#define addr (lowByte + (((uint16_t)nibble2nd) << 8))

extern byte     currentPressedKey;
extern uint16_t currentKeyTimeout;
extern bool     keyPressed;

extern byte soundTimer;
extern byte delayTimer;

typedef struct {
  bool     isHelp : 1;
  uint16_t delayFactor;
} CliSwitches;

extern CliSwitches CommandSwitches;

typedef enum { KC_ASCII, KC_CTRL_DIR, KC_CTRL_BTNS } keyConfigurationDirection;

typedef struct KeyConfigurationStruct {
  uint8_t                   hexCode : 4; /* Code for chip up keypress 0 - 9, a-f */
  keyConfigurationDirection type : 2;    // 0 -> ascii, 1 -> controller direction, 2 -> controller button
  union {
    char asciiKeyChar; // serial key char to map to hexCode
    struct {
      uint8_t controllerId : 1; // controller number (0 or 1)
      union {
        uint8_t controllerDirection : 5; // 1 to 8 to indicate a controller direction flag
        uint8_t controllerButtons : 2;
      };
    };
  };
} KeyConfiguration;

typedef enum {
  CONTROLLER_DIRECTION_UP = 1,
  CONTROLLER_DIRECTION_UP_RIGHT,
  CONTROLLER_DIRECTION_RIGHT,
  CONTROLLER_DIRECTION_DOWN_RIGHT,
  CONTROLLER_DIRECTION_DOWN,
  CONTROLLER_DIRECTION_DOWN_LEFT,
  CONTROLLER_DIRECTION_LEFT,
  CONTROLLER_DIRECTION_UP_LEFT

} ControllerDirection;

#define GAME_KEYS_MAX 64
extern byte             gameColours[4];
extern KeyConfiguration gameKeys[GAME_KEYS_MAX];
extern uint8_t          gameKeyCount; // TODO: watch for overflow of gameKeys

extern uint16_t delayFactor;

#define VideoResModeLow  0
#define VideoResModeHigh 1

extern byte videoResMode;
extern byte videoPixelWidth;
extern byte videoPixelHeight;
extern byte videoPixelWidthMask;
extern byte videoPixelHeightMask;

extern uint16_t timerTick;

extern bool     audioActive;
extern uint16_t audioPeriod;
typedef enum _audioSystem { AS_NONE, AS_YM2149, AS_HBIOS } audioSystem;
extern audioSystem installedAudioSystem;
extern bool        isYm2149;

extern uint16_t currentInstruction;

#define HIRES_WIDTH       128
#define HIRES_WIDTH_MASK  255
#define HIRES_HEIGHT      64
#define HIRES_HEIGHT_MASK 127

#define LORES_WIDTH  64
#define LORES_HEIGHT 32

#endif
