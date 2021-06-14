
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
	EXTERN	_NEWKEY
	EXTERN	_manageTimers
	EXTERN	_addVxVy
	EXTERN	_applicationExit
	EXTERN	_stack
	EXTERN	_stackIndex
	EXTERN	_tmsDraw
	PUBLIC	_drawFunctionPtr
	EXTERN	_v9958DrawSinglePlane
	EXTERN	_v9958DrawPlane3
	EXTERN	_videoResMode
	EXTERN	_v9958ScrollDown
	EXTERN	_v9958ScrollUp
	EXTERN	_v9958ScrollLeft
	EXTERN	_v9958ScrollRight

REGISTERS	EQU	$100

define(uniq,0)
define(lab, label$1$2$3$4$5$6$7$8$9)

define(`SKIP_NEXT_INSTRUCTION', `
define(`uniq', incr(uniq))
	exx

	; TEST FOR LOAD I LARGE (F000) , to skip skip 4 bytes
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

; LOAD HL TO ADDRESS OF REGISTER AS PER 2ND NIBBLE
; ASSUME IYH HAS INSTRUCTION HIGH
define(`SET_HL_REG_2nd', `
	LD	A, IYH
	AND	0x0F
	LD	L, A
	LD	H, REGISTERS / 256		; HL => REGISTERS[NIBBLE2ND]
')

; LOAD SW TO ADDRESS OF REGISTER AS PER 3RD NIBBLE
; ASSUME IYL HAS INSTRUCTION LOW & H is 1
define(`SET_DE_REG_3rd', `
	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, 0x0F
	LD	D, H ; REGISTERS / 256
	LD	E, A
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
;chip8/byte_code_executor.c:36: const uint16_t r = *chip8PC;
	exx
	LD	a, (hl)
	LD	(_currentInstruction+0), a
	LD	IYH, A
	inc	hl
	LD	a, (hl)
	LD	IYL, A
	LD	(_currentInstruction+1), a
	inc	hl
	exx

	CALL	_manageTimers

; if (CTRL_STOP_PRESSED())
	LD	a, (_NEWKEY + 6)
	bit	1, a
	jr	NZ, BCE_1
	LD	a, (_NEWKEY + 7)
	bit	4, a
	jr	NZ, BCE_1
;   return false;
	LD	l, 0x00
	JP	BCE_EXIT

BCE_1:
	LD	bc, _currentInstruction

;chip8/byte_code_executor.c:64: switch (firstNibble) {
	LD	A, IYH

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
	JP	BCE_6XXX
	db	0
	JP	BCE_7XXX
	db	0
	JP	BCE_8XXX
	db	0
	JP	BCE_9XXX
	db	0
	JP	BCE_AXXX
	db	0
	JP	BCE_BXXX
	db	0
	JP	BCE_CXXX
	db	0
	JP	BCE_DXXX
	db	0
	JP	BCE_EXXX
	db	0
	JP	BCE_FXXX

;chip8/byte_code_executor.c:65: case 0x0: {
BCE_0XXX:
;chip8/byte_code_executor.c:66: switch (highByte) {
	LD	a, IYH
	or	a, a
	JP	NZ, BCE_BAD_INSTRUCTION

; BCE_00XX (PART 1):
; switch (lowByte) {
	LD	A, IYL
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
ifndef CPM
	ld	a, (_videoResMode)
	or	a
	JP	Z, BCE_POST_PROCESS

	EXX
	PUSH	HL
	EXX
	CALL	_v9958ScrollRight
	EXX
	POP	HL
	EXX
endif
	JP	BCE_POST_PROCESS

BCE_00FC:	; SCRL LEFT
ifndef CPM
	ld	a, (_videoResMode)
	or	a
	JP	Z, BCE_POST_PROCESS

	EXX
	PUSH	HL
	EXX
	CALL	_v9958ScrollLeft
	EXX
	POP	HL
	EXX
endif
	JP	BCE_POST_PROCESS

BCE_00XX:
; (nibble3rd) {
	LD	A, IYL
	AND	A, $F0

	CP	A, $C0
	JR	Z, BCE_00CX

	CP	A, $D0
	JR	Z, BCE_00DX

	JP	BCE_BAD_INSTRUCTION

BCE_00CX:	; SCROLL DOWN
ifndef CPM
	ld	a, IYL
	AND	$0F
	JP	Z, BCE_POST_PROCESS
	ld	IYL, A

	ld	a, (_videoResMode)
	or	a
	JP	Z, BCE_POST_PROCESS

	EXX
	PUSH	HL
	EXX
	CALL	_v9958ScrollDown
	EXX
	POP	HL
	EXX
endif
	JP	BCE_POST_PROCESS

BCE_00DX:	; SCROLL UP
ifndef CPM
	ld	a, IYL
	AND	$0F
	JP	Z, BCE_POST_PROCESS
	ld	IYL, A

	ld	a, (_videoResMode)
	or	a
	JP	Z, BCE_POST_PROCESS

	EXX
	PUSH	HL
	EXX
	CALL	_v9958ScrollUp
	EXX
	POP	HL
	EXX
endif
	JP	BCE_POST_PROCESS

BCE_1XXX:	; JP XXX
	; READ 12bit address from instruction into HL'
	EXX
	LD	E, IYL
	LD	A, IYH
	AND	$0F
	LD	D, A
	EX	DE, HL

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
	LD	a, IYL
	LD	l, a
	LD	a, IYH
	and	a, 0x0f
	LD	h, a

	cp	0x02			; IS IT LESS THAN 0x200
	jr	C, BCE_2XXX_INVALID_ADDR

	EXX
	JP	BCE_POST_PROCESS

BCE_2XXX_INVALID_ADDR:
; printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
	DEC	DE		; CURRENT CHIP8 PC IS IN DE
	DEC	DE
	PUSH	DE
	PUSH	HL		; ADDRESS TO JUMP TO
	LD	HL,___str_1
	PUSH	HL
	CALL	_applicationExit
	LD	HL, 6
	ADD	HL, SP
	LD	SP, HL
	JP	BCE_EXIT_ERROR

BCE_STACK_OVERFLOWED:
; printf("Stack overflow\r\n");
	LD	HL, ___str_1
	PUSH	HL
	CALL	_applicationExit
	POP	AF
	JP	BCE_EXIT_ERROR

BCE_3XXX:	; SE Vx, byte
; if (registers[nibble2nd] == lowByte)
	SET_HL_REG_2nd()
	LD	E, (HL)		; E = VX
	LD	A, IYL		; A => LOWBYTE (XX)
	CP	E		; DO COMPARISON
	JP	NZ, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

BCE_4XXX:	; SNE Vx, byte
; if (registers[nibble2nd] != lowByte)
	SET_HL_REG_2nd()
	LD	E, (HL)
	LD	A, IYL
	CP	E
	JP	Z, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

BCE_5XXX:
; switch (readFourthNibble) {
	LD	A, IYL
	AND	A, $0F
	OR	A
	JR	Z, BCE_5XX0
	CP	2
	JR	Z, BCE_5XX2
	CP	3
	JR	Z, BCE_5XX3
	JP	BCE_BAD_INSTRUCTION

BCE_5XX0:	; SE Vx, Vy
; : if (registers[nibble2nd] == registers[nibble3rd])
	SET_HL_REG_2nd()
	LD	E, (HL)		; E = VX

	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	$0F
	LD	L, A
	LD	H, REGISTERS / 256
	LD	A, (HL)		; A = VY

	CP	E
	JP	NZ, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

BCE_5XX2:
	; 2nd nibble?
	LD	A, IYH
	AND	A, $0F
	LD	L, A

	; 3rd nibble
	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F

	CP	L
	JR	NC, ldIVxVyNormal

	LD	H, A
	LD	A, L
	SUB	H
	LD	B, A

	INC	B

	LD	DE, (_registerI)
	LD	H, REGISTERS / 256

ldIVxVyLoop:
	LD	A, (HL)
	LD	(DE), A
	DEC	HL
	INC	DE
	DJNZ	B, ldIVxVyLoop
	JP	BCE_POST_PROCESS

ldIVxVyNormal:
	LD	H, A
	SUB	L

	LD	C, A
	INC	C

	LD	B, 0
	LD	DE, (_registerI)
	LD	H, REGISTERS / 256

	LDIR
	JP	BCE_POST_PROCESS

BCE_5XX3:
	; 2nd nibble?
	LD	A, IYH
	AND	A, $0F
	LD	E, A
;
	; 3rd nibble
	LD	A, IYL
	RLCA
	RLCA
	RLCA
	RLCA
	AND	A, $0F

	CP	E
	JR	NC, ldVxVyINormal

	LD	H, A
	LD	A, E
	SUB	H
	LD	B, A

	INC	B

	LD	HL, (_registerI)
	LD	D, REGISTERS / 256

ldVxVyILoop:
	LD	A, (HL)
	LD	(DE), A
	INC	HL
	DEC	DE
	DJNZ	B, ldVxVyILoop
	JP	BCE_POST_PROCESS

ldVxVyINormal:

	LD	H, A
	SUB	E

	LD	C, A
	INC	C

	LD	B, 0
	LD	HL, (_registerI)
	LD	D, REGISTERS / 256

	LDIR
	JP	BCE_POST_PROCESS

BCE_6XXX:	; LD Vx, byte
	SET_HL_REG_2nd()
	LD	A, IYL
	LD	(HL), A
	JP	BCE_POST_PROCESS


BCE_7XXX:	; ADD Vx, byte
	SET_HL_REG_2nd()
	LD	A, IYL
	ADD	(HL)
	LD	(HL), A
	JP	BCE_POST_PROCESS

BCE_8XXX:
;chip8/byte_code_executor.c:166: switch (readFourthNibble) {
	LD	A, IYL
	AND	A, $0F
	JR	Z, BCE_8XX0
	DEC	A
	JR	Z, BCE_8XX1
	DEC	A
	JR	Z, BCE_8XX2
	DEC	A
	JR	Z, BCE_8XX3
	DEC	A
	JP	Z, BCE_8XX4
	DEC	A
	JP	Z, BCE_8XX5
	DEC	A
	JP	Z, BCE_8XX6
	DEC	A
	JP	Z, BCE_8XX7
	CP	$07
	JP	Z, BCE_8XXE
	JP	BCE_BAD_INSTRUCTION


BCE_8XX0:	; LD Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	LD	(HL), A
	JP	BCE_POST_PROCESS

BCE_8XX1:	; OR Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	OR	(HL)
	LD	(HL), A
	JP	BCE_POST_PROCESS

 BCE_8XX2:	; AND Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	AND	(HL)
	LD	(HL), A
	JP	BCE_POST_PROCESS

BCE_8XX3: 	; XOR Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	XOR	(HL)
	LD	(HL), A
	JP	BCE_POST_PROCESS

BCE_8XX4:	;  ADD Vx, Vy, VF=CARRY
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	ADD	(HL)
	LD	(HL), A
	JR	NC, BCE_CLR_CARRY

	LD      A, 1
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

BCE_CLR_CARRY:
	XOR	A
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

BCE_8XX5:	; SUB Vx, Vy, VF=BORROW
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	EX	DE, HL
	LD	A, (DE)		; A => VY
	SUB	(HL)		; A = A - (VX)
	LD	(DE), A
	JR	C, BCE_CLR_CARRY

	LD      A, 1
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

BCE_8XX6:	; SHR Vx
	SET_HL_REG_2nd()

	LD	A, (HL)
	SRL	A
	LD	(HL), A
	JR	NC, BCE_CLR_CARRY

	LD      A, 1
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:202: case 0x7:
BCE_8XX7:	; SUBN Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)		; A => VY
	SUB	(HL)		; A = A - (VX)
	LD	(HL), A
	JR	C, BCE_CLR_CARRY

	LD      A, 1
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

BCE_8XXE:	;  SHL Vx
	SET_HL_REG_2nd()

	LD	A, (HL)
	SLA	A
	LD	(HL), A
	JR	NC, BCE_CLR_CARRY

	LD      A, 1
	LD	(REGISTERS + $0F), A
	JP	BCE_POST_PROCESS

BCE_9XXX:
; if (readFourthNibble == 0)
	LD	a, IYL
	and	a, 0x0f
	JP	NZ, BCE_BAD_INSTRUCTION

BCE_9XX0:	; SNE Vx, Vy
	SET_HL_REG_2nd()
	SET_DE_REG_3rd()

	LD	A, (DE)
	CP	(HL)
	JP	Z, BCE_POST_PROCESS
	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

BCE_AXXX:	; LD I, addr
	LD	HL, _registerI
	LD	A, IYL
	LD	(HL), A
	INC	HL
	LD	A, IYH
	AND	$0F
	LD	(HL), A
	JP	BCE_POST_PROCESS

BCE_BXXX:	; JP V0, addr
	EXX
	LD	E, IYL
	LD	A, IYH
	AND	$0F
	LD	D, A
	EX	DE, HL		; CURRENT PC TO DE

	LD	A, ($100)
	LD	C, A
	LD	B, 0
	ADD	HL, BC		; NEW PC IN HL

	LD	A, H
	EXX
	CP	$02
	JP	NC, BCE_POST_PROCESS

; printf("Illegal jump to %04X at %p\r\n", addr12Bit, chip8PC - 1);
	EXX
	DEC	DE
	DEC	DE
	LD	BC,___str_1+0
	PUSH	DE
	PUSH	HL
	PUSH	BC
	CALL	_applicationExit
	LD	HL,6
	ADD	HL, SP
	LD	SP, HL
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:235: case 0xC: {
BCE_CXXX:
;chip8/byte_code_executor.c:236: rnd();
	LD	a, IYH
	and	a,0x0f
	LD	d,0x00
	LD	e, a
	LD	a, d
	inc	a
	LD	d, a
	push	iy
	push	de
	CALL	_chip8Rand
	LD	a, l
	pop	de
	pop	iy
	LD	c, IYL
	and	a, c
	LD	(de), a
	JP	BCE_POST_PROCESS


BCE_DXXX:
;chip8/byte_code_executor.c:241: draw();
	exx			; PROTECT CHIP8 PC Counter
	push	hl
	exx

	CALL	_tmsDraw
_drawFunctionPtr	EQU	$-2

	exx			; PROTECT CHIP8 PC Counter
	pop	hl
	exx

	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:245: case 0xE: {
BCE_EXXX:
;chip8/byte_code_executor.c:246: switch (lowByte) {
	LD	a, IYL
	cp	0x9e
	jr	Z, BCE_EX9E
	sub	a,0xa1
	jr	Z, BCE_EXA1
	JP	BCE_BAD_INSTRUCTION

BCE_EX9E:
; if (isKeyDown(registers[nibble2nd]))
	SET_HL_REG_2nd()
	LD	L, (HL)
	CALL	_isKeyDown
	LD	A, L
	OR	A, A
	JP	Z, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:252: case 0xA1: {
BCE_EXA1:
;chip8/instr_pc.h:74: if (!isKeyDown(registers[nibble2nd]))
	SET_HL_REG_2nd()
	LD	L, (HL)
	CALL	_isKeyDown
	LD	A, L
	OR	A
	JP	NZ, BCE_POST_PROCESS

	SKIP_NEXT_INSTRUCTION()
	JP	BCE_POST_PROCESS

;chip8/byte_code_executor.c:264: case 0xF: {
BCE_FXXX:
;chip8/byte_code_executor.c:265: switch (lowByte) {
	LD	a, IYL
	or	a, a
	jr	Z,l_executeSingleInstruction_00161
	cp	a,0x01
	jr	Z, BCE_FX01
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
	JP	Z,BCE_FX33
	cp	a,0x55
	JP	Z,l_executeSingleInstruction_00177
	sub	a,0x65
	JP	Z,l_executeSingleInstruction_00178
	JP	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:267: case 0x00:
l_executeSingleInstruction_00161:
;chip8/byte_code_executor.c:268: if (highByte == 0xF0)
	LD	a, IYH
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

 BCE_FX01:	; PLANE X
	LD	A, (BC)
	AND	A, $0F
	LD	(__color), A
	CP	3
	JR	Z, BCE_SET_COLOR3
	LD	HL, _v9958DrawSinglePlane
	LD	(_drawFunctionPtr), HL
	JP	BCE_POST_PROCESS
BCE_SET_COLOR3:
	LD	HL, _v9958DrawPlane3
	LD	(_drawFunctionPtr), HL
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
	jP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:302: case 0x18: {
l_executeSingleInstruction_00173:
;chip8/byte_code_executor.c:303: ldStVx();
	CALL	_ldStVx
;chip8/byte_code_executor.c:304: break;
	jP	BCE_POST_PROCESS
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
	jP	BCE_POST_PROCESS
;chip8/byte_code_executor.c:312: case 0x29: {
l_executeSingleInstruction_00175:
;chip8/byte_code_executor.c:313: ldfIVx();
	CALL	_ldfIVx
;chip8/byte_code_executor.c:314: break;
	jP	BCE_POST_PROCESS

BCE_FX33:	; LD BCD, Vx
	SET_HL_REG_2nd()
	LD	A, (HL)

	LD      D, 0
	CP      100
	JR      C, BCE_FX33_SKP1
	INC     D

BCE_FX33_SKP1:
	CP      200
	JR      C, BCE_FX33_SKP2
	INC     D

BCE_FX33_SKP2:
	LD      C, A
	LD      B, 8
	XOR     A
LOOP:
	SLA     C
	ADC     A, A
	DAA
	DJNZ    LOOP
	LD      E, A

	LD	HL, (_registerI)
	LD	(HL), D

	AND	$F0
	RRCA
	RRCA
	RRCA
	RRCA
	INC	HL
	LD	(HL), A
	INC	HL
	LD	A, E
	AND	$0F
	LD	(HL), A
	JR	BCE_POST_PROCESS

;chip8/byte_code_executor.c:322: case 0x55: {
l_executeSingleInstruction_00177:
;chip8/instr_registers.h:82: __endasm;
	LD	de, (_registerI)
	LD	hl, 0x100
	LD	a, IYH
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
	LD	a, IYH
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
	push	iy
	LD	bc, ___str_2
	push	bc
	CALL	_applicationExit		; ONLY RETURNS WHEN RUNNING UNDER TESTS
	LD	hl, 6
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
