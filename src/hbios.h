#ifndef __HBIOS
#define __HBIOS

#include <stdint.h>

#define HCB_BIDUSR  0x8E /* USER BANK (TPA) */
#define HCB_BIDBIOS 0x8D /* BIOS BANK (HBIOS, UBIOS) */

void print(const char *str) __z88dk_fastcall;

typedef struct {
  uint8_t driver;
  char    chr;
} hbCioParams;

extern uint8_t hbCioIn(hbCioParams *) __z88dk_fastcall;
extern uint8_t hbCioIst(uint8_t) __z88dk_fastcall;
extern uint8_t hbCioOut(hbCioParams *) __z88dk_fastcall;

typedef struct {
  uint8_t  destBank;
  uint8_t  sourceBank;
  uint16_t byteCount;
  void *   destAddr;
  void *   sourceAddr;
} hbiosBankCopy;
extern uint8_t hbSysBankCopy(hbiosBankCopy *pData) __z88dk_fastcall;

extern uint8_t hbSndReset(uint8_t driver) __z88dk_fastcall;

typedef struct {
  uint8_t driver;
  union {
    uint8_t  volume;
    uint16_t note;
    uint8_t  channel;
    uint16_t period;
  };
} hbSndParams;
extern uint8_t hbSndVolume(hbSndParams *pParams) __z88dk_fastcall;
extern uint8_t hbSndNote(hbSndParams *pParams) __z88dk_fastcall;
extern uint8_t hbSndPlay(hbSndParams *pParams) __z88dk_fastcall;
extern uint8_t hbSndPeriod(hbSndParams *pParams) __z88dk_fastcall;

#endif
