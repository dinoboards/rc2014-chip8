#include "test_system.h"
#include "msx.h"
#include "systemstate.h"
#include "timers.h"
#include "ym2149.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

bool appRunning = false;

void resetKeySimulator() {
  for (uint8_t i = 0; i < sizeof(msxNewKey); i++)
    msxNewKey[i] = 0xFF;
  JIFFY = 0;
}

void simulateKey(const uint8_t rowIndex, const uint8_t bitMask) {
  msxNewKey[rowIndex] &= ~bitMask;
  msxJiffy += 4;
}

uint16_t getSysTimer() { return JIFFY; }

void simulateTimerTick() {
  msxJiffy++;
  manageTimers();
}

byte nextRandomNumber = 0;

byte chip8Rand() { return nextRandomNumber; }

void simulateRandomNumber(byte b) { nextRandomNumber = b; }

void updateMachineStateDisplay() {}

void tmsDriverNotFound() {}

void ym2149Play(uint16_t period) __z88dk_fastcall { (void *)period; }

void ym2149Stop() {}

uint8_t getControllerDirection(uint8_t port) __z88dk_fastcall {
  (void)port;

  return 0;
}

uint8_t getControllerButton(uint8_t b) __z88dk_fastcall { return b; }

bool ym2149Probe() { return false; }

void restoreSystemHardware() { }

audioCommand command;
