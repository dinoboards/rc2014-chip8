#include "keys.h"
#include "datatypes.h"
#include "hbios.h"

static hbCioParams cioParams;

byte keyReady() { return hbCioIst(0); }

char getKey() {
  cioParams.driver = 0;
  hbCioIn(&cioParams);
  return cioParams.chr;
}
