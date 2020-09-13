#include "charconstants.h"
#include "keys.h"
#include "systemstate.h"
#include "timers.h"
#include "xstdio.h"

static uint16_t lastCheckTime = 0;

bool checkForKeyPresses() {

  if (currentTimerTick == lastCheckTime)
    return true;

  lastCheckTime = currentTimerTick;

  if (!keyReady()) {
    if (!keyPressed)
      return true;

    if (currentTimerTick < currentKeyTimeout)
      return true;

    currentPressedKey = '\0';
    keyPressed = false;
    return true;
  }

  currentPressedKey = getKey();

#ifdef DIAGNOSTICS_ON
  if (currentPressedKey == 'x' || currentPressedKey == 'X') {
    startDebugging = true;
    return true;
  }
#endif

  if (currentPressedKey == CTRL_Z) {
    return false;
  }

  if (currentPressedKey >= '0' && currentPressedKey <= '9')
    currentPressedKey -= '0';

  else if (currentPressedKey >= 'A' && currentPressedKey <= 'Z')
    currentPressedKey += (-'A' + 10);

  else if (currentPressedKey >= 'a' && currentPressedKey <= 'z')
    currentPressedKey += (-'a' + 10);

  currentKeyTimeout = currentTimerTick + 10;
  keyPressed = true;

  return true;
}
