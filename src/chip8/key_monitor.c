#include "keys.h"
#include "systemstate.h"
#include "xstdio.h"

void checkForKeyPresses() {

  currentPressedKey = '\0';
  keyPressed = false;

  if (!keyReady())
    return;

  getKey(&currentPressedKey);

  if (currentPressedKey >= '0' && currentPressedKey <= '9')
    currentPressedKey -= '0';

  else if (currentPressedKey >= 'A' && currentPressedKey <= 'Z')
    currentPressedKey += (-'A' + 10);

  else if (currentPressedKey >= 'a' && currentPressedKey <= 'z')
    currentPressedKey += (-'a' + 10);

  keyPressed = true;
}
