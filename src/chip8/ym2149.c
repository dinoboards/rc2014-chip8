#include "ym2149.h"
#include "audio.h"
#include "systemstate.h"
#include "timers.h"

audioCommand command;

extern uint16_t ym2149Cmd(const audioCommand *) __z88dk_fastcall;

void ym2149Play(uint16_t period) __z88dk_fastcall {
  command.volume = 15;
  command.period = period;

  ym2149Cmd(&command);
}

void ym2149Stop() {
  command.volume = 0;
  command.period = 0;
  ym2149Cmd(&command);
}
