
	SECTION CODE

; Extracted from z88dk library to avoid use of undocumented instruction
; not currently support in the cpm emulator

	PUBLIC	_xstrtol
	EXTERN	error_erange_lmc, error_erange_zc, error_einval_zc
	EXTERN	l_valid_base, l_eat_ws, l_eat_sign, l_eat_digits, error_lzc
	EXTERN	l_neg_dehl, l_char2num, l_mulu_40_32x8, l_eat_base_prefix

; extern long xstrtol(const char *str, char **endptr, int base);

_xstrtol:
	pop	af
	pop	hl
	pop	de
	pop	bc

	push	bc	; int base
	push	de	; endptr
	push	hl	; str
	push	af	; ret addr

	; enter : bc = int base
	;         de = char **endp
	;         hl = char *nptr
	;
	; exit  : *endp = bc as detailed below
	;
	;         no error:
	;
	;           carry reset
	;           dehl = long result
	;             bc = char *nptr (& next unconsumed char)
	;
	;         invalid input string or base:
	;
	;           carry set
	;             bc = initial char *nptr
	;           dehl = 0
	;           errno set to EINVAL
	;
	;         overflow:
	;
	;           carry set
	;             bc = char *nptr (& next unconsumed char following oversized number)
	;           dehl = $7fffffff (LONG_MAX) or $80000000 (LONG_MIN)
	;           errno set to ERANGE
	;
	; uses  : af, bc, de, hl, ix

	call 	__strtoul__
	jr 	c, check_errors

	; unsigned conversion was successful but signed
	; number range is narrower so must check again

	dec a
	ret z                       ; negative result is in range

	bit 7,d                     ; if most significant bit of positive number
	ret z                       ; is set we are out of range

positive_overflow:
	jp error_erange_lmc         ; dehl = $7fffffff = LONG_MAX

check_errors:
	; what kind of error was it

	dec a
	jp m, error_einval_zc       ; on invalid base or invalid string

	; overflow occurred

	dec a
	jr z, positive_overflow

	ld de,$8000
	jp error_erange_zc          ; dehl = $80000000 = LONG_MIN

__strtoul__:
	; strtol, strtoul helper
	;
	; enter : bc = base
	;         de = char **endp
	;         hl = char *
	;
	; exit  : carry reset indicates no error
	;
	;              a = 0 (number not negated) or 1 (number negated)
	;           dehl = result
	;             bc = char * (& next unconsumed char in string)
	;
	;         carry set indicates error, a holds error code
	;
	;              a = 0/-1 for invalid string, -2 for invalid base
	;           dehl = 0
	;             bc = original char *
	;
	;              a = 3 indicates negate on unsigned overflow
	;             bc = char * (& next unconsumed char following number)
	;
	;              a = 2 indicates unsigned overflow
	;             bc = char * (& next unconsumed char following number)
	;
	;              a = 1 indicates negative underflow
	;             bc = char * (& next unconsumed char following number)
	;
	; uses  : af, bc, de, hl, ix

	push	ix
    	dec 	sp	; reserve 2 bytes on the stack
	ld 	ix, 0
	add 	ix, sp

   	call	entry

	inc	sp
   	pop	ix
   	ret

entry:

	ld	a,d
	or	e
	jr	z, no_endp

	; have char **endp

	push	de                     ; save char **endp
	call	no_endp

	; strtoul() done, now must write endp

	;   bc = char * (first uninterpretted char)
	; dehl = result
	;    a = error code (if carry set)
	; carry set = overflow or error
	; stack = char **endp

	ex	(sp),hl
	ld	(hl),c
	inc	hl
	ld	(hl),b

	pop	hl
	ret

no_endp:
	call	l_valid_base

	ld 	d,a                      ; d = base
	ld 	c,l
	ld 	b,h                      ; bc = original char *

	jr 	z, valid_base            ; accept base == 0
	jr 	nc, invalid_base

	valid_base:
	;  bc = original char *
	;  hl = char *
	;   d = base

	call	l_eat_ws               ; skip whitespace
	call	l_eat_sign             ; carry set if negative
	jr	nc, positive

	; negative sign found

	call	positive

	; return here to negate result

	;   bc = char * (first uninterpretted char)
	; dehl = result
	;    a = error code (if carry set)
	; carry set = overflow or error

	inc	a                       ; indicate negate applied
	ret	c                       ; return if error

	; successful conversion, check for signed overflow

	ld	a,d
	add	a,a                     ; carry set if signed overflow

	call	l_neg_dehl             ; negate, carry unaffected

	ld	a,1
	ret

positive:

   ;  bc = original char*
   ;  hl = char *
   ;   d = base

   ld a,d                      ; a = base
   call l_eat_base_prefix
   ld d,a                      ; d = base

   ; there must be at least one valid digit

   ld a,(hl)
   call l_char2num
   jr c, invalid_input

   cp d
   jr nc, invalid_input

   ; there is special code for base 2, 8, 10, 16

   ld e,a
   ld a,d

   cp 10
   jr z, decimal

   cp 16
   jr z, hex

   cp 8
   jr z, octal

   cp 2
   jr z, binary

   ; use generic algorithm

   ;  hl = char *
   ;   d = base
   ;   e = first numerical digit

   ld (ix+0),d

   ld c,l
   ld b,h
   inc bc                      ; bc = & next char to consume

   ld d,0
   ld l,d
   ld h,d
   ex de,hl                    ; dehl = initial digit

loop:

   ; dehl = result
   ;   bc = char *
   ;  ixl = base

   ; get next digit

   ld a,(bc)
   call l_char2num             ; a = digit
   jr c, number_complete

   cp (ix+0)
   jr nc, number_complete

   inc bc                      ; consume the char

   ; multiply pending result by base

   push af                     ; save new digit
   push bc                     ; save char *

   ld a,(ix+0)
   call l_mulu_40_32x8

   pop bc                      ; bc = char *

   or a                        ; result confined to 32 bits ?
   jr nz, unsigned_overflow

   pop af                      ; a = digit to add

   ; add digit to result

   add a,l                     ; dehl += a
   ld l,a
   jr nc, loop

   inc h
   jr nz, loop

   inc e
   jr nz, loop

   inc d
   jr nz, loop

   push af

unsigned_overflow:

   ;   bc = char * (next unconsumed char)
   ;  ixl = base
   ; stack = junk

   pop af

u_oflow:

   ; consume the entire number before reporting error

   ld l,c
   ld h,b                      ; hl = char *

   ld c,(ix+0)

   call l_eat_digits

   ld c,l
   ld b,h

   ld a,2
   scf

   ;   bc = char * (next unconsumed char)
   ;    a = 2 (error overflow)
   ; carry set for error

   ret

invalid_base:

   call invalid_input
   ld a,-2
   ret

invalid_input:

   ;  bc = original char*

   xor a

   ;   bc = original char *
   ; dehl = 0
   ;    a = 0 (error invalid input string)
   ; carry set for error

   jp error_lzc

number_complete:
   xor a                       ; carry reset and a=0
   ret


decimal:
   ;  hl = char *

   EXTERN l_atoul

   ex de,hl
   call l_atoul
   jr c, u_oflow

   xor a
   ret

hex:
   ;  hl = char *

   EXTERN l_htoul

   ex de,hl
   call l_htoul
   jr c, u_oflow

   xor a
   ret

octal:
   ; hl = char *

   EXTERN l_otoul

   ex de,hl
   call l_otoul
   jr c, u_oflow

   xor a
   ret

binary:
   ; hl = char *

   EXTERN l_btoul

   ex de,hl
   call l_btoul
   jr c, u_oflow

   xor a
   ret
