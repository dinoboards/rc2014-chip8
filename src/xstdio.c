#include "xstdio.h"
#include "datatypes.h"
#include "hbios.h"
#include <stdarg.h>
#include <stdio.h>

char xbuffer[MAX_MESSAGE_TEXT];

void xprintf(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  vsnprintf(xbuffer, MAX_MESSAGE_TEXT - 1, (char *)msg, arg);
  va_end(arg);
  print(xbuffer);
}

#ifdef XTRACE
void xtracef(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  vsnprintf(xbuffer, MAX_MESSAGE_TEXT - 1, (char *)msg, arg);
  va_end(arg);
  print(xbuffer);
}
#else
// void xtracef(const char *msg, ...) { (void)msg; }
#endif
