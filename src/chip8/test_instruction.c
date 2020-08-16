
#include "byte_code_executor.h"
#include "datatypes.h"
#include "instr_output.h"
#include "stack.h"
#include "systemstate.h"
#include "terminal_codes.h"
#include "test_opcodes.h"
#include "timers.h"
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

void setup_se_v4_15_skips() {
  registers[4] = 15;
  programStorage[0] = invertByteOrder(SE_V4_15);
}

void verify_se_v4_15_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_se_v4_15_no_skips() {
  registers[4] = 1;
  programStorage[0] = invertByteOrder(SE_V4_15);
}

void verify_se_v4_15_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_se_v4_v9_skips() {
  registers[4] = 15;
  registers[9] = 15;
  programStorage[0] = invertByteOrder(SE_V4_V9);
}

void verify_se_v4_v9_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_se_v4_v9_no_skips() {
  registers[4] = 15;
  registers[9] = 10;
  programStorage[0] = invertByteOrder(SE_V4_V9);
}

void verify_se_v4_v9_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_sne_v0_123_skips() {
  registers[4] = 15;
  programStorage[0] = invertByteOrder(SNE_V0_123);
}

void verify_sne_v0_123_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_sne_v0_123_no_skips() {
  registers[0] = 123;
  programStorage[0] = invertByteOrder(SNE_V0_123);
}

void verify_sne_v0_123_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_sne_v0_v2_skips() {
  registers[0] = 15;
  registers[2] = 25;
  programStorage[0] = invertByteOrder(SNE_V0_V2);
}

void verify_sne_v0_v2_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_sne_v0_v2_no_skips() {
  registers[0] = 25;
  registers[2] = 25;
  programStorage[0] = invertByteOrder(SNE_V0_V2);
}

void verify_sne_v0_v2_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_cls() { programStorage[0] = invertByteOrder(CLS); }

void verify_cls() { expectEqualEscapedString(buffer, "\033[?25l\033[2J\033[0;0H"); }

void setup_draw_top_right() {
  memset(videoMemory, 0, sizeof(videoMemory));

  registers[2] = 0;
  registers[3] = 0;
  registerI = 0x202;
  programStorage[0] = invertByteOrder(DRAW_V2_V3_1);
  programStorage[(registerI - 0x200) / 2] = 0x80;
}

void verify_draw_top_right() {
  expectEqualEscapedString(buffer, "\033[1;1H\033[47;1m \033[40m");
  expectEqualBytes(registers[0x0F], 0, "VF");
}

void setup_draw_xor() {
  memset(videoMemory, 0, sizeof(videoMemory));

  registers[2] = 0;
  registers[3] = 0;
  registerI = 0x202;
  programStorage[0] = invertByteOrder(DRAW_V2_V3_1);
  programStorage[(registerI - 0x200) / 2] = 0x80;
}

void verify_draw_xor() {
  chip8PC = (uint16_t *)0x200; /* re-execute the instruction to erase sprite */
  executeSingleInstruction();

  expectEqualEscapedString(buffer, "\033[1;1H\033[47;1m \033[40m\033[1;1H ");
  expectEqualBytes(registers[0x0F], 1, "VF");
}

void setup_jp_1026() { programStorage[0] = invertByteOrder(JP_1026); }

void verify_jp_1026() { expectEqualPtrs(chip8PC, (uint16_t *)0x402, "PC"); }

void setup_skp_v3_skips() {
  registers[3] = 0xA;
  simulateKey('A');
  programStorage[0] = invertByteOrder(SKP_V3);
}

void verify_skp_v3_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_skp_v3_no_skips() {
  registers[3] = 0xA;
  // simulateKey('A');
  programStorage[0] = invertByteOrder(SKP_V3);
}

void verify_skp_v3_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_skp_v3_wrong_key() {
  registers[3] = 0xA;
  simulateKey('B');
  programStorage[0] = invertByteOrder(SKP_V3);
}

void verify_skp_v3_wrong_key() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_ld_st_v2() {
  registers[2] = 16;
  programStorage[0] = invertByteOrder(LD_ST_V2);
}

void verify_ld_st_v2() {
  expectEqualBytes(soundTimer, 16, "ST");
  simulateTimerTick();
  expectEqualBytes(soundTimer, 15, "ST");
}

void setup_ld_dt_v3() {
  registers[3] = 12;
  programStorage[0] = invertByteOrder(LD_DT_V3);
}

void verify_ld_dt_v3() {
  expectEqualBytes(delayTimer, 12, "DT");
  simulateTimerTick();
  expectEqualBytes(delayTimer, 11, "DT");
}

void setup_ld_va_dt() {
  delayTimer = 0x45;
  programStorage[0] = invertByteOrder(LD_VA_DT);
}

void verify_ld_va_dt() { expectEqualBytes(registers[0xA], 0x45, "VA"); }

void setup_rnd_ve_15() {
  simulateRandomNumber(12 + 64);
  programStorage[0] = invertByteOrder(RND_VE_15);
}

void verify_rnd_ve_15() { expectEqualBytes(registers[0xE], 12, "VE"); }

void main() {
  assert(ld_v1_10);

  assert(ld_i_1234);

  assert(call_1025);

  assert(ret_from_subroutine);

  assert(final_ret);

  assert(add_ve_1);

  assert(se_v4_15_skips);
  assert(se_v4_15_no_skips);
  assert(se_v4_v9_skips);
  assert(se_v4_v9_no_skips);

  assert(sne_v0_123_skips);
  assert(sne_v0_123_no_skips);
  assert(sne_v0_v2_skips);
  assert(sne_v0_v2_no_skips);

  assert(cls);

  assert(draw_top_right);
  assert(draw_xor);

  assert(jp_1026);

  assert(skp_v3_skips);
  assert(skp_v3_no_skips);
  assert(skp_v3_wrong_key);

  assert(ld_st_v2);
  assert(ld_dt_v3);
  assert(ld_va_dt);

  assert(rnd_ve_15);

  xprintf(testFailure ? RED "Tests Failed\r\n" RESET : BRIGHT_WHITE "All Passed\r\n" RESET);
}
