#include "vid_int.h"
#include "hbios.h"
#include "systemstate.h"
#include "tms.h"

hbSysParams params;

void vsyncHookTimer() {
  hbSysIntInfo(&params);

  if (params.interruptMode != 1)
    return;

  vsyncInstallInterruptHandler();
  timerMode = TMS_TIMER_MODE;
}

void vsyncUnhookTimer() {
  if (timerMode == TMS_TIMER_MODE)
    vsyncRemoveInterruptHandler();
}
