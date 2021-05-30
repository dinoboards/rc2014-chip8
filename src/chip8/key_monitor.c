#include "key_monitor.h"
#include "charconstants.h"
#include "msx.h"
#include "msx_keyboard.h"
#include "systemstate.h"
#include "timers.h"
#include "ym2149.h"
#include <stdio.h>
#include <string.h>

static uint16_t lastCheckTime = 0;
inline uint8_t  toLower(uint8_t c) { return ((c >= 'A' && c <= 'Z')) ? c + ('a' - 'A') : c; }

bool checkForKeyPresses() {
  if (JIFFY % 0x2)
    return true;

  if (JIFFY == lastCheckTime)
    return true;

  lastCheckTime = JIFFY;

  return true;
}

KeyConfiguration *pConfig;
KeyConfiguration  config;

uint8_t currentDirection1;
uint8_t currentButtons1;
uint8_t currentDirection2;
uint8_t currentButtons2;

void loadControllerStates() {
  currentDirection1 = getControllerDirection(1);
  currentButtons1 = getControllerButton(1) | (getControllerButton(3) << 1);
  currentDirection2 = getControllerDirection(2);
  currentButtons2 = getControllerButton(2) | (getControllerButton(4) << 1);
}

#define matchingSerialChar() (IS_KEY_PRESSED(config.matrixRow, config.matrixMask))
#define matchingDirection(n) (isYm2149 && config.controllerId == (n - 1) && config.controllerDirection == currentDirection##n)
#define matchingButtons(n)   (isYm2149 && config.controllerId == (n - 1) && ((currentButtons##n & config.controllerButtons) == config.controllerButtons))

uint8_t isKeyDown(const uint8_t c) __z88dk_fastcall {
  pConfig = gameKeys;

  if (isYm2149)
    loadControllerStates();

  while (pConfig != &gameKeys[gameKeyCount]) {
    memcpy(&config, pConfig, sizeof(KeyConfiguration));

    if (config.hexCode != c)
      goto next;

    switch (config.type) {
    case KC_ASCII:
      if (matchingSerialChar())
        return true;
      break;

    case KC_CTRL_DIR:
      if (matchingDirection(1) || matchingDirection(2))
        return true;
      break;

    case KC_CTRL_BTNS:
      if (matchingButtons(1) || matchingButtons(2))
        return true;
      break;
    }

  next:
    pConfig++;
  }

  return false;
}

uint8_t currentKey() {
  pConfig = gameKeys;

  if (isYm2149)
    loadControllerStates();

  while (pConfig != &gameKeys[gameKeyCount]) {
    memcpy(&config, pConfig, sizeof(KeyConfiguration));

    switch (config.type) {
    case KC_ASCII:
      if (matchingSerialChar())
        return config.hexCode;
      break;

    case KC_CTRL_DIR:
      if (matchingDirection(1) || matchingDirection(2))
        return config.hexCode;
      break;

    case KC_CTRL_BTNS:
      if (matchingButtons(1) || matchingButtons(2))
        return config.hexCode;
      break;
    }

    pConfig++;
  }

  return 255;
}
