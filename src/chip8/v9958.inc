
CMD_VDP_TO_VRAM:	EQU	0xC0
CMD_LINE:		EQU	0x70
CMD_PST:		EQU	0x50
CMD_LOGIC_IMP:		EQU	0x00
CMD_LOGIC_AND:		EQU	0x01
CMD_LOGIC_OR:		EQU	0x02
CMD_LOGIC_XOR:		EQU	0x03
CMD_LOGIC_NOT:		EQU	0x04
CMD_POINT:		EQU	0x40

; V9958 REGISTERS
VDP_DATA:       equ     $98             ; VDP data port (VRAM read/write)
VDP_ADDR:       equ     $99             ; VDP address (write only)
VDP_STAT:       equ     $99             ; VDP status (read only)
VDP_PALT:       equ     $9A             ; VDP palette latch (write only)
VDP_REGS:       equ     $9B             ; VDP register access (write only)
