
#ifndef __SYSTIMER
#define __SYSTIMER

#include "datatypes.h"
#include "hbios.h"
#include "systemstate.h"
#include "systimer.h"

// #define getSysTimer() ((timerMode == TMS_TIMER_MODE) ? timerTick : (timerMode == HBIOS_TIMER_MODE) ? hbSysGetTimer16() : instructionCostCounter / 48)

extern uint16_t getSysTimer();
extern uint8_t  sysTimerSearchDriver();

#endif
