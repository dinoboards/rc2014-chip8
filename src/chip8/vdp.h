#ifndef __VDP
#define __VDP

#include "datatypes.h"

#define VDP_NONE  255
#define VDP_TMS   0
#define VDP_V9938 1
#define VDP_V9958 2

// Returns one of the above VDP_XXX constants
extern uint8_t videoChipProbe(void);

#endif
