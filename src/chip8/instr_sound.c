#include "instr_sound.h"
#include "hbios.h"
#include "systemstate.h"
#include "timers.h"

hbSndParams sndParams;

void ldStVx() {
  initTimers();
  soundTimer = registers[secondNibble];

  if (soundTimer == 0) {
    hbSndReset(0);
    return;
  }

  sndParams.driver = 0;
  sndParams.volume = 255;
  hbSndVolume(&sndParams);

  sndParams.note = 0x30;
  hbSndNote(&sndParams);

  sndParams.channel = 0;
  hbSndPlay(&sndParams);
}

void soundOff() { hbSndReset(0); }
