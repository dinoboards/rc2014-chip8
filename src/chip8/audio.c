#include "audio.h"
#include "datatypes.h"
#include "hbios.h"
#include "hbios_audio.h"
#include "systemstate.h"
#include "ym2149.h"

void audioInit() {
  const bool ym2149Found = ym2149Probe();

  if (ym2149Found) {
    installedAudioSystem = AS_YM2149;
    isYm2149 = true;
    print("YM2149 Audio Found\r\n");
    ym2149Init();
    return;
  }

  installedAudioSystem = AS_HBIOS;
  print("HBIOS Audio\r\n");
}

void audioPlay(uint16_t p) __z88dk_fastcall {
  if (isYm2149) {
    ym2149Play(p);
    return;
  }

  hbiosAudioPlay(p);
}

void audioStop() {
  if (isYm2149) {
    ym2149Stop();
    return;
  }

  hbiosAudioStop();
}
