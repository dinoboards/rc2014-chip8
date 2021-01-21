#include "instr_sound.h"
#include "audio.h"
#include "hbios.h"
#include "instr_sound_hbios.h"
#include "instr_sound_msx.h"
#include "systemstate.h"
#include "timers.h"

void ldStVx() {
  initTimers();
  soundTimer = registers[nibble2nd];

  if (soundTimer == 0) {
    soundOff();
    return;
  }

  if (installedAudioSystem == AS_HBIOS)
    hbios_ldStVx();
  else
    msx_ldStVx();
}

void soundOff() {
  if (installedAudioSystem == AS_HBIOS)
    hbios_soundOff();
  else
    msx_soundOff();
}

static uint8_t countOfPeriods = 0;
static uint8_t lastBitIndex = 0;
static uint8_t lastBitState = 0;
static uint8_t currentBitIndex = 0;
static uint8_t currentBitState = 0;
static uint8_t indexWithinCurrentByte = 0;
static uint8_t currentByte = 0;

static uint16_t process8Samples(uint16_t total) __z88dk_fastcall {
  uint8_t indexWithinCurrentByte = 9;

  while (--indexWithinCurrentByte != 0) {
    currentBitState = currentByte & 0x80;
    currentByte <<= 1;

    if (lastBitState != currentBitState) {
      uint8_t diff = lastBitIndex - currentBitIndex;
      if (diff != 0) {
        total += diff;
        countOfPeriods++;
        lastBitIndex = currentBitIndex;
      }
      lastBitState = currentBitState;
    }

    currentBitIndex--;
  }

  return total;
}

static uint16_t total = 0;

static uint16_t findSoundPeriod() __z88dk_fastcall {
  countOfPeriods = 0;
  lastBitIndex = AUDIO_SAMPLE_SIZE;
  lastBitState = 0;
  currentBitIndex = 0;
  currentBitState = 0;
  indexWithinCurrentByte = 0;
  currentByte = 0;
  total = 0;

  const uint8_t *pPattern = (const uint8_t *)registerI;

  currentBitIndex = AUDIO_SAMPLE_SIZE;
  while (currentBitIndex != 0) {
    currentByte = *pPattern++;

    total = process8Samples(total);
  }

  return total * 64 / countOfPeriods;
}

void audio() {
  audioActive = true;
  audioPeriod = findSoundPeriod();
}
