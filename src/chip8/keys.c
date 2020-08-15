#include "keys.h"
#include "datatypes.h"
#include "hbios.h"

bool keyReady() { return hbCioIst(0) != 0; }

void getKey(char *r) { hbCioIn(0, r); }
