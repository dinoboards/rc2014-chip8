#include "labels.h"
#include "datatypes.h"
#include "xstdio.h"
#include <string.h>

#define MAX_LABEL_STORAGE 4096

typedef struct labelAddressesStruct {
  uint16_t address; // if 0, then yet to be assigned.
  char     value[];
} labelAddresses;

extern byte            labelStorage[MAX_LABEL_STORAGE];
static labelAddresses *nextEmptyPtr;

static labelAddresses *next(const labelAddresses *current) __z88dk_fastcall {
  byte *p = (byte *)current;

  p += (sizeof(uint16_t) + 1 + strlen(current->value));

  return (labelAddresses *)p;
}

static labelAddresses *findLabel(const char *label) __z88dk_fastcall {
  labelAddresses *p = (labelAddresses *)&labelStorage;

  while (p < nextEmptyPtr) {
    if (strcmp(p->value, label) == 0)
      return p;

    p = next(p);
  }

  return NULL;
}

uint16_t findLabelAddress(const char *label) __z88dk_fastcall {
  labelAddresses *p = findLabel(label);
  if (p)
    return p->address;

  return 0;
}

void addLabel(const char *label, uint16_t address, bool allowDuplicates) {
  xtracef("%s = %04X @ %p\r\n", label, address, nextEmptyPtr);

  labelAddresses *p = findLabel(label);
  if (!allowDuplicates && p) {
    xprintf("Duplicate label found: %s\r\n", label);
    exit(1);
  }

  if (p) {
    nextEmptyPtr->address = address;
    return;
  }

  nextEmptyPtr->address = address;
  strcpy(nextEmptyPtr->value, label);

  nextEmptyPtr = next(nextEmptyPtr);
}

void initLabelStorage() { nextEmptyPtr = (labelAddresses *)&labelStorage; }
