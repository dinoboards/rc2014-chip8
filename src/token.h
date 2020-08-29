#ifndef __TOKEN
#define __TOKEN

extern char tokenCurrentChar;
extern char tokenTerminatorChar;

extern int lineNumber;

#define getCurrentLineNumber() lineNumber

extern void getNextToken();
extern void openTokenStream();
extern void closeTokenStream();
extern void getToLineEnd();

extern char getNextChar();

#endif
