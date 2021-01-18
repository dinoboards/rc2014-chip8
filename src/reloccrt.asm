
	EXTERN	_main, __Exit, l_command_line_parse           ;main() is always external to crt0 code
	EXTERN	_himem

	SECTION code_crt_init

;	APP will be relocated to himem - therefore stack can be set to start from just below app start

APPSTART:
_himem:
	LD	SP, APPSTART

	SECTION	code_crt_main

	ld hl,0x0080                ; default dma buffer

	ld c,(hl)
	ld b,h                      ; bc = length of command line

	inc l
	ex de,hl

	call l_command_line_parse

	push bc                     ; argc
	ld	hl, sp

	CALL	_main
__Exit:
	JP	0


PHYSTART:
	SECTION	IGNORE
