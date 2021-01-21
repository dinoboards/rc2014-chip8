#include "audio.h"
#include "datatypes.h"
#include "hbios.h"
#include "instr_sound_msx.h"
#include "systemstate.h"

void audioInit() {
  const uint8_t audType = audioChipProbe();

  switch (audType) {
  case 1:
    installedAudioSystem = AS_MSX;
    print("YM2149 Audio Found\r\n");
    msx_soundInit();
    break;

  default:
    installedAudioSystem = AS_HBIOS;
    print("HBIOS Audio\r\n");
    break;
  }
}
