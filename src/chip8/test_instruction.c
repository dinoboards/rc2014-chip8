
#include "byte_code_executor.h"
#include "datatypes.h"
#include "fontsets.h"
#include "instr_serial_output.h"
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

void setup_ld_v3_va() {
  registers[0xA] = 23;
  programStorage[0] = invertByteOrder(LD_V3_VA);
}

void verify_ld_v3_va() { expectEqualBytes(registers[3], 23, "V3"); }

void setup_ld_i_1234() { programStorage[0] = invertByteOrder(LD_I_1234); }

void verify_ld_i_1234() { expectEqualInts(registerI, 1234, "I"); }

void setup_ld_vb_i() {
  registerI = 0x600;
  for (int i = 0; i < 12; i++)
    ((byte *)registerI)[i] = i + 1;
  programStorage[0] = invertByteOrder(LD_VB_I);
  registers[0xC] = 99;
}

void verify_ld_vb_i() {
  expectEqualBytes(registers[0], 1, "V0");
  expectEqualBytes(registers[1], 2, "V1");
  expectEqualBytes(registers[2], 3, "V2");
  expectEqualBytes(registers[3], 4, "V3");
  expectEqualBytes(registers[4], 5, "V4");
  expectEqualBytes(registers[5], 6, "V5");
  expectEqualBytes(registers[6], 7, "V6");
  expectEqualBytes(registers[7], 8, "V7");
  expectEqualBytes(registers[8], 9, "V8");
  expectEqualBytes(registers[9], 10, "V9");
  expectEqualBytes(registers[0xA], 11, "VA");
  expectEqualBytes(registers[0xB], 12, "VB");
  expectEqualBytes(registers[0xC], 99, "VC");
  // expectEqualInts(registerI, 0x600 + 12, "I"); //QUICKS
}

void setup_ld_v0_i() {
  registerI = 0x600;
  ((byte *)registerI)[0] = 55;
  programStorage[0] = invertByteOrder(LD_V0_I);
  registers[0x1] = 99;
}

void verify_ld_v0_i() {
  expectEqualBytes(registers[0], 55, "V0");
  expectEqualBytes(registers[1], 99, "V1");
  // expectEqualInts(registerI, 0x600 + 1, "I"); //quirks
}

void setup_ld_i_ve() {
  registerI = 0x600;
  memset((void *)registerI, 0, 16);
  programStorage[0] = invertByteOrder(LD_I_VE);
  for (int i = 0; i < 15; i++)
    registers[i] = i + 10;
}

void verify_ld_i_ve() {
  byte *pExpected = (byte *)0x600;
  expectEqualBytes(pExpected[0x0], 10, "[i+0x0]");
  expectEqualBytes(pExpected[0x1], 11, "[i+0x1]");
  expectEqualBytes(pExpected[0x2], 12, "[i+0x2]");
  expectEqualBytes(pExpected[0x3], 13, "[i+0x3]");
  expectEqualBytes(pExpected[0x4], 14, "[i+0x4]");
  expectEqualBytes(pExpected[0x5], 15, "[i+0x6]");
  expectEqualBytes(pExpected[0x6], 16, "[i+0x7]");
  expectEqualBytes(pExpected[0x7], 17, "[i+0x8]");
  expectEqualBytes(pExpected[0x8], 18, "[i+0x9]");
  expectEqualBytes(pExpected[0x9], 19, "[i+0x9]");
  expectEqualBytes(pExpected[0xA], 20, "[i+0xA]");
  expectEqualBytes(pExpected[0xB], 21, "[i+0xB]");
  expectEqualBytes(pExpected[0xC], 22, "[i+0xC]");
  expectEqualBytes(pExpected[0xD], 23, "[i+0xD]");
  expectEqualBytes(pExpected[0xE], 24, "[i+0xE]");
  // expectEqualInts(registerI, 0x600 + 15, "I"); //quirks
}

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

void setup_add_i_v9() {
  registers[0x9] = 3;
  registerI = 100;
  programStorage[0] = invertByteOrder(ADD_I_V9);
}

void verify_add_i_v9() { expectEqualBytes(registerI, 103, "I"); }

void setup_add_v6_v1_no_carry() {
  registers[0x6] = 3;
  registers[0x1] = 2;
  programStorage[0] = invertByteOrder(ADD_V6_V1);
}

void verify_add_v6_v1_no_carry() {
  expectEqualBytes(registers[0x6], 5, "V6");
  expectEqualBytes(registers[0xF], 0, "VF");
}

void setup_add_v6_v1_with_carry() {
  registers[0x6] = 255;
  registers[0x1] = 20;
  programStorage[0] = invertByteOrder(ADD_V6_V1);
}

void verify_add_v6_v1_with_carry() {
  expectEqualBytes(registers[0x6], 19, "V6");
  expectEqualBytes(registers[0xF], 1, "VF");
}

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

void setup_sknp_vd_skips() {
  registers[0xD] = 0xB;
  simulateKey('A');
  programStorage[0] = invertByteOrder(SKNP_VD);
}

void verify_sknp_vd_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_sknp_vd_no_skips() {
  registers[0xD] = 0xB;
  simulateKey('B');
  programStorage[0] = invertByteOrder(SKNP_VD);
}

void verify_sknp_vd_no_skips() { expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC"); }

void setup_sknp_vd_no_keys_pressed() {
  registers[0xD] = 0xB;
  // simulateKey('B');
  programStorage[0] = invertByteOrder(SKNP_VD);
}

void verify_sknp_vd_no_keys_pressed() { expectEqualPtrs(chip8PC, (uint16_t *)0x204, "PC"); }

void setup_key_v5() {
  registers[5] = 2;
  simulateKey('2');
  programStorage[0] = invertByteOrder(KEY_V5);
}

void verify_key_v5() {
  expectEqualBytes(registers[5], 2, "V5");
  expectEqualPtrs(chip8PC, (uint16_t *)0x202, "PC");
}

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

void setup_and_va_vb() {
  registers[0xA] = 0xF0;
  registers[0xB] = 0x81;
  programStorage[0] = invertByteOrder(AND_VA_VB);
}

void verify_and_va_vb() { expectEqualBytes(registers[0xA], 0x80, "VA"); }

// SHR Quicks issues - see https://www.reddit.com/r/EmuDev/comments/72dunw/chip8_8xy6_help/dno06ix/?utm_source=reddit&utm_medium=web2x&context=3

void setup_shr_va_vb_bit_set() {
  registers[0xA] = 0x81;
  // registers[0xB] = 0x81;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SHR_VA_VB);
}

void verify_shr_va_vb_bit_set() {
  expectEqualBytes(registers[0xA], 0x40, "VA");
  // expectEqualBytes(registers[0xB], 0x81, "VB");
  expectEqualBytes(registers[0xF], 0x1, "VF");
}

void setup_shr_va_vb_bit_unset() {
  registers[0xA] = 0x40;
  // registers[0xB] = 0x40;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SHR_VA_VB);
}

void verify_shr_va_vb_bit_unset() {
  expectEqualBytes(registers[0xA], 0x20, "VA");
  // expectEqualBytes(registers[0xB], 0x40, "VB");
  expectEqualBytes(registers[0xF], 0x0, "VF");
}

void setup_shl_va_vb_bit_set() {
  registers[0xA] = 0x81;
  // registers[0xB] = 0x81;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SHL_VA_VB);
}

void verify_shl_va_vb_bit_set() {
  expectEqualBytes(registers[0xA], 0x02, "VA");
  // expectEqualBytes(registers[0xB], 0x81, "VB");
  expectEqualBytes(registers[0xF], 0x1, "VF");
}

void setup_shl_va_vb_bit_unset() {
  registers[0xA] = 0x40;
  // registers[0xB] = 0x40;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SHL_VA_VB);
}

void verify_shl_va_vb_bit_unset() {
  expectEqualBytes(registers[0xA], 0x80, "VA");
  // expectEqualBytes(registers[0xB], 0x40, "VB");
  expectEqualBytes(registers[0xF], 0x0, "VF");
}

void setup_sub_v3_ve_no_borrow() {
  registers[0x3] = 0x20;
  registers[0xE] = 0x10;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SUB_V3_VE);
}

void verify_sub_v3_ve_no_borrow() {
  expectEqualBytes(registers[0x3], 0x10, "V3");
  expectEqualBytes(registers[0xE], 0x10, "VE");
  expectEqualBytes(registers[0xF], 0x1, "VF");
}

void setup_sub_v3_ve_with_borrow() {
  registers[0x3] = 0x10;
  registers[0xE] = 0x28;
  registers[0xF] = 0x00;
  programStorage[0] = invertByteOrder(SUB_V3_VE);
}

void verify_sub_v3_ve_with_borrow() {
  expectEqualBytes(registers[0x3], 0xE8, "V3");
  expectEqualBytes(registers[0xE], 0x28, "VE");
  expectEqualBytes(registers[0xF], 0x0, "VF");
}

void setup_xor_v3_ve() {
  registers[0x3] = 0x41;
  registers[0xE] = 0x40;
  programStorage[0] = invertByteOrder(XOR_V3_VE);
}

void verify_xor_v3_ve() {
  expectEqualBytes(registers[0x3], 0x01, "V3");
  expectEqualBytes(registers[0xE], 0x40, "VE");
}

void setup_bcd_i_v3() {
  registerI = 0x600;
  memset((void *)registerI, 0, 3);
  registers[0x3] = 128;
  programStorage[0] = invertByteOrder(BCD_I_V3);
}

void verify_bcd_i_v3() {
  byte *i = (byte *)0x600;

  expectEqualBytes(i[0], 1, "[i+0]");
  expectEqualBytes(i[1], 2, "[i+1]");
  expectEqualBytes(i[2], 8, "[i+2]");
}

void setup_ldf_i_v4() {
  registers[0x4] = 1;
  programStorage[0] = invertByteOrder(LDF_I_V4);
}

void verify_ldf_i_v4() { expectEqualInts(registerI, (uint16_t)&fonts[5], "I"); }

void main() {
  CommandSwitches.isSerial = true;

  assert(add_i_v9);
  assert(add_v6_v1_no_carry);
  assert(add_v6_v1_with_carry);
  assert(add_ve_1);
  assert(and_va_vb);
  assert(bcd_i_v3);
  assert(call_1025);
  assert(cls);
  assert(draw_top_right);
  assert(draw_xor);
  assert(jp_1026);
  assert(key_v5);
  assert(ld_dt_v3);
  assert(ld_i_1234);
  assert(ld_i_ve);
  assert(ld_st_v2);
  assert(ld_v0_i);
  assert(ld_v1_10);
  assert(ld_v3_va); // LD_V3_VA)
  assert(ld_va_dt);
  assert(ld_vb_i);
  assert(ldf_i_v4);
  assert(ret_from_subroutine);
  assert(rnd_ve_15);
  assert(se_v4_15_no_skips);
  assert(se_v4_15_skips);
  assert(se_v4_v9_no_skips);
  assert(se_v4_v9_skips);
  assert(shl_va_vb_bit_set);
  assert(shl_va_vb_bit_unset);
  assert(shr_va_vb_bit_set);
  assert(shr_va_vb_bit_unset);
  assert(sknp_vd_no_keys_pressed);
  assert(sknp_vd_no_skips);
  assert(sknp_vd_skips);
  assert(skp_v3_no_skips);
  assert(skp_v3_skips);
  assert(skp_v3_wrong_key);
  assert(sne_v0_123_no_skips);
  assert(sne_v0_123_skips);
  assert(sne_v0_v2_no_skips);
  assert(sne_v0_v2_skips);
  assert(sub_v3_ve_no_borrow);
  assert(sub_v3_ve_with_borrow);
  assert(xor_v3_ve);
  assertTerminates(final_ret);

  xprintf(testFailure ? RED "Tests Failed\r\n" RESET : BRIGHT_WHITE "All Passed\r\n" RESET);
}
