#include "audio.h"
#include "datatypes.h"
#include "systemstate.h"
#include "ym2149.h"
#include <stdio.h>

void audioInit(void) {
  // const bool ym2149Found = ym2149Probe();

  // if (ym2149Found) {
  //   installedAudioSystem = AS_YM2149;
  //   isYm2149 = true;
  //   printf("YM2149 Audio Found\r\n");
  //   ym2149Init();
  //   return;
  // }

  // installedAudioSystem = AS_HBIOS;
  // printf("HBIOS Audio\r\n");
}

void audioPlay(uint16_t p) __z88dk_fastcall {
  (void)p;
  // if (isYm2149) {
  //   ym2149Play(p);
  //   return;
  // }
}

void audioStop(void) {
  // if (isYm2149) {
  //   ym2149Stop();
  //   return;
  // }
}
