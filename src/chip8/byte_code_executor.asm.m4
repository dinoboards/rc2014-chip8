
	EXTERN	_invertByteOrder
	EXTERN	_currentInstruction
	EXTERN	_registerI
	EXTERN	_bcdIVx
	EXTERN	_ldStVx
	EXTERN	_delayTimer
	EXTERN	_initTimers
	EXTERN	__color
	EXTERN	_audio
	EXTERN	_ldfIVx
	EXTERN	_currentKey
	EXTERN	_isKeyDown
	EXTERN	_draw
	EXTERN	_chip8Rand
	EXTERN	_subnVxVy
	EXTERN	_subVxVy
	EXTERN	_ldVxVyI
	EXTERN	_ldIVxVy
	EXTERN	_pushPc
	EXTERN	_scrlUp
	EXTERN	_scrlDown
	EXTERN	_scrlLeft
	EXTERN	_scrlRight
	EXTERN	_cls
	EXTERN	_videoHigh
	EXTERN	_fourthNibble
	EXTERN	_NEWKEY
	EXTERN	_manageTimers
	EXTERN	_addVxVy
	EXTERN	_applicationExit
	EXTERN	_stack
	EXTERN	_stackIndex

define(uniq,0)
define(lab, label$1$2$3$4$5$6$7$8$9)

define(`SKIP_NEXT_INSTRUCTION', `
define(`uniq', incr(uniq))
	exx

	LD	a, (hl)
	cp	0xF0
	jr	nz, lab(skip2bytes, uniq())
	inc	hl
	LD	a, (hl)
	or	a
	jr	nz, lab(skip1byte, uniq())

	inc	hl

lab(skip2bytes, uniq()):
	inc	hl
lab(skip1byte, uniq()):
	inc	hl

	exx
')

	SECTION code_compiler
; uint16_t getChip8PC();
	PUBLIC	_getChip8PC
_getChip8PC:
	EXX
	PUSH	HL
	EXX
	POP	HL
	RET

; void setChip8PC(uint16_t pc) __z88dk_fastCALL
	PUBLIC	_setChip8PC
_setChip8PC:
	PUSH	HL
	EXX
	POP	HL
	EXX
	RET

	PUBLIC _invertByteOrder
; extern uint16_t invertByteOrder(uint16_t word) __naked __z88dk_fastCALL {
_invertByteOrder:
	LD	A, H
	LD	H, L
	LD	L, A
	RET

;chip8/byte_code_executor.c:50: bool executeSingleInstruction() {
;	---------------------------------
; Function executeSingleInstruction
; ---------------------------------
	PUBLIC	_executeSingleInstruction
_executeSingleInstruction:
	push	ix
	LD	ix,0
	add	ix,sp
	dec	sp
;chip8/byte_code_executor.c:36: const uint16_t r = *chip8PC;
	exx
	LD	a, (hl)
	LD	(_currentInstruction+0), a
	inc	hl
	LD	a, (hl)
	LD	(_currentInstruction+1), a
	inc	hl
	exx

	CALL	_manageTimers

; if (CTRL_STOP_PRESSED())
	LD	a, (_NEWKEY + 0x0006)
	bit	1, a
	jr	NZ, BCE_1
	LD	a, (_NEWKEY + 0x0007)
	bit	4, a
	jr	NZ, BCE_1
;   return false;
	LD	l, 0x00
	JP	BCE_EXIT

BCE_1:
;chip8/byte_code_executor.c:58: fourthNibble = readFourthNibble;
	LD	bc, _currentInstruction
	LD	a, (_currentInstruction + 1)
	and	a, 0x0f
	LD	(_fourthNibble), a

;chip8/byte_code_executor.c:64: switch (firstNibble) {
	LD	a, (bc)

	rlca
	rlca
	rlca
	rlca
	and	a,0x0f

	LD	l, a
	LD	h, 0
	add	hl, hl
	add	hl, hl
	LD	de, BCE_FIRST_NIBBLE_TABLE
	add	hl, de
	JP	(hl)

BCE_FIRST_NIBBLE_TABLE:
	JP	BCE_0XXX
	db	0
	JP	BCE_1XXX
	db	0
	JP	BCE_2XXX
	db	0
	JP	BCE_3XXX
	db	0
	JP	BCE_4XXX
	db	0
	JP	BCE_5XXX
	db	0
	JP	l_executeSingleInstruction_00133
	db	0
	JP	l_executeSingleInstruction_00134
	db	0
	JP	BCE_8XXX
	db	0
	JP	BCE_9XXX
	db	0
	JP	l_executeSingleInstruction_00151
	db	0
	JP	l_executeSingleInstruction_00152
	db	0
	JP	l_executeSingleInstruction_00153
	db	0
	JP	l_executeSingleInstruction_00154
	db	0
	JP	BCE_EXXX
	db	0
	JP	BCE_FXXX

;chip8/byte_code_executor.c:65: case 0x0: {
BCE_0XXX:
;chip8/byte_code_executor.c:66: switch (highByte) {
	LD	a, (bc)
	or	a, a
	JP	NZ, BCE_BAD_INSTRUCTION

; BCE_00XX (PART 1):
; switch (lowByte) {
	LD	A, (_currentInstruction + 1)
	CP	A, $E0
	JR	Z, BCE_00E0
	CP	A, $EE
	JR	Z, BCE_00EE
	CP	A, $FB
	JR	Z, BCE_00FB
	CP	A, $FC
	JR	Z, BCE_00FC
	INC	A
	JR	NZ, BCE_00XX

BCE_00FF:
	CALL	_videoHigh
	JP	BCE_POST_PROCESS

BCE_00E0:
	CALL	_cls
	JP	BCE_POST_PROCESS

BCE_00EE:	; RET
; if (stackIndex <= 0)
	LD	L, 0
	LD	A, (_stackIndex)
	OR	A
	JR	Z, BCE_STACK_EMPTY

; chipPC = stack[--stackIndex];
	DEC	A
	LD	(_stackIndex), a

	EXX			; PC IS HL'
	ADD	A
	LD	HL, _stack
	OR	L
	LD	L, A		; HL' => _stack + (_stackIndex * 2)
	LD	A, (HL)
	INC	HL		; HL' = (HL')
	LD	H, (HL)
	LD	L, A		; HL' now contains new PC
	EXX
	JP	BCE_POST_PROCESS

BCE_STACK_EMPTY:
	JP	BCE_EXIT_ERROR	; return exit signal

BCE_00FB:	; SCRL RIGHT
	CALL	_scrlRight
	JP	BCE_POST_PROCESS

BCE_00FC:	; SCRL LEFT
	CALL	_scrlLeft
	JP	BCE_POST_PROCESS

BCE_00XX:
; (nibble3rd) {
	INC	BC
	LD	A, (BC)
	AND	A, $F0

	CP	A, $C0
	JR	Z, BCE_00CX

	CP	A, $D0
	JR	Z, BCE_00DX

	JP	BCE_BAD_INSTRUCTION

BCE_00CX:
	CALL	_scrlDown
	JP	BCE_POST_PROCESS

BCE_00DX:
	CALL	_scrlUp
	JP	BCE_POST_PROCESS

BCE_1XXX:	; JP XXX
	; READ 12bit address from instruction into HL'
	EXX
	EX	DE, HL			; TRANSFER CURRENT CHIP8 TO DE'
	LD	A, (_currentInstruction + 1)
	LD	L, A
	LD	A, (_currentInstruction)
	AND	$0F
	LD	H, A			; HL' NOW CONTAINS NEW CHIP8 PC

	CP	$02			; IS IT LESS THAN $0200
	jr	C, BCE_1XXX_INVALID_ADDR

	EXX
	JP	BCE_POST_PROCESS

BCE_1XXX_INVALID_ADDR:
;  printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
	DEC	DE
	DEC	DE
	EX	DE, HL
	PUSH	HL
	PUSH	DE
	EXX
	LD	hl, ___str_1
	PUSH	HL
	CALL	_applicationExit
	LD	HL, 6
	ADD	HL, SP
	LD	SP, HL
	JP	BCE_EXIT_ERROR


BCE_2XXX: 	; CALL XXX
	; SAVE CURRENT PC ONTO STACK
	LD	a, (_stackIndex)	; IS THERE ROOOM ON THE STACK?
	CP	0x10
	jr	NC, BCE_STACK_OVERFLOWED

	INC	A			; _stackIndex++
	LD	(_stackIndex), A
	DEC	A

	EXX
	EX	DE, HL			; DE => CURRENT PC
	LD	HL, _stack
	ADD	A
	OR	L
	LD	L, A			; HL => STACK REFERENCE
	LD	(HL), E
	INC	HL
	LD	(HL), D			; (STACK) = DE

	; RETRIEVE 12 BIT CALL ADDR
	LD	a, (_currentInstruction + 1)
	LD	l, a
	LD	a, (_currentInstruction)
	and	a, 0x0f
	LD	h, a

	cp	0x02			; IS IT LESS THAN 0x200
	jr	C, BCE_2XXX_INVALID_ADDR

	EXX
	JP	BCE_POST_PROCESS

BCE_2XXX_INVALID_ADDR:
; printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
	dec	de		; current chip8 PC is in DE
	dec	de
	push	de
	push	hl		; Address to jump to
	LD	hl,___str_1
	push	hl
	CALL	_applicationExit
	LD	hl, 6
	add	hl, sp
	LD	sp, hl
	JP	BCE_EXIT_ERROR

BCE_STACK_OVERFLOWED:
; printf("Stack overflow\r\n");
	LD	hl,___str_1
	push	hl
	CALL	_applicationExit
	pop	af
	JP	BCE_EXIT_ERROR

;chip8/byte_code_executor.c:124: case 0x3: {
BCE_3XXX:	; SE Vx, byte

; if (registers[nibble2nd] == lowByte)
	LD	a, (BC)
	and	0x0f
	LD	l, a
	LD	h, 0x01		; HL => REGISTERS[NIBBLE2ND]
	LD	e, (hl)		; E = VX
	inc	bc
	LD	a, (bc)		; A => LOWBYTE (XX)
	CP	e		; DO COMPARISON
	JP	NZ, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:129: case 0x4: {
BCE_4XXX:
;chip8/instr_pc.h:37: if (registers[nibble2nd] != lowByte)
	LD	e, c
	LD	d, b
	LD	a, (de)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	e, (hl)
	inc	bc
	LD	a, (bc)
	sub	a, e
	JP	Z,BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:134: case 0x5: {

BCE_5XXX:
;chip8/byte_code_executor.c:136: switch (readFourthNibble) {
	LD	a, (_currentInstruction + 1)
	and	a,0x0f
	or	a
	jr	Z, BCE_5XX0
	cp	2
	jr	Z, BCE_5XX2
	cp	3
	jr	Z, BCE_5XX3
	JP	BCE_BAD_INSTRUCTION

;chip8/byte_code_executor.c:137: case 0x0:
BCE_5XX0:
;chip8/instr_pc.h:32: if (registers[nibble2nd] == registers[nibble3rd])
	LD	e, c
	LD	d, b
	LD	a, (de)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	e, (hl)
	inc	bc
	LD	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	LD	c, a
	LD	a,0x00
	inc	a
	LD	b, a
	LD	a, (bc)
	sub	a, e
	JP	NZ,BCE_POST_PROCESS
	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:141: case 0x2:
 BCE_5XX2:
;chip8/byte_code_executor.c:142: ldIVxVy();
	CALL	_ldIVxVy
;chip8/byte_code_executor.c:143: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:145: case 0x3:
 BCE_5XX3:
;chip8/byte_code_executor.c:146: ldVxVyI();
	CALL	_ldVxVyI
;chip8/byte_code_executor.c:147: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:155: case 0x6:
l_executeSingleInstruction_00133:
;chip8/byte_code_executor.c:156: ldVxByte();
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	d,0x00
	LD	e, a
	LD	a, d
	inc	a
	LD	d, a
	inc	bc
	LD	a, (bc)
	LD	(de), a
;chip8/byte_code_executor.c:157: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:159: case 0x7: {
l_executeSingleInstruction_00134:
;chip8/byte_code_executor.c:160: addVxByte();
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	d,0x00
	LD	e, a
	LD	a, d
	inc	a
	LD	d, a
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	a, (hl)
	LD	l, c
	LD	h, b
	inc	hl
	LD	c, (hl)
	add	a, c
	LD	(de), a
;chip8/byte_code_executor.c:161: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:164: case 0x8: {
BCE_8XXX:
;chip8/byte_code_executor.c:166: switch (readFourthNibble) {
	LD	a, (_currentInstruction + 1)
	and	a,0x0f
	jr	Z, BCE_8XX0
	dec	a
	jr	Z, BCE_8XX1
	dec	a
	jr	Z, BCE_8XX2
	dec	a
	jr	Z, BCE_8XX3
	dec	a
	JP	Z, BCE_8XX4
	dec	a
	JP	Z, BCE_8XX5
	dec	a
	JP	Z, BCE_8XX6
	dec	a
	JP	Z, BCE_8XX7
	cp	0x07
	JP	Z, BCE_8XXE
	JP	BCE_BAD_INSTRUCTION

BCE_8XX0:
	LD	hl, (_currentInstruction)
	LD	a, l
	and	a, 0x0f
	LD	d,0x100 / 256
	LD	e, a
	LD	a, h
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	LD	c, a
	LD	b, 0x100 / 256
	LD	a, (bc)
	LD	(de), a
	JP	BCE_POST_PROCESS

BCE_8XX1:
	LD	d, 0x100 / 256
	LD	a, (_currentInstruction)
	and	a, 0x0F
	LD	e, a
	LD	h, d
	LD	a, (_currentInstruction + 1)
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	LD	l, a
	LD	a, (de)
	or	a, (hl)
	LD	(de), a
	JP	BCE_POST_PROCESS

 BCE_8XX2:
	LD	d, 0x100 / 256
	LD	a, (_currentInstruction)
	and	a, 0x0F
	LD	e, a
;
	LD	h, d
	LD	a, (_currentInstruction + 1)
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	LD	l, a
	LD	a, (de)
	and	a, (hl)
	LD	(de), a
	JP	BCE_POST_PROCESS

BCE_8XX3:
;chip8/instr_registers.h:211: inline void xorVxVy() { registers[nibble2nd] ^= registers[nibble3rd]; }
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	d,0x00
	LD	e, a
	LD	a, d
	inc	a
	LD	d, a
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	a, (hl)
	LD	(ix-1),a
	inc	bc
	LD	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	LD	c, a
	LD	a,0x00
	inc	a
	LD	b, a
	LD	a, (bc)
	xor	a,(ix-1)
	LD	(de), a
	JP	BCE_POST_PROCESS
BCE_8XX4:
	CALL	_addVxVy
	JP	BCE_POST_PROCESS

BCE_8XX5:
	CALL	_subVxVy
	JP	BCE_POST_PROCESS

BCE_8XX6:
;chip8/instr_registers.h:143: uint8_t *register2ndNibble = &registers[nibble2nd];
	LD	a, (bc)
	and	a,0x0f
	LD	b,0x00
	LD	c, a
	LD	a, b
	inc	a
	LD	b, a
;chip8/instr_registers.h:144: registers[0xF] = *register2ndNibble & 0x1;
	LD	a, (bc)
	and	a,0x01
	LD	(0x010f),a
;chip8/instr_registers.h:145: *register2ndNibble >>= 1;
	LD	a, (bc)
	srl	a
	LD	(bc), a
;chip8/byte_code_executor.c:199: break;
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:202: case 0x7:
BCE_8XX7:
	CALL	_subnVxVy
	JP	BCE_POST_PROCESS
BCE_8XXE:
;chip8/instr_registers.h:149: uint8_t *register2ndNibble = &registers[nibble2nd];
	LD	a, (bc)
	and	a,0x0f
	LD	b,0x00
	LD	c, a
	LD	a, b
	inc	a
	LD	b, a
;chip8/instr_registers.h:151: registers[0xF] = !!(*register2ndNibble & 0x80);
	LD	a, (bc)
	rlca
	and	a,0x01
	xor	a,0x01
	xor	a,0x01
	LD	(0x010f),a
;chip8/instr_registers.h:152: *register2ndNibble <<= 1;
	LD	a, (bc)
	add	a, a
	LD	(bc), a
;chip8/byte_code_executor.c:208: break;
	JP	BCE_POST_PROCESS

BCE_9XXX:
;chip8/byte_code_executor.c:218: if (readFourthNibble == 0)
	LD	a, (_currentInstruction + 1)
	and	a,0x0f
	JP	NZ,BCE_BAD_INSTRUCTION
;chip8/instr_pc.h:42: if (registers[nibble2nd] != registers[nibble3rd])
	LD	e, c
	LD	d, b
	LD	a, (de)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	e, (hl)
	inc	bc
	LD	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	LD	c, a
	LD	a,0x00
	inc	a
	LD	b, a
	LD	a, (bc)
	sub	a, e
	JP	Z,BCE_POST_PROCESS
	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:225: case 0xA:
l_executeSingleInstruction_00151:
;chip8/byte_code_executor.c:226: ldIAddr();
	LD	a, (_currentInstruction + 1)
	LD	e, a
	LD	d,0x00
	LD	a, (bc)
	and	a,0x0f
	LD	b, a
	LD	c,0x00
	LD	a, e
	LD	hl,_registerI
	add	a, c
	LD	(hl), a
	LD	a, d
	adc	a, b
	inc	hl
	LD	(hl), a
;chip8/byte_code_executor.c:229: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:231: case 0xB:
l_executeSingleInstruction_00152:
;chip8/instr_pc.h:58: chip8PC = (uint16_t *)(addr12Bit + registers[0]);
	LD	a, (_currentInstruction + 1)
	LD	l, a
	LD	h,0x00
	LD	e, c
	LD	d, b
	LD	a, (de)
	and	a,0x0f
	LD	d, a
	LD	e,0x00
	add	hl, de
	ex	de, hl
	LD	hl,0x0100
	LD	l, (hl)
	LD	h,0x00
	add	hl, de
	push	hl
	exx
	pop	hl
	exx
;chip8/instr_pc.h:60: if ((uint16_t)chip8PC < 0x200) {
	exx
	LD	a, h
	exx
	sub	a,0x02
	JP	NC, BCE_POST_PROCESS
;chip8/instr_pc.h:61: printf("Illegal jump to %04X at %p\r\n", addr12Bit, chip8PC - 1);
	exx
	push	hl
	exx
	pop	de
	dec	de
	dec	de
	LD	a, (_currentInstruction + 1)
	LD	l, a
	LD	h,0x00
	LD	a, (bc)
	and	a,0x0f
	LD	c, a
	LD	a,0x00
	LD	b, c
	LD	c, a
	add	hl, bc
	LD	bc,___str_1+0
	push	de
	push	hl
	push	bc
	CALL	_applicationExit
	LD	hl,6
	add	hl, sp
	LD	sp, hl
;chip8/byte_code_executor.c:62: #endif
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:235: case 0xC: {
l_executeSingleInstruction_00153:
;chip8/byte_code_executor.c:236: rnd();
	LD	l, c
	LD	h, b
	LD	a, (hl)
	and	a,0x0f
	LD	d,0x00
	LD	e, a
	LD	a, d
	inc	a
	LD	d, a
	push	bc
	push	de
	CALL	_chip8Rand
	LD	a, l
	pop	de
	pop	hl
	inc	hl
	LD	c, (hl)
	and	a, c
	LD	(de), a
;chip8/byte_code_executor.c:237: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:240: case 0xD: {
l_executeSingleInstruction_00154:
;chip8/byte_code_executor.c:241: draw();
	CALL	_draw
;chip8/byte_code_executor.c:242: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:245: case 0xE: {
BCE_EXXX:
;chip8/byte_code_executor.c:246: switch (lowByte) {
	LD	a, (_currentInstruction + 1)
	cp	0x9e
	jr	Z, BCE_EX9E
	sub	a,0xa1
	jr	Z, BCE_EXA1
	JP	BCE_BAD_INSTRUCTION

BCE_EX9E:
; if (isKeyDown(registers[nibble2nd]))
	LD	a, (bc)
	and	0x0f
	LD	l, a
	LD	h, 0x01
	LD	l, (hl)
	CALL	_isKeyDown
	LD	a, l
	or	a, a
	JP	Z, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:252: case 0xA1: {
BCE_EXA1:
;chip8/instr_pc.h:74: if (!isKeyDown(registers[nibble2nd]))
	LD	a, (bc)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	l, (hl)
	CALL	_isKeyDown
	LD	a, l
	or	a, a
	JP	NZ, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:264: case 0xF: {
BCE_FXXX:
;chip8/byte_code_executor.c:265: switch (lowByte) {
	LD	a, (_currentInstruction + 1)
	or	a, a
	jr	Z,l_executeSingleInstruction_00161
	cp	a,0x01
	jr	Z,l_executeSingleInstruction_00165
	cp	a,0x02
	jr	Z,l_executeSingleInstruction_00166
	cp	a,0x07
	jr	Z,l_executeSingleInstruction_00170
	cp	a,0x0a
	jr	Z,l_executeSingleInstruction_00171
	cp	a,0x15
	JP	Z,l_executeSingleInstruction_00172
	cp	a,0x18
	JP	Z,l_executeSingleInstruction_00173
	cp	a,0x1e
	JP	Z,l_executeSingleInstruction_00174
	cp	a,0x29
	JP	Z,l_executeSingleInstruction_00175
	cp	a,0x33
	JP	Z,l_executeSingleInstruction_00176
	cp	a,0x55
	JP	Z,l_executeSingleInstruction_00177
	sub	a,0x65
	JP	Z,l_executeSingleInstruction_00178
	JP	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:267: case 0x00:
l_executeSingleInstruction_00161:
;chip8/byte_code_executor.c:268: if (highByte == 0xF0)
	LD	a, (bc)
	sub	a,0xf0
	JP	NZ,BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:270: ldILargeAddr();
	exx
	LD	d, (hl)
	inc	hl
	LD	e, (hl)
	inc	hl
	LD	(_registerI), de
	exx
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:275: case 0x01: {
l_executeSingleInstruction_00165:
;chip8/byte_code_executor.c:276: videoPlane();
	LD	a, (bc)
	and	a,0x0f
	LD	(__color+0), a
;chip8/byte_code_executor.c:277: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:280: case 0x02:
l_executeSingleInstruction_00166:
;chip8/byte_code_executor.c:281: if (highByte == 0xF0)
	LD	a, (bc)
	sub	a,0xf0
	JP	NZ,BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:282: audio();
	CALL	_audio
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:287: case 0x07: {
l_executeSingleInstruction_00170:
;chip8/byte_code_executor.c:288: ldVxDt();
	LD	a, (bc)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	a,(_delayTimer + 0)
	LD	(hl), a
;chip8/byte_code_executor.c:289: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:292: case 0x0A: {
l_executeSingleInstruction_00171:
;chip8/instr_pc.h:79: const uint8_t b = currentKey();
	push	bc
	CALL	_currentKey
	LD	a, l
	pop	bc
;chip8/instr_pc.h:80: if (b != 255) {
	LD	e,a
	inc	a
	jr	Z,l_executeSingleInstruction_00239
;chip8/instr_pc.h:81: registers[nibble2nd] = b;
	LD	a, (bc)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	(hl), e
;chip8/byte_code_executor.c:82: case 0xFB:
	JP	BCE_POST_PROCESS
l_executeSingleInstruction_00239:
;chip8/instr_pc.h:84: chip8PC -= 1;
	exx
	dec	hl
	dec	hl
	exx
;chip8/byte_code_executor.c:294: break;
	JP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:297: case 0x15: {
l_executeSingleInstruction_00172:
;chip8/byte_code_executor.c:298: ldDtVx();
	push	bc
	CALL	_initTimers
	pop	bc
	LD	a, (bc)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	a, (hl)
	LD	(_delayTimer+0), a
;chip8/byte_code_executor.c:299: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:302: case 0x18: {
l_executeSingleInstruction_00173:
;chip8/byte_code_executor.c:303: ldStVx();
	CALL	_ldStVx
;chip8/byte_code_executor.c:304: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:307: case 0x1E: {
l_executeSingleInstruction_00174:
;chip8/byte_code_executor.c:308: addIVx();
	LD	a, (bc)
	and	a,0x0f
	LD	l, a
	LD	a,0x00
	inc	a
	LD	h, a
	LD	c, (hl)
	LD	b,0x00
	LD	hl,_registerI
	LD	a, (hl)
	add	a, c
	LD	(hl), a
	inc	hl
	LD	a, (hl)
	adc	a, b
	LD	(hl), a
;chip8/byte_code_executor.c:309: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:312: case 0x29: {
l_executeSingleInstruction_00175:
;chip8/byte_code_executor.c:313: ldfIVx();
	CALL	_ldfIVx
;chip8/byte_code_executor.c:314: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:317: case 0x33: {
l_executeSingleInstruction_00176:
;chip8/byte_code_executor.c:318: bcdIVx();
	CALL	_bcdIVx
;chip8/byte_code_executor.c:319: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:322: case 0x55: {
l_executeSingleInstruction_00177:
;chip8/instr_registers.h:82: __endasm;
	LD	de, (_registerI)
	LD	hl, 0x100
	LD	a, (_currentInstruction)
	and	a, 0x0F
	inc	a
	LD	c, a
	LD	b, 0
	ldir
;chip8/byte_code_executor.c:324: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:327: case 0x65: {
l_executeSingleInstruction_00178:
;chip8/instr_registers.h:64: __endasm;
	LD	hl, (_registerI)
	LD	de, 0x100
	LD	a, (_currentInstruction)
	and	a, 0x0F
	inc	a
	LD	c, a
	LD	b, 0
	ldir
;chip8/byte_code_executor.c:329: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:339: badInstruction:
BCE_BAD_INSTRUCTION:
;chip8/byte_code_executor.c:340: printf("Bad instruction %04X at %p\r\n", invertByteOrder(currentInstruction), chip8PC - 1);
	exx
	push	hl
	exx
	pop	de
	dec	de
	dec	de
	push	de
	LD	hl, (_currentInstruction)
	CALL	_invertByteOrder
	pop	de
	LD	bc,___str_2+0
	push	de
	push	hl
	push	bc
	CALL	_applicationExit		; ONLY RETURNS WHEN RUNNING UNDER TESTS
	LD	hl,6
	add	hl, sp
	LD	sp, hl
;chip8/byte_code_executor.c:341: return false;
	LD	l,0x00
	jr	BCE_EXIT
;chip8/byte_code_executor.c:342: }

BCE_POST_PROCESS:
;chip8/byte_code_executor.c:344: if ((uint16_t)chip8PC < 0x200) {
	exx
	LD	a, h
	exx
	sub	a, 0x02
	jr	NC, BCE_EXIT_OK
;chip8/byte_code_executor.c:345: printf("PC counter below 0x200 - %p\r\n", chip8PC);
	exx
	push	hl
	exx
	pop	hl
	push	hl
	LD	hl,___str_3
	push	hl
	CALL	_applicationExit
	pop	af
	pop	af
;chip8/byte_code_executor.c:346: return false;
BCE_EXIT_ERROR:
	LD	l,0x00
	jr	BCE_EXIT
;chip8/byte_code_executor.c:349: return true;
;chip8/byte_code_executor.c:350: }
BCE_EXIT_OK:
	LD	l, 1
BCE_EXIT:
	inc	sp
	pop	ix
	ret
	SECTION rodata_compiler
___str_1:
	DEFM "Illegal jump to %04X at %p"
	DEFB 0x0d
	DEFB 0x0a
	DEFB 0x00
	SECTION rodata_compiler
___str_2:
	DEFM "Bad instruction %04X at %p"
	DEFB 0x0d
	DEFB 0x0a
	DEFB 0x00
	SECTION rodata_compiler
___str_3:
	DEFM "PC counter below 0x200 - %p"
	DEFB 0x0d
	DEFB 0x0a
	DEFB 0x00
	SECTION code_compiler
