
	EXTERN	_main, __Exit

	SECTION code_crt_init

APPSTART:
	LD	HL, (6)
	LD	SP, HL

	SECTION	code_crt_main
	CALL	_main
__Exit:
	JP	0

PHYSTART:

	SECTION	IGNORE
