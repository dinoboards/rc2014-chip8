	PUBLIC	_initDrawParams

	SECTION CODE

	include	"v9958.inc"

_initDrawParams:
	; clear all registers
	LD	A, 32
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	B, 15
	XOR	A
loop:
	OUT	(VDP_REGS), A
	DJNZ	loop
	RET

