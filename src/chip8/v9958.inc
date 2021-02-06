
CMD_VRAM_TO_CPU:		EQU	0xA0
CMD_CPU_TO_VRAM:		EQU	0xB0
CMD_VDP_TO_VRAM:		EQU	0xC0
CMD_HIGH_SPEED_VRAM_TO_VRAM:	EQU	0xD0
CMD_VRAM_TO_VRAM_Y_ONLY:	EQU	0xE0
CMD_LINE:			EQU	0x70
CMD_PST:			EQU	0x50
CMD_LOGIC_IMP:			EQU	0x00
CMD_LOGIC_AND:			EQU	0x01
CMD_LOGIC_OR:			EQU	0x02
CMD_LOGIC_XOR:			EQU	0x03
CMD_LOGIC_NOT:			EQU	0x04
CMD_POINT:			EQU	0x40

; V9958 REGISTERS
VDP_DATA:       		EQU     $98             ; VDP data port (VRAM read/write)
VDP_ADDR:       		EQU     $99             ; VDP address (write only)
VDP_STAT:       		EQU     $99             ; VDP status (read only)
VDP_PALT:       		EQU     $9A             ; VDP palette latch (write only)
VDP_REGS:       		EQU     $9B             ; VDP register access (write only)

HIRES_WIDTH:			EQU	128
HIRES_WIDTH_MASK:		EQU	255
HIRES_HEIGHT:			EQU	64
HIRES_HEIGHT_MASK:		EQU	127

