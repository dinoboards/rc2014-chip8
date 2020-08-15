#ifndef __HBIOS
#define __HBIOS

#include <stdint.h>

void print(const char *str) __z88dk_fastcall;

extern uint8_t hbCioIn(uint8_t driver, char *result);
extern uint8_t hbCioIst(uint8_t) __z88dk_fastcall;
extern uint8_t hbCioOut(uint8_t, char);

#endif
