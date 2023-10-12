
#ifndef __INSTR_TMS_OUTPUT
#define __INSTR_TMS_OUTPUT

#include "datatypes.h"

extern void tmsDraw(void);
extern void tmsCls(void);
extern void tmsVideoInit(void);

#define tmsVideoClose()                                                                                                            \
  {}

#endif
