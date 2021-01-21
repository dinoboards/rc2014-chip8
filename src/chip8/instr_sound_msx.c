#include "instr_sound_msx.h"
#include "audio.h"
#include "systemstate.h"
#include "timers.h"

audioCommand msxAudioCommand;

void msx_ldStVx() {
  msxAudioCommand.volume = 15;

  if (audioActive)
    msxAudioCommand.period = audioPeriod;
  else
    msxAudioCommand.period = 123;

  audioCmd(&msxAudioCommand);
}

void msx_soundOff() {
  msxAudioCommand.volume = 0;
  msxAudioCommand.period = 0;
  audioCmd(&msxAudioCommand);
}

void msx_soundInit() { msxAudioCommand.channel = 1; }
