#ifndef __APPMSX
#define __APPMSX

#include <stdlib.h>

#ifdef MSX
#define JIFFY_ADDR __at 0xFC9E
#else
#define JIFFY_ADDR
#endif

// FC9E-FC9F: software clock, updated at each VDP interrupt
extern uint16_t JIFFY_ADDR JIFFY;

#endif
