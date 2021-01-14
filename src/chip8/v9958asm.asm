
	PUBLIC	_waitForCommandCompletion, _setReadRegisterToZero
	PUBLIC	__color

	SECTION CODE

	include "v9958.inc"

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

R44:
__color:	DB	0

	SECTION IGNORE
