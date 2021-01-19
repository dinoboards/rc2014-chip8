	PUBLIC	_v9958ScrollUp

	EXTERN	_initDrawParams, _waitForCommandCompletion
	EXTERN	__color, _yy, _fourthNibble, __color
	EXTERN	processPixels, clearLine, readLineFromVdp, COLOR_MASK, LINESRC, LINEDST

	SECTION CODE

	include	"v9958.inc"

	;    c = colour plane
	;    s = line(y) byte
	;    d = line(y+1) byte
	; for y = 62; y > 0; y--
	; read all bytes (y)
	; read all bytes (y+1)
	; setup write command for y+1
	; for each byte:
	; if (c & s) // bit is on
	;   d = d or (s and c)
	; else
	;   d = d and (s or ~c)
	; send byte d

	; (s OR ~c & 3)  (if c == 1, c = 2, if c == 2, c = 1, if c==3, c = 3???)

	; then lastly clear all line 0 for color plane

_v9958ScrollUp:
	LD	A, (__color)
	CP	3
	JR	Z, scrollDownAllPlanes
	LD	C, A
	RRCA
	RRCA
	RRCA
	RRCA
	OR	C
	LD	(COLOR_MASK), A

	DI
	LD	A, 0x80 | 14
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A
	EI

; READ EACH ROW INTO BUFFER
	LD	A, (_fourthNibble)
	LD	D, A			;LOOP FOR Y = COUNT to 63
	LD	C, A

nextRow:
	LD	HL, LINESRC
	CALL	readLineFromVdp

	LD	A, D		; CALCULATE THE DEST LINE INDEX
	SUB	C
	LD	D, A
	CALL	readLineFromVdp

	ld	a, d


	exx

	; WRITE OUT THE TRANSPOSED LINE
	ld	d, a
	XOR	a

	DI
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, D
	or	0x40	; WRITE MODE
	OUT	(VDP_ADDR), A

	LD	HL, LINEDST
	LD	DE, LINESRC

	LD	A, (COLOR_MASK)
	LD	C, A

	ld	b, 128
wrLoop1:
	call	processPixels
	INC	HL
	INC	DE
	out	(VDP_DATA), a
	djnz	wrLoop1

	LD	HL, LINEDST
	ld	b, 128
wrLoop2:
	LD	a, (HL)
	INC	HL
	nop
	nop
	nop
	nop
	out	(VDP_DATA), a
	djnz	wrLoop2

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, $80 | 15
	OUT	(VDP_ADDR), A
	EI
	exx


	LD	A, D		; RESTORE THE DEST LINE INDEX
	ADD	C
	LD	D, A

	INC	D
	LD	A, 63
	CP	D
	JP	NZ, nextRow

	LD	A, (_fourthNibble)
	DEC	A
	LD	D, A

	LD	A, 63
	SUB	D
	LD	D, 63


	LD	A, (COLOR_MASK)
	CPL
	AND	$33
	LD	C, A
clearNextRow:
	LD	HL, LINESRC
	CALL	readLineFromVdp
	CALL	clearLine
	INC	D
	LD	A, 64
	CP	D
	JR	NZ, clearNextRow
	RET

scrollDownAllPlanes:
	LD	A, (_fourthNibble)
	ADD	A	; double it
	LD	D, A
	LD	E, A

	DI
	CALL	_waitForCommandCompletion

	; SETUP COMMAND TO SHIFT UP _fourthNibble*2 DOTS

	; SETUP INDIRECT REGISTER ACCESS FROM R#34
	LD	A, 34
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	A, D
; 	;R34 = SOURCE Y = COUNT (LOW)
	OUT	(VDP_REGS), A

	; HIGH
	XOR	A
	OUT	(VDP_REGS), A

	;R36 = X = 0 (LOW)
	OUT	(VDP_REGS), A

	;R37 = X = 0 (HIGH)
	OUT	(VDP_REGS), A

	;R38 = DEST Y = 0
	OUT	(VDP_REGS), A

	;R39 = DEST Y = 0
	OUT	(VDP_REGS), A

;	;R40 AND R41 NOT USED
	OUT	(VDP_REGS), A
	OUT	(VDP_REGS), A

	;R42 = NUMBER OF DOTS TO SHIFT
	; HEIGHT - COUNT
	LD	A, HIRES_HEIGHT*2
	SUB	D
	OUT	(VDP_REGS), A
	;R43 DOTS (HIGH)
	XOR	A
	OUT	(VDP_REGS), A

;	;R44 NOT USED
	OUT	(VDP_REGS), A

	; DIRECTION
	LD	A, 0x80	; SHIFT UP
	OUT	(VDP_REGS), A

	; OPERATION
	LD	A, CMD_VRAM_TO_VRAM_Y_ONLY
	OUT	(VDP_REGS), A


	; INVOKE CMD_VDP_TO_VRAM TO CLEAR
	; BOTTOM LINES FROM ( HIRES_HEIGHT*2)-( _fourthNibble*2) FOR HEIGHT OF  _fourthNibble * 2
	CALL	_waitForCommandCompletion

	; SETUP INDIRECT REGISTER ACCESS FROM R#36
	LD	A, 36
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	; R36/37 - DX = 0
	XOR	A
	OUT	(VDP_REGS), A
	OUT	(VDP_REGS), A

	LD	A, HIRES_HEIGHT*2
	; DEC	A
	SUB	D
	; R38/39 - DY = 0
	OUT	(VDP_REGS), A
	XOR	A
	OUT	(VDP_REGS), A

	; R40/41 - DOTS WIDE = HIRES_WIDTH*2
	XOR	A
	OUT	(VDP_REGS), A
	LD	A, 1
	OUT	(VDP_REGS), A

	; R42/43 - DOTS HEIGHT = _fourthNibble*2
	LD	A, E
	OUT	(VDP_REGS), A
	XOR	A
	OUT	(VDP_REGS), A

	;R44 - COLOR - BACKGROUND - 0
	XOR	A
	OUT	(VDP_REGS), A

	;R45 - DIRECTION - RIGHT, DOWN
	OUT	(VDP_REGS), A

	;R46 - CMD
	LD	A, CMD_VDP_TO_VRAM
	OUT	(VDP_REGS), A


	;RESET DEFAULT STATUS REGISTER INDEX
	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A
	EI
	RET
