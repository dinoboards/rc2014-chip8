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

  shouldAssemble("LD V3, va", LD_V3_VA);
  shouldAssemble("LD V1, 10", LD_V1_10);
  shouldAssemble("LD I, 1234", LD_I_1234);
  shouldAssemble("DRW V2, V3, 11", DRAW_V2_V3_11);
  shouldAssemble("DRW V2, V3, 1", DRAW_V2_V3_1);
  shouldAssemble("DB 2, 255", 0x02FF);
  shouldAssemble("CALL 1025", CALL_1025);
  shouldAssemble("CALL LABL\r\n  LABL: RET", 0x2202);
  shouldAssemble("CLS", CLS);
  shouldAssemble("RET", RET);
  shouldAssemble("ADD VE, 1", ADD_VE_1);
  shouldAssemble("SE V4, 15", SE_V4_15);
  shouldAssemble("SE V4, v9", SE_V4_V9);
  shouldAssemble("SNE V0, 123", SNE_V0_123);
  shouldAssemble("SNE V0, V2", SNE_V0_V2);
  shouldAssemble("JP 1026", JP_1026);
  shouldAssemble("LD ST, V2", LD_ST_V2);
  shouldAssemble("LD DT, V3", LD_DT_V3);
  shouldAssemble("SKP V3", SKP_V3);
  shouldAssemble("SKNP VD", SKNP_VD);
  shouldAssemble("LD VA, DT", LD_VA_DT);
  shouldAssemble("RND vE, 15", RND_VE_15);
  shouldAssemble("ADD I, V9", ADD_I_V9);
  shouldAssemble("LD VB, [i]", LD_VB_I);
  shouldAssemble("AND VA, VB", AND_VA_VB);

  shouldError("BAD INSTRUCTION", "Expected Instruction but found BAD");
  shouldError("LD BADREG, 123", "Expected one of Vx, I, ST or DT but found BADREG");
  shouldError("JP NOWHERE", "Unknown label 'NOWHERE' referenced at line number 1");

  xprintf(testFailure ? RED "Tests Failed\r\n" RESET : BRIGHT_WHITE "All Passed\r\n" RESET);
}
