
	EXTERN	_main, __Exit           ;main() is always external to crt0 code

	SECTION code_crt_init

;	APP will be relocated to himem - therefore stack can be set to start from just below app start

APPSTART:
	LD	SP, APPSTART

	SECTION	code_crt_main
	CALL	_main
__Exit:
	JP	0

PHYSTART:
	SECTION	IGNORE
