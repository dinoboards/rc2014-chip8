#ifndef __HBIOS
#define __HBIOS

#include <stdint.h>

#define HCB_BIDUSR  0x8E /* USER BANK (TPA) */
#define HCB_BIDBIOS 0x8D /* BIOS BANK (HBIOS, UBIOS) */

void print(const char *str) __z88dk_fastcall;

typedef struct {
  uint8_t func;
  uint8_t unit;
  void *  driverFnAddr;
  void *  driverDataAddr;
} hbiosDriverEntry;

extern uint8_t hbCioIn(uint8_t driver, char *result);
extern uint8_t hbCioIst(uint8_t) __z88dk_fastcall;
extern uint8_t hbCioOut(uint8_t, char);

extern uint8_t hbSysGetTimer(long *result) __z88dk_fastcall;

extern uint8_t hbSysGetVda(hbiosDriverEntry *pData) __z88dk_fastcall;

typedef struct {
  uint8_t  destBank;
  uint8_t  sourceBank;
  uint16_t byteCount;
  void *   destAddr;
  void *   sourceAddr;
} hbiosBankCopy;

extern uint8_t hbSysBankCopy(hbiosBankCopy *pData) __z88dk_fastcall;

#endif
