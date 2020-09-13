#include "instr_sound.h"
#include "hbios.h"
#include "systemstate.h"
#include "timers.h"

static hbSndParams sndParams;

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

void soundOff() { hbSndReset(0); }

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

static uint16_t findSoundPeriod(const uint8_t *pattern) __z88dk_fastcall {
  countOfPeriods = 0;
  lastBitIndex = AUDIO_SAMPLE_SIZE;
  lastBitState = 0;
  currentBitIndex = 0;
  currentBitState = 0;
  indexWithinCurrentByte = 0;
  currentByte = 0;
  total = 0;

  const uint8_t *pPattern = pattern;

  currentBitIndex = AUDIO_SAMPLE_SIZE;
  while (currentBitIndex != 0) {
    currentByte = *pPattern++;

    total = process8Samples(total);
  }

  return total * 64 / countOfPeriods;
}

void audio() {
  audioActive = true;
  audioPeriod = findSoundPeriod((const uint8_t *)registerI);
}
