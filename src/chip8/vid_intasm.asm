	PUBLIC _vsyncInstallInterruptHandler, _vsyncRemoveInterruptHandler
	EXTERN _timerTick

	SECTION CODE
; INTERRUPT HANDLING (VSYNC) FOR TIMER TICK

include "hbios_sys.inc"
include	"v9958.inc"

intHandler:
	IN	A, (VDP_ADDR)			; TEST FOR INT FLAG
	AND	$80
	JR	NZ, vsyncInt

	JP	0
nextHandler	EQU	$-2

vsyncInt:
	LD	HL, (_timerTick)
	INC	HL
	LD	(_timerTick), HL
	OR	$FF
	RET

_vsyncInstallInterruptHandler:
	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_SET

	LD	E, 0
	LD	HL, intHandler

	RST	08

	LD	(nextHandler), HL

	POP	IX
	RET

_vsyncRemoveInterruptHandler:
	; DISABLE INTERRUPTS GENERATION FROM VIDEO CHIP
	DI
	IN	A, (VDP_ADDR)
	XOR	A			; /* 16K,Disable Display, Disable Int.,8x8 Sprites,Mag.Off */
	OUT	(VDP_ADDR), A
	LD	A, 0x81 		; SELECT R#1
	OUT	(VDP_ADDR), A
	EI

	PUSH	IX

	LD	BC, BF_SYSINT * 256 + BF_SYSINT_SET

	LD	E, 0
	LD	HL, (nextHandler)

	RST	08

	LD	L, A

	POP	IX
	RET

	SECTION IGNORE
