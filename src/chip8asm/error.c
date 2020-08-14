#include "error.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>

void logError(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  xprintf(msg, arg);
  va_end(arg);
}
