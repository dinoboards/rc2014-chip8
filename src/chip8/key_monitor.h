#ifndef __KEY_MONITOR
#define __KEY_MONITOR

#include "datatypes.h"

extern bool    checkForKeyPresses(void);
extern uint8_t isKeyDown(const uint8_t c) __z88dk_fastcall;
extern uint8_t currentKey(void);

#endif
