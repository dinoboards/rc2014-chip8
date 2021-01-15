	PUBLIC	_v9958ScrollDown

	EXTERN	_initDrawParams, _waitForCommandCompletion
	EXTERN	__color, _yy, _fourthNibble, __color

	SECTION CODE

	include	"v9958.inc"
HIRES_WIDTH		EQU	128
HIRES_WIDTH_MASK	EQU	255
HIRES_HEIGHT		EQU	64
HIRES_HEIGHT_MASK	EQU	127

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

_v9958ScrollDown:
	LD	A, (__color)
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
	LD	C, A
	LD	A, HIRES_HEIGHT-1
	SUB	C			;LOOP FOR Y = 63 - COUNT to 0
	LD	D, A

nextRow:
	LD	HL, LINESRC
	CALL	readFromVdp

	LD	A, D		; CALCULATE THE DEST LINE INDEX
	ADD	C
	LD	D, A
	CALL	readFromVdp

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
	SUB	C
	LD	D, A

	DEC	D
	JP	P, nextRow

	LD	A, (_fourthNibble)
	DEC	A
	LD	D, A

	LD	A, (COLOR_MASK)
	CPL
	AND	$33
	LD	C, A
clearNextRow:
	LD	HL, LINESRC
	CALL	readFromVdp
	CALL	clearTopLine
	DEC	D
	JP	P, clearNextRow
	RET

; params:
; y co-ord - b
; location - hl
readFromVdp:
	XOR	a
	DI
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, d
	OUT	(VDP_ADDR), A

	ld	b, 128
loop:
	nop
	nop
	nop
	nop
	in	a, (VDP_DATA)
	LD	(HL), A
	INC	HL
	djnz	loop

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, $80 | 15
	OUT	(VDP_ADDR), A
	EI
	ret

; X = 0
; width = 256
; Y = 0
; height = count (C)

clearTopLine:

	LD	HL, LINESRC
	LD	B, 128

	XOR	A
	DI
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, d
	or	0x40	; WRITE MODE
	OUT	(VDP_ADDR), A

clLoop1:
	LD	A, (HL)
	AND	C
	INC	HL

	; or	0xF0
	out	(VDP_DATA), a
	djnz	clLoop1

	ld	b, 128
	LD	HL, LINESRC
clLoop2:
	LD	A, (HL)
	AND	C
	INC	HL

	; or	0xF0
	out	(VDP_DATA), a
	djnz	clLoop2


	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, $80 | 15
	OUT	(VDP_ADDR), A
	EI

	RET


	; for each byte:
	; if (c & s) // bit is on
	;   d = d or (s and c)
	; else
	;   d = d and (s or ~c)
	; send byte d

processPixels:
	LD	A, (DE)		; SOURCE
	AND	C
	JR	Z, plColorOff

	OR	(HL)
	LD	(HL), a
	RET

plColorOff:
	LD	A, C
	CPL		; a is ~color
	and	0x33
	EX	DE, HL
	OR	(HL)	; a = a | source
	EX	DE, HL
	AND	(HL)	; a = a & dest
	LD	(HL),A
	RET


	SECTION	DATA

COLOR_MASK:	DB	0

LINESRC:	DS	HIRES_WIDTH
LINEDST:	DS	HIRES_WIDTH
