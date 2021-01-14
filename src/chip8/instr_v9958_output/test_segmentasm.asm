	PUBLIC	_testSegment

	EXTERN	__color, _yy

	SECTION CODE

	include	"v9958.inc"

REGISTERS	EQU	0x100


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

	; r#34
	LD	A, (_yy)
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 34
	OUT	(VDP_ADDR), A

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
