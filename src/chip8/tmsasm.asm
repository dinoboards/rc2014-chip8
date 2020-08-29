	PUBLIC  _tmsSetMode1, _tmsWriteData, _tmsReadData, _tmsClearData, _tmsReadByte, _tmsWriteByte, _tmsSetReadAddr, _tmsSetWriteAddr
	EXTERN	_tmsRegisters, _tmsColour, _tmsIoPorts

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

	SECTION IGNORE
