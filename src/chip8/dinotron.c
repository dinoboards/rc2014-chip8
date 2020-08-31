#include "dinotron.h"
#include "hbios.h"
#include "pigfx.h"

void dinotronSwitchSerial() {
  // clang-format off
  __asm
    LD    A, 0
    OUT   (15), A
  __endasm;
  // clang-format on
  print(TERM_RESET TERM_FONT_8X16 TERM_MODE_2 TERM_CURSOR_VISIBLE "\r\n");
}

void dinotronSwitchTms() {
  // clang-format off
  __asm
    LD    A, 255
    OUT   (15), A
  __endasm;
  // clang-format on
}
