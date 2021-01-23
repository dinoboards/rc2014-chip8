#include "key_monitor.h"
#include "charconstants.h"
#include "keys.h"
#include "systemstate.h"
#include "timers.h"
#include "ym2149.h"

static uint16_t lastCheckTime = 0;
inline uint8_t  toLower(uint8_t c) { return ((c >= 'A' && c <= 'Z')) ? c + ('a' - 'A') : c; }

bool checkForKeyPresses() {
  if (timerTick % 0x2)
    return true;

  if (timerTick == lastCheckTime)
    return true;

  lastCheckTime = timerTick;

  if (!keyReady()) {
    if (!keyPressed)
      return true;

    if (timerTick < currentKeyTimeout)
      return true;

    currentPressedKey = '\0';
    keyPressed = false;
    return true;
  }

  currentPressedKey = toLower(getKey());

  if (currentPressedKey == CTRL_Z) {
    return false;
  }

  currentKeyTimeout = timerTick + 5;
  keyPressed = true;

  return true;
}

KeyConfiguration *pConfig;

uint8_t currentDirection;
uint8_t currentButtons;

uint8_t isKeyDown(uint8_t c) __z88dk_fastcall {
  pConfig = gameKeys;

  if (isYm2149) {
    currentDirection = getControllerDirection(1);
    currentButtons = getControllerButton(1) | (getControllerButton(3) << 1);
  }

  while (pConfig <= &gameKeys[GAME_KEYS_MAX - 1]) {
    if (pConfig->hexCode != c)
      goto next;

    if (pConfig->type == KC_ASCII && keyPressed && pConfig->asciiKeyChar == currentPressedKey)
      return true;

    if (isYm2149 && pConfig->type == KC_CTRL_DIR && pConfig->controllerDirection == currentDirection)
      return true;

    if (isYm2149 && pConfig->type == KC_CTRL_BTNS) {
      if ((currentButtons & pConfig->controllerButtons == currentButtons))
        return true;
    }

  next:
    pConfig++;
  }

  return false;
}

uint8_t currentKey() {
  pConfig = gameKeys;

  if (isYm2149) {
    currentDirection = getControllerDirection(1);
    currentButtons = getControllerButton(1) | getControllerButton(3) < 1;
  }

  while (pConfig <= &gameKeys[GAME_KEYS_MAX - 1]) {
    if (keyPressed && pConfig->type == KC_ASCII && pConfig->asciiKeyChar == currentPressedKey)
      return pConfig->hexCode;

    if (isYm2149) {
      if (pConfig->type == KC_CTRL_DIR && pConfig->controllerDirection == currentDirection)
        return pConfig->hexCode;

      if ((pConfig->type == KC_CTRL_BTNS)) {
        if ((currentButtons & pConfig->controllerButtons == currentButtons))
          return pConfig->hexCode;
      }
    }

    pConfig++;
  }

  return 255;
}
