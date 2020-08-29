
#ifndef __FILEREADER
#define __FILEREADER

#include "cpm.h"

extern void setFileStream(FCB *_fcb);
extern void openFileStream();
extern void closeFileStream();
extern char getNextCharRaw();

#endif
