
	SECTION	CODE
	EXTERN	_v9958DrawX

	PUBLIC	_v9958DrawPlane3, _v9958DrawSinglePlane

	EXTERN	_currentInstruction
	EXTERN	_yy
	EXTERN	_xx
	EXTERN	_fourthNibble
	EXTERN	__color
	EXTERN	_registerI
	EXTERN	_v9958DrawPlane
	EXTERN	_yAddOne
	EXTERN	_drawRow

REGISTERS		EQU	$100
PIXEL_WIDTH_MASK	EQU 	255
PIXEL_HEIGHT_MASK	EQU	127

_v9958DrawPlane3:
ifndef CPM

	LD	HL, REGISTERS + $0F
	LD	(HL), $00

	LD	A, IYH
	AND	$0F
	LD	L, A
	LD	A, (HL)
	ADD	A
	LD	(_xx), A

; yy = (registers[nibble3rd] * 2) & PIXEL_HEIGHT_MASK;
	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F
	LD	L, A
	LD	A, (HL)
	ADD	A
	AND	PIXEL_HEIGHT_MASK
	LD	(_yy), A

; const uint8_t originalYY = yy;
	ld	a, (_yy)
	ld	b, a

; v9958DrawDblPlane((byte *)registerI);
	ld	ix, (_registerI)

; if (fourthNibble == 0) {
	ld	a, IYL
	AND	$0F
	jr	NZ, l_v9958DrawXPlane3_00105

; fourthNibble = 16;
	ld	a, $10
	ld	(_fourthNibble), a

; _color = 1;
	exx
	ld	l, 1
	exx

; v9958DrawDblPlane((byte *)registerI);
	push	bc
	call	_v9958DrawDblPlane
	pop	bc

; yy = originalYY;
	ld	hl, _yy
	ld	(hl), b

; _color = 2;
	exx
	ld	l, 2
	exx

; v9958DrawDblPlane((byte *)(registerI + 32));
	JP	_v9958DrawDblPlane

l_v9958DrawXPlane3_00105:
; _color = 1;
	exx
	ld	L, 1
	exx

; v9958DrawPlane((byte *)registerI);
	push	bc
	call	_v9958DrawPlane
	pop	bc

; yy = originalYY;
	ld	hl, _yy
	ld	(hl), b

; _color = 2;
	exx
	ld	l, 2
	exx

; v9958DrawPlane((byte *)(registerI + fourthNibble));
	JP	_v9958DrawPlane

else
	ret
endif


_v9958DrawSinglePlane:
ifndef CPM
	LD	HL, REGISTERS + $0F
	LD	(HL), $00

	LD	A, IYH
	AND	$0F
	LD	L, A
	LD	A, (HL)
	ADD	A
	LD	(_xx), A

; yy = (registers[nibble3rd] * 2) & PIXEL_HEIGHT_MASK;
	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F
	LD	L, A
	LD	A, (HL)
	ADD	A
	AND	PIXEL_HEIGHT_MASK
	LD	(_yy), A

	EXX
	LD	A, (__color)
	ld	L, A
	EXX

; const uint8_t originalYY = yy;
	ld	a, (_yy)
	ld	b, a

; v9958DrawDblPlane((byte *)registerI);
	ld	ix, (_registerI)

; if (fourthNibble == 0) {
	ld	a, IYL
	AND	$0F
	JP	NZ, _v9958DrawPlane

; fourthNibble = 16;
	ld	a, $10
	ld	(_fourthNibble), a

	jp	_v9958DrawDblPlane

else
	ret
endif


; INPUTS:
;	IX	=> registerI
;	L'	=> __color
;	_yAddOne
;	_xx
;	_yy
;
; MUTATES
;	IX	=> registerI (incremented)
;	B	=> counter
;	E	-> sprite row *registerI

_v9958DrawDblPlane:
ifndef CPM
	ld	hl, _xx

	ld	b, 0x10			; LOOP COUNTER FOR 16 ROWS

l_v9958DrawDblPlane_00103:
; yAddOne = (yy + 1) & PIXEL_HEIGHT_MASK;
	ld	a, (_yy)
	inc	a
	and	PIXEL_HEIGHT_MASK
	ld	(_yAddOne), a

; drawRow(*pSpriteData++);
	exx
	ld	e, (ix)
	inc	ix
	call	_drawRow
	exx

; xx += 16;
	ld	a, (hl)
	add	a, 0x10
	ld	(hl), a

; drawRow(*pSpriteData++);
	exx
	ld	e, (ix)
	inc	ix
	call	_drawRow
	exx

; xx -= 16;
	ld	a,(hl)
	add	a,0xf0
	ld	(hl), a

; yy = (yAddOne + 1) & PIXEL_HEIGHT_MASK;
	ld	a,(_yAddOne + 0)
	inc	a
	and	a,0x7f
	ld	(_yy), a

	djnz	l_v9958DrawDblPlane_00103
endif
	ret
