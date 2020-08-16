#include "random.h"
#include "datatypes.h"
#include "systimer.h"
#include <stdlib.h>

void seedRandom() { srand((int)getSysTimer()); }

byte chip8Rand() { return rand(); }
