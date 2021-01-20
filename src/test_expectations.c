#include "test_expectations.h"
#include "terminal_codes.h"
#include "xstdio.h"
#include <string.h>

bool testFailure = false;

void expectFalse(bool a, const char *msg) {
  if (!a) {
    xprintf("\t%s == false\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to be false but was %d\r\n" RESET, msg, (int)a);
}

void expectTrue(bool a, const char *msg) {
  if (a) {
    xprintf("\t%s == true\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to be true but was %d\r\n" RESET, msg, (int)a);
}

void expectEqualBytes(byte a, byte b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to equal %d, but was %d\r\n" RESET, msg, b, a);
}

void expectEqualChars(char a, char b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %02X\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to equal '%c', but was '%c'\r\n" RESET, msg, b, a);
}

void expectEqualInts(int a, int b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to equal %d, but was %d\r\n" RESET, msg, b, a);
}

void expectEqualPtrs(uint16_t *a, uint16_t *b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %p\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf(RED "Expected %s to equal %p, but was %p\r\n" RESET, msg, b, a);
}

void replace(char *string, char from, char to) {
  for (int i = 0; i < strlen(string); ++i)
    if (string[i] == from)
      string[i] = to;
}

void unescape(const char *string) { replace((char *)string, '\033', '~'); }

void expectEqualEscapedString(const char *a, const char *b) {
  if (strcmp(a, b) == 0) {
    unescape(a);
    return;
  }

  testFailure = true;

  unescape(a);
  unescape(b);

  xprintf(RED "Expected output of \"%s\" but got \"%s\"\r\n" RESET, b, a);
}
