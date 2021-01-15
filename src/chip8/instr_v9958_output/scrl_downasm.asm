	PUBLIC	_v9958ScrollDown

	EXTERN	_initDrawParams, _waitForCommandCompletion
	EXTERN	__color, _yy

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
	DI
	LD	A, 0x80 | 14
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A


; READ EACH ROW INTO BUFFER
again:
	LD	B, HIRES_HEIGHT-1 	;LOOP FOR Y = 63 to 1

nextRow:
	LD	HL, LINEDST
	push	bc
	CALL	readFromVdp
	pop	bc


	; READ NEXT LINE
	DEC	B
	LD	HL, LINESRC
	push	bc
	CALL	readFromVdp
	pop	bc

	exx
	call	processLine
	exx

; 	; process line dst from line src

; 	; move linesrc to linedst

	INC	B
	LD	HL, LINEDST
	push	bc
	CALL	writeLineTwiceToDVP
	pop	bc

	DJNZ	nextRow

	call	clearTopLine

done:
	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, $80 | 15
	OUT	(VDP_ADDR), A
	EI
	RET

; params:
; y co-ord - b
; location - hl
readFromVdp:
	XOR	a
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, b
	OUT	(VDP_ADDR), A

	ld	b, 128
loop:
	nop
	in	a, (VDP_DATA)
	LD	(HL), A
	INC	HL
	djnz	loop

	ret



writeLineTwiceToDVP:
	XOR	a
	OUT	(VDP_REGS), A

	ld	d, h
	ld	e, l

	OUT	(VDP_ADDR), A
	ld	a, b
	or	0x40	; WRITE MODE
	OUT	(VDP_ADDR), A

	ld	b, 128
wrLoop1:
	LD	a, (HL)
	INC	HL
	out	(VDP_DATA), a
	djnz	wrLoop1

	ld	b, 128
	ld	l, e
	ld	h, d
wrLoop2:
	LD	a, (HL)
	INC	HL
	out	(VDP_DATA), a
	djnz	wrLoop2

	ret


clearTopLine:
	XOR	a
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, 0x40	; WRITE MODE
	OUT	(VDP_ADDR), A

	ld	b, 0
	xor	a
clLoop1:
	out	(VDP_DATA), a
	djnz	clLoop1

	ret


processLine:
	LD	HL, LINESRC
	LD	DE, LINEDST
	LD	BC, HIRES_WIDTH
	LDIR
	RET

	SECTION	DATA

LINESRC:	DS	HIRES_WIDTH
LINEDST:	DS	HIRES_WIDTH
