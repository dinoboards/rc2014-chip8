	PUBLIC  _tmsSetMode1, _tmsInitCode, _tmsWriteData, _tmsReadData, _tmsClearData, _tmsReadByte, _tmsWriteByte, _tmsSetReadAddr, _tmsSetWriteAddr
	EXTERN	_tmsRegisters, _tmsColour, _tmsIoPorts

	SECTION CODE

; void tmsInitCode()

_tmsInitCode:	; self modify code to point to correct io ports for TMS chip
	ld	a, (_tmsIoPorts)
	ld	(DATREG1), a
	ld	(DATREG2), a
	ld	(DATREG3), a
	ld	(DATREG4), a
	ld	(DATREG5), a

	ld	a, (_tmsIoPorts + 1)
	ld	(CMDREG1), a
	ld	(CMDREG2), a
    	LD	(CMDREG3), A
	LD	(CMDREG4), A
	LD	(CMDREG5), A
	LD	(CMDREG6), A
	LD	(CMDREG7), A
	LD	(CMDREG8), A
	LD	(CMDREG9), A
	LD	(CMDREG10), A
	LD	(CMDREG11), A
	LD	(CMDREG12), A
    	RET

; void initMode1()
tmsIoDelay:
	push af
	pop	af
	push	af
	pop	af
	push af
	pop	af
	RET

; 27 TSTATES
#define		TMS_IODELAY	CALL tmsIoDelay

_tmsSetMode1:
	LD	HL, _tmsRegisters
	LD	B, 8
	LD	C, 0x7F			;$80 - 1

INITMODE1:
	LD	A, (HL)
	OUT	(0), A
CMDREG1:	EQU	$-1
	TMS_IODELAY
	INC	HL
	INC	C
	LD	A, C
	OUT	(0), A
CMDREG2:	EQU	$-1
	TMS_IODELAY
	DJNZ	INITMODE1
	RET

;extern void tmsWriteData(tmsDataParams* p) __z88dk_fastcall;
_tmsWriteData:
	LD	A, (hl)
	inc	hl
	OUT	(0), A
CMDREG3:	EQU	$-1
	LD	A, (hl)
	inc	hl
	OR	$40
	OUT	(0), A
CMDREG4:	EQU	$-1

	ld	E, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	ld	h, (hl)		; de is length
	ld	l, a		; hl is source addres

	LD	C, 0
DATREG1:	EQU	$-1

_tmsWriteData1:
	LD	B, (hl)
	inc	hl
	OUT	(C), B
	TMS_IODELAY
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsWriteData1
	RET


;extern void tmsReadData(tmsDataParams* p) __z88dk_fastcall;
_tmsReadData:
	LD	A, (hl)
	inc	hl
	OUT	(0), A
CMDREG5:	EQU	$-1
	LD	A, (hl)
	inc	hl
	AND	$3F
	OUT	(0), A
CMDREG6:	EQU	$-1

	ld	E, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	ld	h, (hl)		; de is length
	ld	l, a		; hl is source addres

	LD	C, 0
DATREG2:	EQU	$-1

_tmsReadData1:
	IN	a, (C)
	TMS_IODELAY
	LD	(hl), a
	inc	hl
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsReadData1
	RET


;extern void tmsClearData(tmsClearParams* p) __z88dk_fastcall;
_tmsClearData:
	LD	A, (hl)
	inc	hl
	OUT	(0), A
CMDREG7:	EQU	$-1
	LD	A, (hl)
	inc	hl
	OR	$40
	OUT	(0), A
CMDREG8:	EQU	$-1

	ld	E, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	b, (hl)

	LD	C, 0
DATREG3:	EQU	$-1

_tmsClearData1:
	inc	hl
	OUT	(C), B
	TMS_IODELAY
	DEC	DE
	LD	A, E
	OR	D
	JR	NZ, _tmsClearData1
	RET




;extern void tmsSetWriteAddr(void*) __z88dk_fastcall;

_tmsSetWriteAddr:
	di
	LD	A, l
	OUT	(0), A
CMDREG9:	EQU	$-1
	TMS_IODELAY
	LD	A, h
	OR	$40
	OUT	(0), A
CMDREG10:	EQU	$-1
	TMS_IODELAY
	ei
	RET


;extern void tmsSetReadAddr(void*) __z88dk_fastcall;
_tmsSetReadAddr:
	di
	LD	A, l
	OUT	(0), A
CMDREG11:	EQU	$-1
	TMS_IODELAY
	LD	A, h
	AND	$3F
	OUT	(0), A
CMDREG12:	EQU	$-1
	TMS_IODELAY
	ei
	RET


; extern void tmsWriteByte(byte) __z88dk_fastcall;

_tmsWriteByte:
	di
	TMS_IODELAY

	ld	a, l
	OUT	(0), a
DATREG4:	EQU	$-1
	ei
	RET

; extern byte tmsReadByte() __z88dk_fastcall;

_tmsReadByte:
	di
	TMS_IODELAY

	IN	a, (0)
DATREG5:	EQU	$-1
	ld	l, a
	ei
	RET

	SECTION IGNORE
