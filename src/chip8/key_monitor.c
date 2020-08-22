#include "charconstants.h"
#include "keys.h"
#include "systemstate.h"
#include "timers.h"
#include "xstdio.h"

void checkForKeyPresses() {

  if (!keyReady()) {
    if (!keyPressed)
      return;

    if (currentTimerTick < currentKeyTimeout)
      return;

    currentPressedKey = '\0';
    keyPressed = false;
    return;
  }

  getKey(&currentPressedKey);

#ifdef DIAGNOSTICS_ON
  if (currentPressedKey == 'x' || currentPressedKey == 'X') {
    startDebugging = true;
    return;
  }
#endif

  if (currentPressedKey == CTRL_Z)
    exit(0);

  if (currentPressedKey >= '0' && currentPressedKey <= '9')
    currentPressedKey -= '0';

  else if (currentPressedKey >= 'A' && currentPressedKey <= 'Z')
    currentPressedKey += (-'A' + 10);

  else if (currentPressedKey >= 'a' && currentPressedKey <= 'z')
    currentPressedKey += (-'a' + 10);

  currentKeyTimeout = currentTimerTick + 2;
  keyPressed = true;
}
