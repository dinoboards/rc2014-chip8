#include "filereader.h"
#include "charconstants.h"
#include "cpm.h"
#include "datatypes.h"

static uint8_t  diskBuffer[129];
static uint8_t *diskBufferP;

static FCB *fcb;

void setFileStream(FCB *_fcb) { fcb = _fcb; }

void openFileStream() {
  chkMsg(fOpen(fcb), "Unable to open file");
  fDmaOff(diskBuffer);
  chkMsg(fRead(fcb), "Unable to read file");

  diskBufferP = diskBuffer;
}

void closeFileStream() { chk(fClose(fcb)); }

char getNextCharRaw() {
  if (diskBufferP >= &diskBuffer[128]) {
    fDmaOff(diskBuffer);
    chkMsg(fRead(fcb), "Unable to read file");
    diskBufferP = diskBuffer;
  }

  const char b = *diskBufferP;

  if (b == CTRL_Z || b == 0)
    return '\0';

  diskBufferP += 1;

  return b;
}
