	PUBLIC	_drawSegment

	EXTERN	_waitForCommandCompletion, _testSegment, _initDrawParams
	EXTERN	__color, _yy, _yAddOne

	SECTION CODE

	include	"v9958.inc"
	include(`macros.inc.m4')

; Params: (_xx (c), _yy, _xxTo, _yAddOne)
; DRAW TWO HORIZONTAL LINES WITH SAME START X AND END X CO-ORDINATES
; AUTO WRAPS LINE IF IT GOES BEYOND SCREEN RIGHT
; AS SUCH, ASSUMES X1 IS START AND X2 IS END
; C: X1
; D: X2
; (_yy): Y OF TOP LINE
; (_yAddOne): Y OF BOTTOM LINE
; L => __color (passed to _testSegment also)

_drawSegment:
; if( xxTo < xx) {


	xor	a

	DI
	;R41= longside high
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 41
	OUT	(VDP_ADDR), A

	ld	a, d
	sub	c
	jP	NC, l_drawSegment_00102

;   drawLine(xx, yy, PIXEL_WIDTH_MASK, yy, 15, CMD_LOGIC_XOR);
;    DRAW TOP LINE TO SCREEN RIGHT

	call	_waitForCommandCompletion

	;R36 = x
	ld	a, c
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A

	;R40 = longside
	xor	a
	sub	c
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 40
	OUT	(VDP_ADDR), A

	push	de
	ld	d, 255
	call	_testSegment
	pop	de

	; reset color - testSegment causes it to be zeroed
	ld	a, l ;(__color)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 44
	OUT	(VDP_ADDR), A

	;R38 = y
	ld	a, (_yy)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

;   drawLine(0, yy, xxTo, yy, 15, CMD_LOGIC_XOR);
;	DRAW TOP LINE FROM SCREEN LEFT

	call	_waitForCommandCompletion

	;R40 = longside
	ld	a, d
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 40
	OUT	(VDP_ADDR), A

	push	bc
	ld	c, 0
	call	_testSegment
	pop	bc

	; reset color - testSegment causes it to be zeroed
	ld	a, l; (__color)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 44
	OUT	(VDP_ADDR), A

	;R36 = x
	xor	a
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A

	;R38 = y
	ld	a, (_yy)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

;   drawLine(xx, yAddOne, PIXEL_WIDTH_MASK, yAddOne, 15, CMD_LOGIC_XOR);
;	DRAW BOTTOM LINE TO SCREEN RIGHT
	; pop	bc
	call	_waitForCommandCompletion

	;R36 = x
	ld	a, c
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A

	;R40 = longside
	xor	a
	sub	c
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 40
	OUT	(VDP_ADDR), A

	;R38 = y
	ld	a, (_yAddOne)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A


;   drawLine(0, yAddOne, xxTo, yAddOne, 15, CMD_LOGIC_XOR);
;	DRAW BOTTOM LINE FROM SCREEN LEFT

	call	_waitForCommandCompletion

	;R40 = longside
	ld	a, d
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 40
	OUT	(VDP_ADDR), A

	;R36 = x
	xor	a
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A

	;R38 = y
	ld	a, (_yAddOne)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

	M_RESET_V9958_DEFAULT_REGISTER()
	EI
	RET

l_drawSegment_00102:
;   drawLine(xx, yy, xxTo, yy, 15, CMD_LOGIC_XOR);
;	DRAW TOP LINE
	ex	af, af'					; SAVE LONG SIDE
	call	_waitForCommandCompletion
	EX	AF, AF'					; RESTORE LONG SIDE

	; R40 = longside
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 40
	OUT	(VDP_ADDR), A

; 	;R36 = x
	ld	a, c			; (_xx)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A

	call	_testSegment

	; reset color - testSegment causes it to be zeroed
	ld	a, l ;(__color)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 44
	OUT	(VDP_ADDR), A

	;R38 = y
	ld	a, (_yy)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

	call	_waitForCommandCompletion

;    drawLine(xx, yAddOne, xxTo, yAddOne, 15, CMD_LOGIC_XOR);
;	DROP BOTTOM LINE

ifdef(`EMULATOR_WORKAROUNDS', `
; 	;R36 = x
	ld	a, c			; (_xx)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 36
	OUT	(VDP_ADDR), A
')

	ld	a, (_yAddOne)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

	M_RESET_V9958_DEFAULT_REGISTER()

	EI
	RET
