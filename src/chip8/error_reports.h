
#ifndef __ERROR_REPORTS
#define __ERROR_REPORTS

#include "datatypes.h"

extern void expectedEqualSign();
extern void unexpectedToken();
extern void unexpectedEntry();
extern void expectedColour();
extern void tmsDriverNotFound();
extern void warnNoTimerFound();

extern uint16_t expectedError(const char *p) __z88dk_fastcall;

#endif
