
#ifndef __ERROR_REPORTS
#define __ERROR_REPORTS

#include "datatypes.h"

extern void expectedError(const char *) __z88dk_fastcall;
extern void unknownLabelError(const char *) __z88dk_fastcall;
extern void errorBadExpression(const char *) __z88dk_fastcall;
extern void errorUnexpectedCharacter(char) __z88dk_fastcall;
extern void errorUnexpectedContent(void);
extern void errorExpectedChar(char) __z88dk_fastcall;
extern void errorExpressionTooLong(void);
extern void errorIntTooLarge(const uint16_t x) __z88dk_fastcall;

#endif
