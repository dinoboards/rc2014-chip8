	PUBLIC  _tmsSetMode1, _tmsWriteData, _tmsReadData, _tmsClearData, _tmsReadByte
	PUBLIC _tmsWriteByte, _tmsSetReadAddr, _tmsSetWriteAddr, _tmsInstallInterruptHandler, _tmsRemoveInterruptHandler
	EXTERN	_tmsRegisters, _tmsColour, _tmsIoPorts
	EXTERN _timerTick

	SECTION CODE

; void initMode1()
tmsIoDelay:
	PUSH 	AF
	POP	AF
	RET

; 27 TSTATES
#define		TMS_IODELAY	CALL tmsIoDelay

_tmsSetMode1:
	LD	HL, _tmsRegisters
	LD	B, 8
	LD	D, 0x7F			;$80 - 1

INITMODE1:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, (HL)

	DI
	OUT	(C), A
	TMS_IODELAY
	INC	HL
	INC	D
	LD	A, D
	OUT	(C), A
	TMS_IODELAY

	DJNZ	INITMODE1
	EI
	RET

;extern void tmsWriteData(tmsDataParams* p) __z88dk_fastcall;
_tmsWriteData:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, (HL)
	INC	HL
	DI
	OUT	(C), A
	TMS_IODELAY

	LD	A, (HL)
	INC	HL
	OR	$40
	OUT	(C), A
	TMS_IODELAY

	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	LD	H, (HL)		; DE IS LENGTH
	LD	L, A		; HL IS SOURCE ADDRES

	LD	A, (_tmsIoPorts)
	LD	C, A

_tmsWriteData1:
	LD	B, (hl)
	INC	HL
	OUT	(C), B
	TMS_IODELAY
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsWriteData1
	EI
	RET


;extern void tmsReadData(tmsDataParams* p) __z88dk_fastcall;
_tmsReadData:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, (HL)
	INC	HL
	DI
	OUT	(C), A
	TMS_IODELAY

	LD	A, (HL)
	INC	HL
	AND	$3F
	OUT	(C), A
	TMS_IODELAY

	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	INC	HL
	LD	A, (HL)
	INC	HL
	LD	H, (HL)		; DE IS LENGTH
	LD	L, A		; HL IS SOURCE ADDRES

	LD	A, (_tmsIoPorts)
	LD	C, A

_tmsReadData1:
	IN	A, (C)
	TMS_IODELAY
	LD	(HL), A
	INC	HL
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsReadData1
	EI
	RET


;extern void tmsClearData(tmsClearParams* p) __z88dk_fastcall;
_tmsClearData:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, (HL)
	INC	HL
	DI
	OUT	(C), A
	TMS_IODELAY

	LD	A, (HL)
	INC	HL
	OR	$40
	OUT	(C), A
	TMS_IODELAY

	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	INC	HL
	LD	B, (HL)

	LD	A, (_tmsIoPorts)
	LD	C, A

_tmsClearData1:
	INC	HL
	OUT	(C), B
	TMS_IODELAY
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsClearData1

	EI
	RET



;extern void tmsSetWriteAddr(void*) __z88dk_fastcall;

_tmsSetWriteAddr:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, l
	DI
	OUT	(C), A
	LD	A, H
	OR	$40
	OUT	(C), A
	EI
	RET


;extern void tmsSetReadAddr(void*) __z88dk_fastcall;
_tmsSetReadAddr:
	LD	A, (_tmsIoPorts + 1)
	LD	C, A
	LD	A, L
	DI
	OUT	(C), A
	LD	A, H
	AND	$3F
	OUT	(C), A
	EI
	RET


; extern void tmsWriteByte(byte) __z88dk_fastcall;
_tmsWriteByte:
	LD	A, (_tmsIoPorts)
	LD	C, A
	LD	A, L
	OUT	(C), A
	RET

; extern byte tmsReadByte() __z88dk_fastcall;
_tmsReadByte:
	LD	A, (_tmsIoPorts)
	LD	C, A
	IN	L, (C)
	RET


;;;;
; INTERRUPT HANDLING (VSYNC) FOR TIMER TICK

include "hbios_sys.inc"

intHandler:
	LD	A, (_tmsIoPorts + 1) 	; CMDREG
 	LD	C, A
	IN	A, (C)			; TEST FOR INT FLAG
	AND	$80
	JR	NZ, tmsInt

	JP	0
nextHandler	EQU	$-2

tmsInt:
	LD	HL, (_timerTick)
	INC	HL
	LD	(_timerTick), HL
	OR	$FF
	RET

_tmsInstallInterruptHandler:
	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_SET

	LD	E, 0
	LD	HL, intHandler

	RST	08

	LD	(nextHandler), HL

	POP	IX
	RET

_tmsRemoveInterruptHandler:
	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_SET

	LD	E, 0
	LD	HL, (nextHandler)

	RST	08

	LD	L, A

	POP	IX
	RET


	SECTION IGNORE
