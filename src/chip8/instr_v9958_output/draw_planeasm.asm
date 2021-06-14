	PUBLIC	_v9958DrawPlane
	EXTERN	_drawRow
	EXTERN	_fourthNibble, _yy, _yAddOne
	EXTERN	__color

	SECTION CODE

	include	"v9958.inc"

; INPUTS
;	IX	=> registerI
;	L'	=> COLOR
;	_fourthNibble
;	_yAddOn
;
; MUTATES
;	B	=> counter
;	HL	-> _fourthNibble
;	_yy	=> incremented
;	IX	=> registerI incremented
;	E'	=> spriteRowData passed to _drawRow

_v9958DrawPlane:
; for(byte row = fourthNibble; row > 0; row--) {
	ld	hl, _fourthNibble
	ld	b, (hl)
	ld	hl, _yAddOne


l_v9958DrawPlane_00103:
; yAddOne = (yy + 1) & PIXEL_HEIGHT_MASK;
	ld	a, (_yy)
	inc	a
	and	a,0x7f
	ld	(hl), a

; drawRow(*pSpriteData++);
	exx
	ld	e, (IX)
	inc	IX
	call	_drawRow
	exx

; yy = (yAddOne + 1) & PIXEL_HEIGHT_MASK;
	ld	a, (hl)
	inc	a
	and	a,0x7f
	ld	(_yy), a

; for(byte row = fourthNibble; row > 0; row--) {
	djnz	l_v9958DrawPlane_00103
	ret

