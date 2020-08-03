#include "xstdio.h"
#include "datatypes.h"
#include "hbios.h"
#include <stdarg.h>
#include <stdio.h>

#define MAX_MESSAGE_TEXT 256

void xprintf(const char *msg, ...) {
  char    pbuffer[MAX_MESSAGE_TEXT];
  va_list arg;
  va_start(arg, msg);
  vsnprintf(pbuffer, MAX_MESSAGE_TEXT - 1, (char *)msg, arg);
  va_end(arg);
  print(pbuffer);
}

#ifdef XTRACE
void xtracef(const char *msg, ...) {
  char    pbuffer[MAX_MESSAGE_TEXT];
  va_list arg;
  va_start(arg, msg);
  vsnprintf(pbuffer, MAX_MESSAGE_TEXT - 1, (char *)msg, arg);
  va_end(arg);
  print(pbuffer);
}
#else
void xtracef(const char *msg, ...) { (void)msg; }
#endif
