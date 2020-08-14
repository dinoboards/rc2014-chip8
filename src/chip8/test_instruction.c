
#include "chip8/byte_code_executor.h"
#include "chip8/stack.h"
#include "chip8/systemstate.h"
#include "datatypes.h"
#include "test_opcodes.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "test_helpers.h"

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

void setup_add_ve_1() {
  registers[0xE] = 3;
  programStorage[0] = invertByteOrder(ADD_VE_1);
}

void verify_add_ve_1() { expectEqualBytes(registers[0xE], 4, "VE"); }

void main() {

  assert(ld_v1_10);

  assert(ld_i_1234);

  assert(call_1025);

  assert(ret_from_subroutine);

  assert(final_ret);

  assert(add_ve_1);

  xprintf(testFailure ? "Tests Failed\r\n" : "All Done\r\n");
}
