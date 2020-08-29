
#ifndef __TMS
#define __TMS

#include "datatypes.h"

#define COL_TRANSPARENT ((byte)0)
#define COL_BLACK       ((byte)1)
#define COL_MEDGREEN    ((byte)2)
#define COL_LIGGREEN    ((byte)3)
#define COL_DRKBLUE     ((byte)4)
#define COL_LIGBLUE     ((byte)5)
#define COL_DRKRED      ((byte)6)
#define COL_CYAN        ((byte)7)
#define COL_MEDRED      ((byte)8)
#define COL_LIGRED      ((byte)9)
#define COL_DRKYELLOW   ((byte)10)
#define COL_LIGYELLOW   ((byte)11)
#define COL_DRKGREEN    ((byte)12)
#define COL_MAGENTA     ((byte)13)
#define COL_GREY        ((byte)14)
#define COL_WHITE       ((byte)15)

#define TMS_MD1_PATTERN_TABLE (0x0800)
#define TMS_MD1_NAME_TABLE    (0x1400)
#define TMS_MD1_COLOUR_TABLE  (0x2000)

typedef struct {
  uint8_t data;
  uint8_t command;
} tms9918IoPorts;

extern tms9918IoPorts tmsIoPorts;

extern void tmsInit();
extern void tmsInitCode();
extern void tmsSetMode1();
extern void tmsRegisterColours(uint8_t bkColour, uint8_t fgColour);

typedef struct {
  uint16_t tmsAddress;
  uint16_t length;
  void *   address;
} tmsDataParams;

extern void tmsWriteData(tmsDataParams *p) __z88dk_fastcall;
extern void tmsReadData(tmsDataParams *p) __z88dk_fastcall;

typedef struct {
  uint16_t tmsAddress;
  uint16_t length;
  uint8_t  data;
} tmsClearParams;

extern void tmsClearData(tmsClearParams *p) __z88dk_fastcall;
extern void tmsWriteByte(uint8_t b) __z88dk_fastcall;
extern byte tmsReadByte() __z88dk_fastcall;
extern void tmsSetWriteAddr(uint16_t) __z88dk_fastcall;
extern void tmsSetReadAddr(uint16_t) __z88dk_fastcall;

#endif
