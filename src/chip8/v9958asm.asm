
	PUBLIC	_waitForCommandCompletion, _setReadRegisterToZero, _initDrawParams, _waitAndClearCommandRegisters
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

_waitAndClearCommandRegisters:
	DI
	CALL	_waitForCommandCompletion
	CALL	_initDrawParams
	CALL	_setReadRegisterToZero
	EI
	RET

	SECTION	DATA

R44:
__color:	DB	0

	SECTION IGNORE
