//#define XTRACE

#ifndef __XSTDIO
#define __XSTDIO

#define MAX_MESSAGE_TEXT 512
extern char xbuffer[MAX_MESSAGE_TEXT];

extern void xprintf(const char *msg, ...);
extern void xtracef(const char *msg, ...);

#endif
