
	EXTERN	_main, __Exit, l_command_line_parse           ;main() is always external to crt0 code

	SECTION code_crt_init

;	APP will be relocated to himem - therefore stack can be set to start from just below app start

APPSTART:
	LD	SP, APPSTART

	SECTION	code_crt_main

	ld hl,0x0080                ; default dma buffer

	ld c,(hl)
	ld b,h                      ; bc = length of command line

	inc l
	ex de,hl

	call l_command_line_parse

	; cpm does not supply program name in command line
	; so place empty string in argv[0] instead

	; bc = int argc
	; hl = char *argv[]
	; de = & empty string
	; bc'= num chars in redirector
	; hl'= char *redirector

	; push de                     ; empty string added to front of argv[]

	; dec hl
	; dec hl                      ; char *argv[] adjusted to include empty string at index 0

	; inc c                       ; argc++

	;push hl                     ; argv
	push bc                     ; argc

	; inc	sp

	ld	hl, sp
	; inc	hl
	; inc	hl


	CALL	_main
__Exit:
	JP	0


PHYSTART:
	SECTION	IGNORE
