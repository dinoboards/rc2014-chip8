
#ifndef __ERROR_REPORTS
#define __ERROR_REPORTS

extern void expectedError(const char *) __z88dk_fastcall;
extern void unknownLabelError(const char *) __z88dk_fastcall;
extern void errorBadExpression(const char *) __z88dk_fastcall;
extern void errorUnexpectedCharacter(char) __z88dk_fastcall;
extern void errorUnexpectedContent();
extern void errorExpectedChar(char) __z88dk_fastcall;
extern void errorExpressionTooLong();

#endif
