
#include "chip8/byte_code_executor.h"
#include "chip8/stack.h"
#include "chip8/systemstate.h"
#include "datatypes.h"
#include "test_opcodes.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

bool testFailure = false;

void expectEqualBytes(byte a, byte b, const char *msg) {
  if (a == b) {
    xprintf("\r\n\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualInts(int a, int b, const char *msg) {
  if (a == b) {
    xprintf("\r\n\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualPtrs(uint16_t *a, uint16_t *b, const char *msg) {
  if (a == b) {
    xprintf("\r\n\t%s == %p\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %p, but was %p\r\n", msg, b, a);
}

void setup_ld_v1_10() { programStorage[0] = invertByteOrder(LD_V1_10); }

void verify_ld_v1_10() { expectEqualBytes(registers[1], 10, "V1"); }

void setup_ld_i_1234() { programStorage[0] = invertByteOrder(LD_I_1234); }

void verify_ld_i_1234() { expectEqualInts(registerI, 1234, "I"); }

void setup_call_1025() { programStorage[0] = invertByteOrder(CALL_1025); }

void verify_call_1025() {
  expectEqualPtrs(chip8PC, (uint16_t *)1025, "PC");
  expectEqualBytes(stackIndex, 1, "Stack Index");
  expectEqualPtrs((uint16_t *)stack[0], (uint16_t *)0x202, "Stack Index");
}

#define assert(a)               \
  {                             \
    xprintf(" " #a "  ");       \
    initSystemState();          \
    setup_##a();                \
    executeSingleInstruction(); \
    verify_##a();               \
  }

void main() {

  assert(ld_v1_10);

  assert(ld_i_1234);

  assert(call_1025);

  xprintf(testFailure ? "Tests Failed\r\n" : "All Done\r\n");
}
