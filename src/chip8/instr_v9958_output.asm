
	SECTION	CODE
	EXTERN	_v9958DrawX

	PUBLIC	_v9958Draw

	EXTERN	_currentInstruction
	EXTERN	_yy
	EXTERN	_xx
	EXTERN	_fourthNibble
	EXTERN	__color
	EXTERN	_registerI
	EXTERN	_v9958DrawDblPlane
	EXTERN	_v9958DrawPlane

REGISTERS		EQU	$100
PIXEL_WIDTH_MASK	EQU 	255
PIXEL_HEIGHT_MASK	EQU	127

_v9958Draw:
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

; if (_color == 3)
	LD	A, (__color)
	sub	a, 0x03
	jr	NZ, l_v9958DrawX_00126
	ld	a, 0x01
	jr	l_v9958DrawX_00127

l_v9958DrawX_00126:
	xor	a,a

l_v9958DrawX_00127:
	ld	c, a			; C is 1 if color == 3

; const uint8_t originalYY = yy;
	ld	a, (_yy)
	ld	b, a

; v9958DrawDblPlane((byte *)registerI);
	ld	de, (_registerI)

; if (fourthNibble == 0) {
	ld	a, IYL
	AND	$0F
	jr	NZ, l_v9958DrawX_00105

; fourthNibble = 16;
	ld	a, $10
	ld	(_fourthNibble), a

; if (_color == 3)
	ld	a, c
	or	a, a
	jr	Z, l_v9958DrawX_00102

; _color = 1;
	ld	hl, __color
	ld	(hl), $01

; v9958DrawDblPlane((byte *)registerI);
	push	bc
	ex	de, hl		; HL => *registerI
	call	_v9958DrawDblPlane
	pop	bc

; yy = originalYY;
	ld	hl, _yy
	ld	(hl), b

; _color = 2;
	ld	hl, __color
	ld	(hl), 0x02

; v9958DrawDblPlane((byte *)(registerI + 32));
	ld	hl, (_registerI)
	ld	de,0x0020
	add	hl, de
	call	_v9958DrawDblPlane

; _color = 3;
	ld	hl,__color
	ld	(hl),0x03
	RET

l_v9958DrawX_00102:
; v9958DrawDblPlane((byte *)registerI);
	ex	de, hl
	jp	_v9958DrawDblPlane


l_v9958DrawX_00105:
; if (_color == 3) {
	ld	a, c
	or	a, a
	jr	Z, l_v9958DrawX_00107

; _color = 1;
	ld	hl,__color
	ld	(hl),0x01

; v9958DrawPlane((byte *)registerI);
	push	bc
	ex	de, hl
	call	_v9958DrawPlane
	pop	bc

; yy = originalYY;
	ld	hl,_yy
	ld	(hl), b

; _color = 2;
	ld	hl,__color
	ld	(hl), 0x02

; v9958DrawPlane((byte *)(registerI + fourthNibble));
	ld	hl ,_fourthNibble
	ld	c, (hl)
	ld	b, 0x00
	ld	hl, (_registerI)
	add	hl, bc
	call	_v9958DrawPlane

; _color = 3;
	ld	hl, __color
	ld	(hl), 0x03
	ret

l_v9958DrawX_00107:
;chip8/instr_v9958_output.c:52: v9958DrawPlane((byte *)registerI);
	ex	de, hl
	jp	_v9958DrawPlane

else
	ret
endif
