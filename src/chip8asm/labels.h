#ifndef __LABELS
#define __LABELS
#include "datatypes.h"

void     initLabelStorage();
void     addLabel(const char *label, uint16_t address, bool allowDuplicates);
uint16_t findLabelAddress(const char *label) __z88dk_fastcall;

#endif
