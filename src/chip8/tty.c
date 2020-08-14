#include "chip8/tty.h"
#include "xstdio.h"
#include <stdarg.h>

void sendDrawCommands(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  xprintf(msg, arg);
  va_end(arg);
}
