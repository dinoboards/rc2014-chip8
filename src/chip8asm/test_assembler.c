#include "assembler.h"
#include "chip8asm/labels.h"
#include "chip8asm/systemstate.h"
#include "cpm.h"
#include "test_opcodes.h"
#include "xstdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "test_helper.h"

void main() {
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
  shouldAssemble("JP 1026", JP_1026);
  shouldAssemble("LD ST, V2", LD_ST_V2);
  shouldAssemble("LD DT, V3", LD_DT_V3);
  shouldAssemble("SKP V3", SKP_V3);
  shouldAssemble("LD VA, DT", LD_VA_DT);
  shouldAssemble("RND vE, 15", RND_VE_15);

  shouldError("BAD INSTRUCTION", "Expected Instruction but found BAD");
  shouldError("LD BADREG, 123", "Expected one of Vx, I, ST or DT but found BADREG");
  shouldError("JP NOWHERE", "Unknown label 'NOWHERE' referenced at line number 1");

  xprintf(testFailure ? RED "Tests Failed\r\n" RESET : BRIGHT_WHITE "All Passed\r\n" RESET);
}
