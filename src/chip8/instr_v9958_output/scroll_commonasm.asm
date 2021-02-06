
	PUBLIC	clearLine, readLineFromVdp
	PUBLIC	COLOR_MASK, LINESRC, LINEDST

	SECTION	CODE

	include	"v9958.inc"

; params:
; y co-ord - d
; location - hl
readLineFromVdp:
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

	EI
	ret


; X = 0
; width = 256
; Y = 0
; height = count
; C = color mask :
	; LD	A, (COLOR_MASK)
	; CPL
	; AND	$33
	; LD	C, A

clearLine:

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


	SECTION	DATA

COLOR_MASK:	DB	0

LINESRC:	DS	HIRES_WIDTH
LINEDST:	DS	HIRES_WIDTH
