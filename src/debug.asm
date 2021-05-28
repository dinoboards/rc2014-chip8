
	EXTERN _debug

	SECTION	CODE
_debug:
	ld b,b
	jr _debug2
_debug2:
	RET
