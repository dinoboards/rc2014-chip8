
	PUBLIC	_hbVdaDev

	SECTION CODE

; typedef struct  {
;   uint8_t unitId;
;   uint8_t type;
;   uint8_t number;
;   uint8_t mode;
;   uint8_t baseIO;
; } hbiosVdaDev;
	;extern byte hbVdaDev(hbiosVdaDev* result) __z88dk_fastcall;
_hbVdaDev:
	PUSH	IX
	LD	B, $43
	LD	C, (HL)
	INC	HL
	PUSH	HL
	RST	08

	POP	BC

	PUSH	AF
	LD	A, D
	LD	(BC), A
	INC	BC
	LD	A, E
	LD	(BC), A
	INC	BC
	LD	A, H
	LD	(BC), A
	INC	BC
	LD	A, L
	LD	(BC), A
	POP	AF

	LD	L, A
	POP	IX
	RET

	SECTION IGNORE
