#include "random.h"
#include "datatypes.h"
#include "systimer.h"
#include <stdlib.h>

void seedRandom() { srand(getSysTimer()); }

byte chip8Rand() { return rand(); }
