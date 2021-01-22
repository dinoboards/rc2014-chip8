#include "key_monitor.h"
#include "charconstants.h"
#include "keys.h"
#include "systemstate.h"
#include "timers.h"

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

uint8_t isKeyDown(uint8_t c) __z88dk_fastcall { return (keyPressed && c == currentKey()); }

inline uint8_t toLower(uint8_t c) { return ((c >= 'A' && c <= 'Z')) ? c + ('a' - 'A') : c; }

KeyConfiguration *pConfig;

uint8_t currentKey() {
  if (!keyPressed)
    return 255;

  pConfig = gameKeys;
  const uint8_t c = toLower(currentPressedKey);

  while (pConfig <= &gameKeys[GAME_KEYS_MAX - 1]) {
    if (pConfig->type == KC_ASCII && pConfig->asciiKeyChar == c)
      return pConfig->hexCode;

    pConfig++;
  }

  return 255;
}
