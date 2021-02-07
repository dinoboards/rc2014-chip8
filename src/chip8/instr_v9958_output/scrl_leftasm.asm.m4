	PUBLIC	_v9958ScrollLeft

	EXTERN	 _waitForCommandCompletion
	EXTERN	__color
	EXTERN	clearLine, readLineFromVdp, COLOR_MASK, LINESRC, LINEDST

	SECTION CODE

	include	"v9958.inc"
	include(`macros.inc.m4')


_v9958ScrollLeft:
	LD	A, (__color)
	CP	3
	JR	Z, scrollDownAllPlanes

	M_STORE_COLOR_MASK_FROM_A()

	ld	d, HIRES_HEIGHT - 1

	; for each line
	;  readLineFromVdp to LINESRC
	;  set HL to LINEDST
	;  set DE to LINESRC+4
	;  for b = 124 to 0
	;    _APPLY_COLOR_MASK_TRANSFORM
	;    out byte
	;  for b = 4 to 0
	;    clearPixels
	;  repeat line write - with persisted processed pixels in LINEDST

	DI
	; SETUP INDIRECT REGISTER ACCESS FOR R#14
	; NON-INCREMENTING
	LD	A, 0x80 | 14
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A
	EI

nextRow:
	LD	HL, LINESRC
	CALL	readLineFromVdp

	XOR	a

	DI
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	ld	a, D
	or	0x40	; WRITE MODE
	OUT	(VDP_ADDR), A

	exx

	LD	HL, LINESRC
	LD	DE, LINESRC + 4

	LD	A, (COLOR_MASK)
	LD	C, A

	ld	b, 124

wrLoop1:
	M_APPLY_COLOR_MASK_TRANSFORM()

	INC	HL
	INC	DE
	out	(VDP_DATA), a
	djnz	wrLoop1


	LD	A, C
	CPL
	AND	$33
	LD	C, A

	ld	b, 4

clearRightColumn:
	ld	a, (hl)
	and	c
	ld	(hl), a
	inc	hl
	out	(VDP_DATA), a
	djnz	clearRightColumn

	; repeat for 2nd line
	LD	HL, LINESRC
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

	EI

	exx
	DEC	D
	JP	P, nextRow

	RET



scrollDownAllPlanes:
	DI
	CALL	_waitForCommandCompletion

	; SETUP COMMAND TO SHIFT LEFT 4 DOTS

	; SETUP INDIRECT REGISTER ACCESS FROM R#32
	LD	A, 32
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	A, 8				; R32 = SOURCE X = 8 (4 CHIP8 DOTS) (LOW)
	OUT	(VDP_REGS), A
	XOR	A				; R33 = HIGH
	OUT	(VDP_REGS), A

	OUT	(VDP_REGS), A			; R34 = SOURCE Y = 0 (LOW)
	OUT	(VDP_REGS), A			; R35 = SOURCE Y = 0 (HIGH)

	OUT	(VDP_REGS), A			; R36 = DEST X = 0 (LOW)
	OUT	(VDP_REGS), A			; R37 = DEST X = 0 (HIGH)

	OUT	(VDP_REGS), A			; R38 = DEST Y = 0 (LOW)
	OUT	(VDP_REGS), A			; R39 = DEST Y = 0 (HIGH)

	LD	A, HIRES_WIDTH * 2 - 8		; R40 = NX LOW = 256 - 8
	OUT	(VDP_REGS), A
	XOR	A				; R41 = NX HIGH = 0
	OUT	(VDP_REGS), A

	LD	A, HIRES_HEIGHT * 2 		; R42 = NY LOW = 128
	OUT	(VDP_REGS), A
	XOR	A				; R43 = NY HIGH = 0
	OUT	(VDP_REGS), A

	OUT	(VDP_REGS), A			; R44 NOT USED

	ld	a, @00000000
	OUT	(VDP_REGS), A			; R45 = DIRECTION = RIGHT DOWN

	LD	A, CMD_HIGH_SPEED_VRAM_TO_VRAM
	OUT	(VDP_REGS), A			; R46 COMMAND


	; INVOKE CMD_VDP_TO_VRAM TO CLEAR
	; BOTTOM LINES FROM ( HIRES_HEIGHT*2)-( _fourthNibble*2) FOR HEIGHT OF  _fourthNibble * 2
	CALL	_waitForCommandCompletion

	; SETUP INDIRECT REGISTER ACCESS FROM R#36
	LD	A, 36
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	A, HIRES_WIDTH * 2 - 8
	OUT	(VDP_REGS), A			; R36 = DX LOW = 256 - 8
	XOR	A
	OUT	(VDP_REGS), A			; R37 = DX HIGH = 0

	OUT	(VDP_REGS), A			; R38 = DY LOW = 0
	OUT	(VDP_REGS), A			; R39 = DY HIGH = 0

	LD	A, 8
	OUT	(VDP_REGS), A			; R40 = NX LOW = 8
	XOR	A
	OUT	(VDP_REGS), A			; R41 = NX HIGH = 0

	LD	A, HIRES_HEIGHT * 2
	OUT	(VDP_REGS), A			; R42 = NY LOW = 128
	XOR	A
	OUT	(VDP_REGS), A			; R43 = NY HIGH = 0

	OUT	(VDP_REGS), A			; R44 - COLOR - BACKGROUND - 0

	OUT	(VDP_REGS), A			; R45 - DIRECTION - RIGHT, DOWN

	LD	A, CMD_VDP_TO_VRAM | CMD_LOGIC_IMP
	OUT	(VDP_REGS), A			; R46 COMMAND

	M_RESET_V9958_DEFAULT_REGISTER()
	EI
	RET
