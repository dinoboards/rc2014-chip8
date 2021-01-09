	PUBLIC	_initDrawParams, _drawSegment, _drawRow

	EXTERN	_xx, _yy, _xxTo, __fromX, __fromY, __color_, __operation, __toY, __color, _yAddOne
	EXTERN	_spriteRowData, _currentVideoPlane
	EXTERN	_waitForCommandCompletion, __startX, __startY, __operation, _dir

	EXTERN	_longSide, _shortSide, _commandFromR36, _commandFromR32

	SECTION CODE

	include	"v9958.inc"

REGISTERS	EQU	0x100

_initDrawParams:
	; clear all registers
	LD	A, 32
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	B, 15
	XOR	A
loop:
	OUT	(VDP_REGS), A
	djnz	loop
	RET

; TEST FOR XORing of bits
; C: X1
; D: X2
; (_yy): Y OF TOP LINE
_testSegment:
	ld	a, (REGISTERS+15)
	or	a
	RET	NZ

	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_testSegmentLoop0:
	IN	A, (VDP_ADDR)
	RRCA
	JR	C, _testSegmentLoop0

	; SET INDIRECT REGISTER TO 32
	LD	A, 32
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	; R32
	ld	(__startX), a
	OUT	(VDP_REGS), a
	xor	a
	OUT	(VDP_REGS), a

	; r#34
	ld	a, (_yy)
	OUT	(VDP_REGS), a
	xor	a
	OUT	(VDP_REGS), a

	ld	a, (__color)
	ld	e, a
	ld	a, c	; START AT X1

_testSegmentLoop:
	ex	af, af'		; A' is X

	; R32
	ex	af, af'		; A is X
	OUT	(VDP_ADDR), A
	ex	af, af'		; A' is X
	LD	A, 0x80 | 32
	OUT	(VDP_ADDR), A

	; r46
	ld	a, CMD_POINT
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	; WAIT FOR COMMAND TO COMPLETE
_testSegmentLoop2:
	IN	A, (VDP_ADDR)
	RRCA
	JR	C, _testSegmentLoop2

	; READ IN POINT's COLOUR VIA S#7
	LD	A, 7
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A
	IN	A, (VDP_ADDR)

	; call	_commandFromR32		; Assumes does not destry DE or A'
	; A => CURRENT COLOUR CODE
	; H IS NEW COLOUR CODE

	xor	e
	and	e
	cp	e
	jr	nz, _testSegmentHit

	ex	af, af'		; A is X

	inc	a		; X += 1
	cp	d		; X == X2?
	jr	nz, _testSegmentLoop
	RET

_testSegmentHit:
	ld	a, 1
	ld	(REGISTERS+15), a
	RET

; Params: (_xx (c), _yy, _xxTo, _yAddOne)
; DRAW TWO HORIZONTAL LINES WITH SAME START X AND END X CO-ORDINATES
; AUTO WRAPS LINE IF IT GOES BEYOND SCREEN RIGHT
; AS SUCH, ASSUMES X1 IS START AND X2 IS END
; C: X1
; D: X2
; (_yy): Y OF TOP LINE
; (_yAddOne): Y OF BOTTOM LINE

_drawSegment:
; if( xxTo < xx) {

	DI

	xor	a
	;R41= longside high
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 41
	OUT	(VDP_ADDR), A

	ld	a, d
	sub	c
	jr	NC, l_drawSegment_00102

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
	ld	a, (__color)
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
	ld	a, (__color)
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

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A
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
	ld	a, (__color)
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
	ld	a, (_yAddOne)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 38
	OUT	(VDP_ADDR), A

	; r46 = command
	ld	a, CMD_LOGIC_XOR | CMD_LINE
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 46
	OUT	(VDP_ADDR), A

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	EI
	RET


; Params: _xx, _yy, _spriteRowData

; _xx is C
; _spriteRowData is E
; loop counter B
; spriteRowData in L
_drawRow:
; pendingDraw = false;
	xor	a
	ex	af, af'

	; STORE _xx in C
	ld	a,(_xx)
	ld	c, a

; xxTo = xx;
	ld	d, a

	; STORE _spriteRowData in E
	ld	e, l

; for(byte t = 8; t > 0; t--) {
	ld	b,0x08

l_drawRow__00114:
; if (spriteRowData & 0x80) {
	; ld	a,(_spriteRowData)

; if (spriteRowData & 0x80) {
	rlc	e
	jr	NC,l_drawRow__00108

;; PIXEL ON HERE

; if(!pendingDraw) {
	ex	af, af'
	or	a
	jr	NZ,l_drawRow__00102

; pendingDraw = true;
	cpl	a
	ex	af, af'

; xxTo = (xx + 1);
	ld	d, c
	inc	d

	jr	l_drawRow__00109

l_drawRow__00102:
 	ex	af, af'

; xxTo = xxTo + 2;
	inc	d
	inc	d

	jr	l_drawRow__00109

l_drawRow__00108:
; if(pendingDraw) {
 	ex	af, af'
 	or	a
	jr	Z,l_drawRow__00105

; drawSegment();
	ex	af, af'
	push	bc
	push	de
	call	_drawSegment
	pop	de
	pop	bc

; pendingDraw = false;
	xor	a
	ex	af, af'

; xx = xxTo + 3;
	ld	c, d
	inc	c
	inc	c
	inc	c

	jr	l_drawRow__00109

l_drawRow__00105:
	ex	af, af'

; xx += 2;
	ld	a, c
	add	a,0x02
	ld	c, a

l_drawRow__00109:
; for(byte t = 8; t > 0; t--) {
	djnz	l_drawRow__00114

; if(pendingDraw) {
	ex	af, af'
	or	a
	ret	Z

; drawSegment();
	jp	_drawSegment


	PUBLIC _v9959DrawPlane
	EXTERN	_fourthNibble, _yy

_v9959DrawPlane:
	ex	de, hl
; for(byte row = fourthNibble; row > 0; row--) {
	ld	hl, _fourthNibble
	ld	b, (hl)
	ld	hl, _yAddOne

l_v9959DrawPlane_00103:
; yAddOne = (yy + 1) & PIXEL_HEIGHT_MASK;

	ld	a,(_yy)
	inc	a
	and	a,0x7f
	ld	(hl),a

; drawRow(*pSpriteData++);
	ld	a, (de)
	inc	de
	exx
	ld	l, a
	call	_drawRow
	exx

; yy = (yAddOne + 1) & PIXEL_HEIGHT_MASK;
	ld	a,(hl)
	inc	a
	and	a,0x7f
	ld	(_yy), a

; for(byte row = fourthNibble; row > 0; row--) {
	djnz	l_v9959DrawPlane_00103

	ret

	SECTION	DATA


	SECTION IGNORE
