#include "tms.h"
#include "datatypes.h"
#include "hbios.h"

#include "keys.h"
#include "xstdio.h"

tms9918IoPorts tmsIoPorts;

byte tmsRegisters[8] = {
    0x00,                               /* Graphics I Mode,No External Video */
    0xC0 | 32,                          /* 16K,Enable Display, Enable Int.,8x8 Sprites,Mag.Off */
    0x05,                               /* Address of Name Table in VRAM = Hex 1400 */
    0x80,                               /* Address of Color Table in VRAM = Hex 2000*/
    0x01,                               /* Address of Pattern Table in VRAM = Hex 0800*/
    0x20,                               /* Address of Sprite Attribute Table in VRAM = Hex 1000*/
    0x00,                               /* Address of Sprite Pattern Table in VRAM = Hex 0000*/
    COL_DRKGREEN *(byte)16 + COL_BLACK, /* Colours */
};

static tmsClearParams clearParams = {0x0000, 16 * 1024, 0};

void tmsInit() {
  hbiosDriverEntry data;
  data.func = 0;
  data.unit = 0;

  // TODO : Check result for error
  hbSysGetVda(&data);

  hbiosBankCopy copyData;
  copyData.sourceBank = HCB_BIDBIOS;
  copyData.destBank = HCB_BIDUSR;
  copyData.sourceAddr = ((byte *)data.driverDataAddr) + 4;
  copyData.destAddr = (void *)tmsIoPorts;
  copyData.byteCount = 2;

  // TODO : Check result for error
  hbSysBankCopy(&copyData);

  tmsInitCode();
  tmsClearData(&clearParams);
}
