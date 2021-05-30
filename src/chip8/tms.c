#include "tms.h"
#include "datatypes.h"
#include "keys.h"
#include "systemstate.h"

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

void tmsRegisterColours(uint8_t bkColour, uint8_t fgColour) { tmsRegisters[7] = fgColour * (byte)16 + bkColour; }

static uint8_t tmsDriverIndex = 0;

void tmsInit() { tmsClearData(&clearParams); }
