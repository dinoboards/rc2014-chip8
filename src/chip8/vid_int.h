
#ifndef __VID_INT
#define __VID_INT

extern void vsyncInstallInterruptHandler();
extern void vsyncRemoveInterruptHandler();

extern void vsyncHookTimer();
extern void vsyncUnhookTimer();

#endif
