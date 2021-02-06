; APPLY TRANSFORMATION OF COLOR MASK
; APPLY THE COLOR AS MASKED IN C FROM (DE) TO (HL)

; IF (C & (DE)) // BIT IS ON
;   (HL) = (HL) | ((DE) & C)
; ELSE
;   (HL) = (HL) & ((DE) | ~C)
define(`M_APPLY_COLOR_MASK_TRANSFORM', `

	LD	A, (DE)		; SOURCE
	AND	C
	JR	Z, COLOR_OFF

	OR	(HL)
	LD	(HL), a
	JR	COLOR_ON

COLOR_OFF:
	LD	A, C
	CPL		; A IS ~COLOR
	AND	0x33
	EX	DE, HL
	OR	(HL)	; A = A | SOURCE
	EX	DE, HL
	AND	(HL)	; A = A & DEST
	LD	(HL), A
COLOR_ON:
')


; TASK LOWER 4 BIT COLOUR CODE IN A
; ADD DUPLICATE TO UPPER NIBBLE
; THEN STORE IN COLOR_MASK
define(`M_STORE_COLOR_MASK_FROM_A', `
	LD	C, A
	RRCA
	RRCA
	RRCA
	RRCA
	OR	C
	LD	(COLOR_MASK), A
')

; RESET DEFAULT STATUS REGISTER INDEX
define(`M_RESET_V9958_DEFAULT_REGISTER', `
	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A
')
