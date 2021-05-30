	PUBLIC  _tmsSetMode1, _tmsWriteData, _tmsReadData, _tmsClearData, _tmsReadByte
	PUBLIC _tmsWriteByte, _tmsSetReadAddr, _tmsSetWriteAddr
	EXTERN	_tmsRegisters, _tmsColour
	EXTERN _timerTick

	SECTION CODE

include	"v9958.inc"

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
	LD	C, VDP_ADDR
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
	LD	C, VDP_ADDR
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

	LD	C, VDP_DATA

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
	LD	C, VDP_ADDR
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

	LD	C, VDP_DATA

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
	LD	A, 0			; RESET V9958 upper address lines to 0
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 14
	OUT	(VDP_ADDR), A


	LD	C, VDP_ADDR
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

	LD	C, VDP_DATA

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
	LD	C, VDP_ADDR
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
	LD	C, VDP_ADDR
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
	LD	C, VDP_DATA
	LD	A, L
	OUT	(C), A
	RET

; extern byte tmsReadByte() __z88dk_fastcall;
_tmsReadByte:
	LD	C, VDP_DATA
	IN	L, (C)
	RET

	SECTION IGNORE
