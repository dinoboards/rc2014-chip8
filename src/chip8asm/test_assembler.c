#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "cpm.h"
#include "expr.h"
#include "test_opcodes.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "test_expectations.h"
#include "test_helper.h"

void testEvaluation() {
  shouldEvaluate("1234", 1234);
  shouldEvaluate("10*2", 20);

  shouldEvaluate("-123", -123);
  shouldEvaluate("-123*10", -123 * 10);
  shouldEvaluate("100/10", 100 / 10);
  shouldEvaluate("100+10", 100 + 10);
  shouldEvaluate("100+10*2", 100 + 10 * 2);
  shouldEvaluate("(100+10)*2", (100 + 10) * 2);
  shouldEvaluate("110 % 10", 100 % 10);
  shouldEvaluate("64 | 32", 64 | 32);
  shouldEvaluate("1 + 2 | 1", 1 + 2 | 1);
  shouldEvaluate("1 + (2 | 1)", 1 + (2 | 1));
  shouldEvaluate("(1 + 2) | 1", (1 + 2) | 1);

  shouldEvaluate("1 + 3 & 1", 1 + 3 & 1);
  shouldEvaluate("1 + (3 & 1)", 1 + (3 & 1));
  shouldEvaluate("(1 + 3) & 1", (1 + 3) & 1);

  shouldEvaluate("2 | 3 & 0", 2 | 3 & 0);
  shouldEvaluate("2 | (3 & 0)", 2 | (3 & 0));
  shouldEvaluate("(2 | 3) & 0", (2 | 3) & 0);

  shouldEvaluateError("A1234567890A1234567890A1234567890A1234567890A1234567890A1234567890A1234567890*10", "Expression item too long");

  shouldEvaluateError("@@@", "Expression had unexpected character '@' at line number 0");
  shouldEvaluateError("[I]", "Expression had unexpected character '[' at line number 0");
}
void main() {

  testEvaluation();

  shouldAssemble("ADD I, V9", ADD_I_V9);
  shouldAssemble("ADD V6, V1", ADD_V6_V1);
  shouldAssemble("ADD VE, 1", ADD_VE_1);
  shouldAssemble("AND VA, VB", AND_VA_VB);
  shouldAssemble("BCD [I], V3", BCD_I_V3);
  shouldAssemble("CALL 1025", CALL_1025);
  shouldAssemble("CALL LABL\r\n  LABL: RET", 0x2202);
  shouldAssemble("CLS", CLS);
  shouldAssemble("DRW V2, V3, 1", DRAW_V2_V3_1);
  shouldAssemble("DRW V2, V3, 11", DRAW_V2_V3_11);
  shouldAssemble("JP 1026", JP_1026);
  shouldAssemble("KEY V5", KEY_V5);
  shouldAssemble("LD [I], VE", LD_I_VE);
  shouldAssemble("LD [I], VA..V1", LD_I_VA_V1);
  shouldAssemble("LD DT, V3", LD_DT_V3);
  shouldAssemble("LD I, 1234", LD_I_1234);
  shouldAssemble("LD ST, V2", LD_ST_V2);
  shouldAssemble("LD V1, 10", LD_V1_10);
  shouldAssemble("LD V3, va", LD_V3_VA);
  shouldAssemble("LD VA, DT", LD_VA_DT);
  shouldAssemble("LD VB, [i]", LD_VB_I);
  shouldAssemble("LD V5..V6, [i]", LD_V5_V6_I);
  shouldAssemble("LDF I, V4", LDF_I_V4);
  shouldAssemble("RET", RET);
  shouldAssemble("RND vE, 15", RND_VE_15);
  shouldAssemble("SE V4, 15", SE_V4_15);
  shouldAssemble("SE V4, v9", SE_V4_V9);
  shouldAssemble("SHL VA, VB", SHL_VA_VB);
  shouldAssemble("SHL VA", SHL_VA);
  shouldAssemble("SHR VA, VB", SHR_VA_VB);
  shouldAssemble("SHR VD", SHR_VD);
  shouldAssemble("SKNP VD", SKNP_VD);
  shouldAssemble("SKP V3", SKP_V3);
  shouldAssemble("SNE V0, 123", SNE_V0_123);
  shouldAssemble("SNE V0, V2", SNE_V0_V2);
  shouldAssemble("SUB V3, VE", SUB_V3_VE);
  shouldAssemble("XOR V3, VE", XOR_V3_VE);
  shouldAssemble("OR V7, V2", OR_V7_V2);
  shouldAssemble("SUBN V2, VE", SUBN_V2_VE);
  shouldAssemble("JP V0, 1024", JP_V0_1024);
  shouldAssemble("HIGH", HIGHRES);
  shouldAssemble("PLANE 2", PLANE_2);
  shouldAssemble("AUDIO", AUDIO);

  shouldAssemble("DB 2, 255", 0x02FF);
  shouldAssembleDS("DB 1, 2, 3, 4", 4);
  shouldAssembleDS("DS 50", 50);
  shouldAssembleDS("DS 4096", 4096);
  shouldAssembleDS("DS 6000", 6000);
  shouldAssembleDblWidth("LD I, 6000", LD_IL_6000_1, LD_IL_6000_2);

  shouldAssemble("SCRL DOWN 5", SCRL_DOWN_5);
  shouldAssemble("SCRL UP 5", SCRL_UP_5);
  shouldAssemble("SCRL LEFT", SCRL_LEFT);
  shouldAssemble("SCRL RIGHT", SCRL_RIGHT);

  shouldError("BAD INSTRUCTION", "Expected Instruction but found BAD");
  shouldError("LD BADREG, 123", "Expected one of Vx, I, [I], ST or DT but found BADREG");
  shouldError("JP NOWHERE", "Unknown label 'NOWHERE' referenced at line number 1");

  printf(testFailure ? RED "Tests Failed\r\n" RESET : BRIGHT_WHITE "All Passed\r\n" RESET);
}
