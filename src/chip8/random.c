#include "random.h"
#include "datatypes.h"
#include "systimer.h"
#include <stdlib.h>

void seedRandom(void) { srand(getSysTimer()); }

byte chip8Rand(void) { return rand(); }
