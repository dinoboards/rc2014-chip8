#include "tms.h"
#include "datatypes.h"
#include "hbios.h"

tms9918Registers tmsRegisters;

void captureTmsRegisters() {
  hbiosDriverEntry data;
  data.func = 0;
  data.unit = 0;

  // TODO : Check result for error
  hbSysGetVda(&data);

  hbiosBankCopy copyData;
  copyData.sourceBank = HCB_BIDBIOS;
  copyData.destBank = HCB_BIDUSR;
  copyData.sourceAddr = ((byte *)data.driverDataAddr) + 4;
  copyData.destAddr = (void *)tmsRegisters;
  copyData.byteCount = 2;

  // TODO : Check result for error
  hbSysBankCopy(&copyData);
}
