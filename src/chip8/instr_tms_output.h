
#ifndef __INSTR_TMS_OUTPUT
#define __INSTR_TMS_OUTPUT

#include "datatypes.h"
#include "vid_int.h"

extern void tmsDraw();
extern void tmsCls();
extern void tmsVideoInit();

#define tmsVideoClose() vsyncRemoveInterruptHandler()

#endif
