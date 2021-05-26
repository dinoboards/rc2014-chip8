
	EXTERN _debug

define(`MMMMMM', `
	LD	A, 1
')


	include "v9958.inc"
	SECTION	CODE
_debug:
	ld b,b
	jr $+2
	MMMMMM()
	RET
