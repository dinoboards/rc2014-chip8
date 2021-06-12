	PUBLIC	_v9958DrawPlane
	EXTERN	_drawRow
	EXTERN	_fourthNibble, _yy, _yAddOne

	SECTION CODE

	include	"v9958.inc"


_v9958DrawPlane:
	exx
	push	hl
	exx

	ex	de, hl
; for(byte row = fourthNibble; row > 0; row--) {
	ld	hl, _fourthNibble
	ld	b, (hl)
	ld	hl, _yAddOne

l_v9958DrawPlane_00103:
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
	djnz	l_v9958DrawPlane_00103

	exx
	pop	hl
	exx
	ret

