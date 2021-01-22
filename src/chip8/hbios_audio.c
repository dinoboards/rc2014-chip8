#include "hbios_audio.h"
#include "hbios.h"
#include "instr_sound.h"
#include "systemstate.h"
#include "timers.h"

static hbSndParams sndParams;

void hbiosAudioPlay(uint16_t period) __z88dk_fastcall {
  sndParams.driver = 0;
  sndParams.volume = 255;
  hbSndVolume(&sndParams);

  if (audioActive) {
    sndParams.period = period;
    hbSndPeriod(&sndParams);

    sndParams.channel = 0;
    hbSndPlay(&sndParams);

    sndParams.period = period + 5;
    hbSndPeriod(&sndParams);
    sndParams.channel = 1;
    hbSndPlay(&sndParams);
  }
}
