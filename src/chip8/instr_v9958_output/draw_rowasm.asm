	PUBLIC	_drawRow

	EXTERN	_drawSegment
	EXTERN	_xx
	EXTERN	__color

	SECTION CODE

	include	"v9958.inc"

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

	ld	a, (__color)
	ld	l, a			; PRELOAD __color INTO L FOR _drawSegment and _testSegment

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
