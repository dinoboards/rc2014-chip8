
	EXTERN _debug, _outChar

define(`MMMMMM', `
	LD	A, 1
')


	include "v9958.inc"
	SECTION	CODE



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
