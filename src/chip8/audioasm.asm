
	PUBLIC	_audioChipProbe, _audioCmd


PSG_REGS:	EQU	$A0             ; PSG register write port
PSG_DATA:	EQU	$A1             ; PSG value write port
PSG_STAT:	EQU	$A2             ; PSG value read port

PSG_PD_RG:	EQU	$00		; PERIOD REGISTER (A TO C)
PSG_LV_RG:	EQU	$08		; LEVEL REGISTERS (A TO C)

	SECTION	CODE

	; RETURNS 0 IF NOT FOUND
	; -1 IF FOUND AY
_audioChipProbe:
	LD	A, 3			; R3 - 2ND CHANNEL HIGH BYTE
	DI
	OUT	(PSG_REGS), A
	LD	A, $55
	OUT	(PSG_DATA), A
	IN	A,(PSG_STAT)
	EI
	CP	$55			; REGISTER
	JR	Z, found
;
	LD	L, 0
	RET

found:					; RESET THE CHIP
	XOR	A
	LD	C, PSG_REGS
	LD	B, 10

clearAllRegisters:
	OUT	(C), B
	OUT	(PSG_DATA), A
	DEC	B
	JP	P, clearAllRegisters

	LD	A, 7
	OUT	(PSG_REGS), A
	LD	A, @00111000		; A AND B AS INPUT NOISE OFF
	OUT	(PSG_DATA), A

	LD	L, 1
	RET

; typedef struct _audioCommand {
;   uint8_t channel;  //2 bits
;   uint8_t volume;   //5 bits
;   uint16_t period;  //12 bits
; } audioCommand;

; extern void audioCmd(const audioCommand*) __z88dk_fastcall;
_audioCmd:
	LD	A, (HL)			; LOAD CHANNEL
	INC	HL
	LD	D, A

	ADD	PSG_LV_RG
	OUT	(PSG_REGS), A		; SELECT VOLUME REGISTER FOR CHANNEL

	LD	A, (HL)			; LOAD VOLUME
	INC	HL
	OUT	(PSG_DATA), A		; SET VOLUME

	LD	A, D
	ADD	A			; CHANNEL * 2 - REGISTER PERIOD LOW
	LD	D, A
	OUT	(PSG_REGS), A

	LD	A, (HL)			; LOAD PERIOD LOW VALUE
	INC	HL
	OUT	(PSG_DATA), A		; SET CHANNEL'S LOW VALUE

	LD	A, D
	INC	A
	OUT	(PSG_REGS), A		; SELECT REGISTER PERIOD HIGH

	LD	A, (HL)			; LOAD PERIOD HIGH VALUE
	INC	HL
	OUT	(PSG_DATA), A		; SET CHANNEL'S HIGH VALUE

	RET
