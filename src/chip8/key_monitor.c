#include "key_monitor.h"
#include "charconstants.h"
#include "keys.h"
#include "systemstate.h"
#include "timers.h"
#include "ym2149.h"

static uint16_t lastCheckTime = 0;

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

  currentPressedKey = getKey();

  if (currentPressedKey == CTRL_Z) {
    return false;
  }

  currentKeyTimeout = timerTick + 5;
  keyPressed = true;

  return true;
}

uint8_t isKeyDown(uint8_t c) __z88dk_fastcall { return (c == currentKey()); }

inline uint8_t toLower(uint8_t c) { return ((c >= 'A' && c <= 'Z')) ? c + ('a' - 'A') : c; }

KeyConfiguration *pConfig;

uint8_t currentKey() {
  pConfig = gameKeys;
  const uint8_t c = toLower(currentPressedKey);
  const bool    isYm2149 = installedAudioSystem == AS_YM2149;

  const uint8_t d = isYm2149 ? getControllerDirection(1) : 0;

  while (pConfig <= &gameKeys[GAME_KEYS_MAX - 1]) {
    if (keyPressed && pConfig->type == KC_ASCII && pConfig->asciiKeyChar == c)
      return pConfig->hexCode;

    if (isYm2149 && pConfig->type == KC_CTRL_DIR && pConfig->controllerDirection == d)
      return pConfig->hexCode;

    pConfig++;
  }

  return 255;
}
