#include <stdint.h>

typedef struct FCBStruct {
  uint8_t       dr;
  char          filename[8];
  unsigned char type[3];
  uint8_t       ex;
  uint8_t       s1;
  uint8_t       s2;
  uint8_t       rc;
  uint8_t       reserved[16];
  uint8_t       cr;
  uint16_t      r;
  uint8_t       r2;
} FCB;

extern void chk(int f) __z88dk_fastcall;

extern int fMake(const FCB *fcb) __z88dk_fastcall;
extern int fRead(const FCB *fcb) __z88dk_fastcall;
extern int fDmaOff(void *dest) __z88dk_fastcall;
extern int fOpen(const FCB *fcb) __z88dk_fastcall;
extern int fSize(const FCB *fcb) __z88dk_fastcall;
extern int fWrite(const FCB *fcb) __z88dk_fastcall;
extern int fClose(const FCB *fcb) __z88dk_fastcall;

#define defaultFCB ((FCB *)0x5C)
