
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
bool appRunning = false;

void expectFalse(bool a, const char *msg) {
  if (!a) {
    xprintf("\t%s == false\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to be false but was %d\r\n", msg, (int)a);
}

void expectTrue(bool a, const char *msg) {
  if (a) {
    xprintf("\t%s == true\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to be true but was %d\r\n", msg, (int)a);
}

void expectEqualBytes(byte a, byte b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualInts(int a, int b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualPtrs(uint16_t *a, uint16_t *b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %p\r\n", msg, a);
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

void setup_ret_from_subroutine() {
  stack[0] = 0xF000;
  stackIndex = 1;
  programStorage[0] = invertByteOrder(RET);
}

void verify_ret_from_subroutine() {
  expectEqualPtrs(chip8PC, (uint16_t *)0xF000, "PC");
  expectTrue(appRunning, "appRunning");
}

void setup_final_ret() {
  stackIndex = 0;
  programStorage[0] = invertByteOrder(RET);
}

void verify_final_ret() { expectFalse(appRunning, "appRunning"); }

#define assert(a)                            \
  {                                          \
    xprintf(#a "\r\n");                      \
    initSystemState();                       \
    setup_##a();                             \
    appRunning = executeSingleInstruction(); \
    verify_##a();                            \
    xprintf("\r\n");                         \
  }

void main() {

  assert(ld_v1_10);

  assert(ld_i_1234);

  assert(call_1025);

  assert(ret_from_subroutine);

  assert(final_ret);

  xprintf(testFailure ? "Tests Failed\r\n" : "All Done\r\n");
}
