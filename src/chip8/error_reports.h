
#ifndef __ERROR_REPORTS
#define __ERROR_REPORTS

#include "datatypes.h"

extern void expectedEqualSign(void);
extern void unexpectedToken(void);
extern void unexpectedEntry(void);
extern void expectedColour(void);
extern void tmsDriverNotFound(void);
extern void warnNoTimerFound(void);

extern uint16_t expectedError(const char *p) __z88dk_fastcall;

#endif
