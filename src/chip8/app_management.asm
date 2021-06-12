
	SECTION CODE

	EXTERN	_printf
	EXTERN	_exit
	EXTERN	_restoreSystemHardware

	PUBLIC	_applicationExit
; extern void applicationExit(const char* message, ...)

_applicationExit:
	CALL	_restoreSystemHardware

	POP	HL		; remove our return address - never returning anyway
	CALL	_printf

	ld	hl, 1
	push	hl
	CALL	_exit
	JP	0		; should never get here
