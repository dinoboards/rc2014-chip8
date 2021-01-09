
	PUBLIC	_outDat, _outCmd, _commandDrawLine, _waitForCommandCompletion, _setReadRegisterToZero
	PUBLIC	_commandFromR32, _commandFromR36
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


;	LOAD REGISTERS FORM R36 TO 45
;	WAITS FOR COMMAND COMPLETION BEFORE STARTING
;	RETURNS WITHOUT WAITING FOR COMPLETION

_commandFromR36:
_commandDrawLine:
	DI
	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_commandDrawLineReady:
	IN	A, (VDP_ADDR)
	RRCA
	JR	C, _commandDrawLineReady

	; SET INDIRECT REGISTER TO 36
	LD	A, 36
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	HL, __fromX
	LD	C, VDP_REGS
	LD	B, 11
	OTIR

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	EI
	RET

;	LOAD REGISTERS FORM R32 TO 45
;	DOES NOT WAITS FOR COMMAND COMPLETION BEFORE STARTING
;	WAITS FOR COMPLETION BEFORE RETURNING
;	RETURNS VALUE IN S#7 in A
_commandFromR32:
	DI

	; SET INDIRECT REGISTER TO 32
	LD	A, 32
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	HL, R32
	LD	C, VDP_REGS
	LD	B, 15
	OTIR

	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_commandFromR32Ready:
	IN	A, (VDP_ADDR)
	RRCA
	JR	C, _commandFromR32Ready

	LD	A, 7
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	IN	A, (VDP_ADDR)
	PUSH	AF

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	POP	AF
	EI
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
