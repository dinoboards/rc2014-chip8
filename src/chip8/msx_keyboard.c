#include "msx_keyboard.h"
#include <string.h>

MatrixMapping MatrixMappings[] = {{"7", 0, 1 << 7},     {"6", 0, 1 << 6},      {"5", 0, 1 << 5},         {"4", 0, 1 << 4},     {"3", 0, 1 << 3},     {"2", 0, 1 << 2},      {"1", 0, 1 << 1},     {"0", 0, 1 << 0},

                                  {";", 1, 1 << 7},     {"]", 1, 1 << 6},      {"[", 1, 1 << 5},         {"\\", 1, 1 << 4},    {"=", 1, 1 << 3},     {"-", 1, 1 << 2},      {"9", 1, 1 << 1},     {"8", 1, 1 << 0},

                                  {"B", 2, 1 << 7},     {"A", 2, 1 << 6},      {"DEAD", 2, 1 << 5},      {"/", 2, 1 << 4},     {".", 2, 1 << 3},     {",", 2, 1 << 2},      {"`", 2, 1 << 1},     {"\'", 2, 1 << 0},

                                  {"J", 3, 1 << 7},     {"I", 3, 1 << 6},      {"H", 3, 1 << 5},         {"G", 3, 1 << 4},     {"F", 3, 1 << 3},     {"E", 3, 1 << 2},      {"D", 3, 1 << 1},     {"C", 3, 1 << 0},

                                  {"R", 4, 1 << 7},     {"Q", 4, 1 << 6},      {"P", 4, 1 << 5},         {"O", 4, 1 << 4},     {"N", 4, 1 << 3},     {"M", 4, 1 << 2},      {"L", 4, 1 << 1},     {"K", 4, 1 << 0},

                                  {"Z", 5, 1 << 7},     {"Y", 5, 1 << 6},      {"X", 5, 1 << 5},         {"W", 5, 1 << 4},     {"V", 5, 1 << 3},     {"U", 5, 1 << 2},      {"T", 5, 1 << 1},     {"S", 5, 1 << 0},

                                  {"F3", 6, 1 << 7},    {"F2", 6, 1 << 6},     {"F1", 6, 1 << 5},        {"CODE", 6, 1 << 4},  {"CAPS", 6, 1 << 3},  {"GRAPH", 6, 1 << 2},  {"CTRL", 6, 1 << 1},  {"SHIFT", 6, 1 << 0},

                                  {"RET", 7, 1 << 7},   {"SELECT", 7, 1 << 6}, {"BACKSPACE", 7, 1 << 5}, {"STOP", 7, 1 << 4},  {"TAB", 7, 1 << 3},   {"ESC", 7, 1 << 2},    {"F5", 7, 1 << 1},    {"F4", 7, 1 << 0},

                                  {"RIGHT", 8, 1 << 7}, {"DOWN", 8, 1 << 6},   {"UP", 8, 1 << 5},        {"LEFT", 8, 1 << 4},  {"DEL", 8, 1 << 3},   {"INSERT", 8, 1 << 2}, {"HOME", 8, 1 << 1},  {"SPACE", 8, 1 << 0},

                                  {"NUM4", 9, 1 << 7},  {"NUM3", 9, 1 << 6},   {"NUM2", 9, 1 << 5},      {"NUM1", 9, 1 << 4},  {"NUM0", 9, 1 << 3},  {"NUM/", 9, 1 << 2},   {"NUM+", 9, 1 << 1},  {"NUM*", 9, 1 << 0},

                                  {"NUM.", 10, 1 << 7}, {"NUM,", 10, 1 << 6},  {"NUM-", 10, 1 << 5},     {"NUM9", 10, 1 << 4}, {"NUM8", 10, 1 << 3}, {"NUM7", 10, 1 << 2},  {"NUM6", 10, 1 << 1}, {"NUM5", 10, 1 << 0}, {0, 0, 0}};

const MatrixMapping *codeToMatrix(const char *keyCode) __z88dk_fastcall {
  const MatrixMapping *p = MatrixMappings;
  while (1) {
    const char *code = p->code;
    if (strcasecmp(code, keyCode) == 0 || *code == 0)
      return p;

    p++;
  }
}
