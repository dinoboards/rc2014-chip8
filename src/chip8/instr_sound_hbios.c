#include "instr_sound_hbios.h"
#include "hbios.h"
#include "instr_sound.h"
#include "systemstate.h"
#include "timers.h"

static hbSndParams sndParams;

void hbios_ldStVx() {
  sndParams.driver = 0;
  sndParams.volume = 255;
  hbSndVolume(&sndParams);

  if (audioActive) {
    sndParams.period = audioPeriod;
    hbSndPeriod(&sndParams);

    sndParams.channel = 0;
    hbSndPlay(&sndParams);

    sndParams.period = audioPeriod + 5;
    hbSndPeriod(&sndParams);
    sndParams.channel = 1;
    hbSndPlay(&sndParams);

  } else {
    sndParams.note = 0x30;
    hbSndNote(&sndParams);
    sndParams.channel = 0;
    hbSndPlay(&sndParams);
  }
}
