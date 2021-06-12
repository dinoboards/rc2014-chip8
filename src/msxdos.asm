
CALSLT:	EQU	0001CH
INITXT:	EQU	0006CH
EXPTBL:	EQU	0FCC1H
RSTPLT:	EQU	00145H
INIPLT:	EQU	00141H
NMI:	EQU	00066H
EXTROM:	EQU	0015FH
H_NMI:	EQU	0FDD6H

; Function: initialises the screen to TEXT1 mode (40 x 24). In this routine, the palette is not initialised. To initialise the palette, call INIPLT in SUB-ROM after this call.
; Input:
; TXTNAM (F3B3H) for the pattern name table
; TXTCGP (F3B7H) for the pattern generator table
; LINL40 (F3AEH) for the length of one line
; Output: none
; Registers: all

; void msxbiosInitxt();
	PUBLIC	_msxbiosInitxt
_msxbiosInitxt:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INITXT
	CALL	CALSLT
	POP	IX
	RET


; void msxbiosRestorePalette
	PUBLIC	_msxbiosRestorePalette
_msxbiosRestorePalette:
	PUSH	IX
	LD	IX, RSTPLT
	CALL	CALSUB
	POP	IX
	RET

; void msxbiosInitPalette
	PUBLIC	_msxbiosInitPalette
_msxbiosInitPalette:
	PUSH	IX
	LD	IX, INIPLT
	CALL	CALSUB
	POP	IX
	RET


; CALSUB
;
; In: IX = address of routine in MSX2 SUBROM
;     AF, HL, DE, BC = parameters for the routine
;
; Out: AF, HL, DE, BC = depending on the routine
;
; Changes: IX, IY, AF', BC', DE', HL'
;
; Call MSX2 subrom from MSXDOS. Should work with all versions of MSXDOS.
;
; Notice: NMI hook will be changed. This should pose no problem as NMI is
; not supported on the MSX at all.
;
CALSUB:  EXX
         EX     AF, AF'       ; STORE ALL REGISTERS
         LD     HL, EXTROM
         PUSH   HL
         LD     HL, 0C300H
         PUSH   HL           ; PUSH NOP ; JP EXTROM
         PUSH   IX
         LD     HL, 021DDH
         PUSH   HL           ; PUSH LD IX,<ENTRY>
         LD     HL, 03333H
         PUSH   HL           ; PUSH INC SP; INC SP
         LD     HL, 0
         ADD    HL,SP        ; HL = OFFSET OF ROUTINE
         LD     A, 0C3H
         LD     (H_NMI), A
         LD     (H_NMI+1), HL ; JP <ROUTINE> IN NMI HOOK
         EX     AF, AF'
         EXX                 ; RESTORE ALL REGISTERS
         LD     IX, NMI
         LD     IY, (EXPTBL-1)
         CALL   CALSLT      ; CALL NMI-HOOK VIA NMI ENTRY IN ROMBIOS
                             ; NMI-HOOK WILL CALL SUBROM
         EXX
         EX     AF, AF'       ; STORE ALL RETURNED REGISTERS
         LD     HL, 10
         ADD    HL, SP
         LD     SP, HL        ; REMOVE ROUTINE FROM STACK
         EX     AF, AF'
         EXX                 ; RESTORE ALL RETURNED REGISTERS
         RET
