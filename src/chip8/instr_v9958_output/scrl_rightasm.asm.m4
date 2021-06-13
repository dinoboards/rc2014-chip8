	PUBLIC	_v9958ScrollRight

	EXTERN	 _waitForCommandCompletion
	EXTERN	__color
	EXTERN	clearLine, readLineFromVdp, COLOR_MASK, LINESRC, LINEDST

	SECTION CODE

	include	"v9958.inc"
	include(`macros.inc.m4')


_v9958ScrollRight:
	LD	A, (__color)
	CP	3
	JR	Z, scrollDownAllPlanes

	M_STORE_COLOR_MASK_FROM_A()

	LD	D,  HIRES_HEIGHT - 1

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

NXT_ROW:
	LD	HL, LINESRC
	CALL	readLineFromVdp

	EXX				; PROTECT D (LINE INDEX)

	LD	HL, LINESRC + HIRES_WIDTH - 1
	LD	DE, LINESRC + HIRES_WIDTH - 4 - 1
	LD	A, (COLOR_MASK)
	LD	C, A
	LD	B, 124

SFT_RGT_DOTS:
	M_APPLY_COLOR_MASK_TRANSFORM()
	DEC	HL
	DEC	DE
	DJNZ	SFT_RGT_DOTS

	; CLEAR THE LEFT MOST 4 DOTS FOR THE SPECIFIC COLOR
	LD	A, C
	CPL
	AND	$33
	LD	C, A
	LD	B, 4
CLR_LFT:
	LD	A, (HL)
	AND	C
	LD	(HL), A
	DEC	HL
	OUT	(VDP_DATA), A
	DJNZ	CLR_LFT

	EXX				; RESTORE D (LINE INDEX)

	; WRITE OUT THE 2 LINES (SINGLE DOT)
	XOR	A
	LD	HL, LINESRC
	LD	B, 128

	DI
	; SET VDP VRAM ADDRESS TO FIRST LINE START
	OUT	(VDP_REGS), A
	OUT	(VDP_ADDR), A
	LD	A, D
	OR	$40	; WRITE MODE
	OUT	(VDP_ADDR), A

	; WRITE OUT LINE FROM LINESRC
LP_LINE1:
	LD	A, (HL)
	INC	HL
	NOP
	NOP
	NOP
	NOP
	OUT	(VDP_DATA), A
	DJNZ	LP_LINE1

	; NOW WRITE OUT 2ND LINE
	LD	HL, LINESRC
	LD	B, 128
LP_LINE2:
	LD	A, (HL)
	INC	HL
	NOP
	NOP
	NOP
	NOP
	OUT	(VDP_DATA), A
	DJNZ	LP_LINE2

	EI

	DEC	D
	JP	P, NXT_ROW

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

	LD	A, HIRES_WIDTH * 2 - 9
	OUT	(VDP_REGS), A			; R32 = SOURCE X = 247 (LOW)
	XOR	A
	OUT	(VDP_REGS), A			; R33 = HIGH

	OUT	(VDP_REGS), A			; R34 = SOURCE Y = 0 (LOW)
	OUT	(VDP_REGS), A			; R35 = SOURCE Y = 0 (HIGH)

	LD	A, HIRES_WIDTH * 2 - 1
	OUT	(VDP_REGS), A			; R36 = DEST X = 255 (LOW)
	LD	A, 0
	OUT	(VDP_REGS), A			; R37 = DEST X = 255 (HIGH)

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

	ld	a, @00000100
	OUT	(VDP_REGS), A			; R45 = DIRECTION = LEFT DOWN

	; INVOKE CMD_HIGH_SPEED_VRAM_TO_VRAM
	LD	A, CMD_HIGH_SPEED_VRAM_TO_VRAM
	OUT	(VDP_REGS), A			; R46 COMMAND

	CALL	_waitForCommandCompletion

	; SETUP COMMAND TO CLEAR LEFT COLUMN
	; SETUP INDIRECT REGISTER ACCESS FROM R#36
	LD	A, 36
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	XOR	A
	OUT	(VDP_REGS), A			; R36 = DX LOW = 0
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
