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

#define VDADEV_VDU  ((uint8_t)0x00) /* ECB VDU - MOTOROLA 6545 */
#define VDADEV_CVDU ((uint8_t)0x10) /* ECB COLOR VDU - MOS 8563 */
#define VDADEV_NEC  ((uint8_t)0x20) /* ECB UPD7220 - NEC UPD7220 */
#define VDADEV_TMS  ((uint8_t)0x30) /* N8 ONBOARD VDA SUBSYSTEM - TMS 9918 */
#define VDADEV_VGA  ((uint8_t)0x40) /* ECB VGA3 - HITACHI HD6445 */

typedef struct {
  uint8_t unitId;
  uint8_t devType;
  uint8_t devNumber;
  uint8_t devMode;
  uint8_t devBaseIO;
} hbiosVdaDev;
extern uint8_t hbVdaDev(hbiosVdaDev *result) __z88dk_fastcall;

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
  };
} hbSndParams;
extern uint8_t hbSndVolume(hbSndParams *pParams) __z88dk_fastcall;
extern uint8_t hbSndNote(hbSndParams *pParams) __z88dk_fastcall;
extern uint8_t hbSndPlay(hbSndParams *pParams) __z88dk_fastcall;

#endif
