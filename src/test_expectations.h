#ifndef __TEST_EXPECTATIONS
#define __TEST_EXPECTATIONS

#include "datatypes.h"
#include "terminal_codes.h"
#include "xstdio.h"
#include <string.h>

extern bool testFailure;

extern void expectFalse(bool a, const char *msg);
extern void expectTrue(bool a, const char *msg);
extern void expectEqualBytes(byte a, byte b, const char *msg);
extern void expectEqualInts(int a, int b, const char *msg);
extern void expectEqualPtrs(uint16_t *a, uint16_t *b, const char *msg);
extern void replace(char *string, char from, char to);
extern void unescape(const char *string);
extern void expectEqualEscapedString(const char *a, const char *b);

#endif
