#ifndef __APPMSX
#define __APPMSX

#include <stdlib.h>

#ifdef MSX
#define JIFFY_ADDR  __at 0xFC9E
#define NEWKEY_ADDR __at 0xFBE5
#define PUTPNT_ADDR __at 0xF3F8
#define GETPNT_ADDR __at 0xF3FA
#else
#define JIFFY_ADDR
#define NEWKEY_ADDR
#define PUTPNT_ADDR
#define GETPNT_ADDR
#endif

// FC9E-FC9F: software clock, updated at each VDP interrupt
extern uint16_t JIFFY_ADDR JIFFY;

// FBE5-FBEF: current state of the keyboard matrix
extern uint8_t NEWKEY_ADDR NEWKEY[10];

// F3F8-F3F9: first free space in the inputbuffer of the keyboard
// everytime a key is added to the inputbuffer, this address is incremented,
// when it equals to GETPNT, the buffer is full
// the buffer is located at KEYBUF
extern char *PUTPNT_ADDR PUTPNT;

// F3FA-F3FB: address in inputbuffer of first character that is not yet read
// everytime a key is read from the buffer it is incremented
// the buffer is located at KEYBUF
extern char *GETPNT_ADDR GETPNT;

#define msxJiffy  JIFFY
#define msxNewKey NEWKEY
#define msxPutPtr PUTPNT
#define msxGetPtr GETPNT

#endif
