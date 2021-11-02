
	EXTERN _debug, _outChar

define(`MMMMMM', `
	LD	A, 1
')


	include "v9958.inc"
	SECTION	CODE

	PUBLIC	_getStack

_getStack:
        LD     HL, 0
        ADD    HL, SP
	RET

_outChar:
	ld	c, 2
	ld	e, l
	CALL 00005h
	RET

_debug:
	ld b,b
	jr $+2
	MMMMMM()
	RET
