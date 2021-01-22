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

  if (currentPressedKey >= '0' && currentPressedKey <= '9')
    currentPressedKey -= '0';

  else if (currentPressedKey >= 'A' && currentPressedKey <= 'Z')
    currentPressedKey += (-'A' + 10);

  else if (currentPressedKey >= 'a' && currentPressedKey <= 'z')
    currentPressedKey += (-'a' + 10);

  currentKeyTimeout = timerTick + 5;
  keyPressed = true;

  return true;
}
