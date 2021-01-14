
	PUBLIC	_outDat, _outCmd, _waitForCommandCompletion, _setReadRegisterToZero
	PUBLIC	__fromX, __fromY, _longSide, _shortSide, __color, _dir, __operation
	PUBLIC	__startX, __startY

	SECTION CODE

	include "v9958.inc"

; void outDat(uint8_t b) __z88dk_fastcall
_outDat:
	LD	A, L
	OUT	(VDP_DATA), A
	RET

; void outCmd(uint8_t b) __z88dk_fastcall
_outCmd:
	LD	A, L
	OUT	(VDP_ADDR), A
	RET

_waitForCommandCompletion:
	; Set read register to 2
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

_waitForCommandCompletionLoop:
	IN	A, (VDP_ADDR)
	RRCA
	JR	C, _waitForCommandCompletionLoop

	RET

_setReadRegisterToZero:
	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	RET


	SECTION	DATA

R32:
__startX:	DW	0

R34:
__startY:	DW	0

R36:
__fromX:	DW	0

R38:
__fromY:	DW	0

R40:
_longSide:	DW	0

R42:
_shortSide:	DW	0

R44:
__color:	DB	0

R45:
_dir:		DB	0

R46:
__operation:	DB	0

	SECTION IGNORE
