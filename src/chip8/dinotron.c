#include "dinotron.h"
#include "hbios.h"

#define TERM_RESET           "\033[m"
#define TERM_FONT_8X16       "\033[=01f"
#define TERM_MODE_2          "\033[=2h"
#define TERM_CURSOR_VISIABLE "\033[?25h"

void dinotronSwitchSerial() {
  // clang-format off
  __asm
    LD    A, 0
    OUT   (15), A
  __endasm;
  // clang-format on
  print(TERM_RESET TERM_FONT_8X16 TERM_MODE_2 TERM_CURSOR_VISIABLE "\r\n");
}

void dinotronSwitchTms() {
  // clang-format off
  __asm
    LD    A, 255
    OUT   (15), A
  __endasm;
  // clang-format on
}
