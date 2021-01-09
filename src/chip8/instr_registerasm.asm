
	SECTION	CODE

	PUBLIC _ldIVxVy, _ldVxVyI
	EXTERN	_registerI, _currentInstruction

REGISTERS	EQU	$100

_ldIVxVy:
	; 2nd nibble?
	LD	A, (_currentInstruction)
	AND	A, $0F
	LD	L, A
;
	; 3rd nibble
	LD	A, (_currentInstruction+1)
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F

	CP	L
	JR	NC, ldIVxVyNormal

	LD	H, A
	LD	A, L
	SUB	H
	LD	B, A

	INC	B

	LD	DE, (_registerI)
	LD	H, REGISTERS / 256

ldIVxVyLoop:
	LD	A, (HL)
	LD	(DE), A
	DEC	HL
	INC	DE
	DJNZ	B, ldIVxVyLoop

	RET

ldIVxVyNormal:
	LD	H, A
	SUB	L

	LD	C, A
	INC	C

	LD	B, 0
	LD	DE, (_registerI)
	LD	H, REGISTERS / 256

	LDIR
	RET

_ldVxVyI:
	; 2nd nibble?
	LD	A, (_currentInstruction)
	AND	A, $0F
	LD	E, A
;
	; 3rd nibble
	LD	A, (_currentInstruction+1)
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F

	CP	E
	JR	NC, ldVxVyINormal

	LD	H, A
	LD	A, E
	SUB	H
	LD	B, A

	INC	B

	LD	HL, (_registerI)
	LD	D, REGISTERS / 256

ldVxVyILoop:
	LD	A, (HL)
	LD	(DE), A
	INC	HL
	DEC	DE
	DJNZ	B, ldVxVyILoop

	RET

ldVxVyINormal:

	LD	H, A
	SUB	E

	LD	C, A
	INC	C

	LD	B, 0
	LD	HL, (_registerI)
	LD	D, REGISTERS / 256

	LDIR
	RET

	SECTION	IGNORE
