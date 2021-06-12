
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
	EXTERN	_popPc
	EXTERN	_cls
	EXTERN	_videoHigh
	EXTERN	_fourthNibble
	EXTERN	_NEWKEY
	EXTERN	_manageTimers
	EXTERN	_addVxVy
	EXTERN	_applicationExit

	SECTION code_compiler

; uint16_t getChip8PC();
	PUBLIC	_getChip8PC
_getChip8PC:
	EXX
	PUSH	HL
	EXX
	POP	HL
	RET

; void setChip8PC(uint16_t pc) __z88dk_fastcall
	PUBLIC	_setChip8PC
_setChip8PC:
	PUSH	HL
	EXX
	POP	HL
	EXX
	RET

	PUBLIC _invertByteOrder
; extern uint16_t invertByteOrder(uint16_t word) __naked __z88dk_fastcall {
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
	ld	ix,0
	add	ix,sp
	dec	sp
;chip8/byte_code_executor.c:36: const uint16_t r = *chip8PC;
	exx
	ld	a, (hl)
	ld	(_currentInstruction+0), a
	inc	hl
	ld	a, (hl)
	ld	(_currentInstruction+1), a
	inc	hl
	exx

	call	_manageTimers

; if (CTRL_STOP_PRESSED())
	ld	a, (_NEWKEY + 0x0006)
	bit	1, a
	jr	NZ, BCE_1
	ld	a, (_NEWKEY + 0x0007)
	bit	4, a
	jr	NZ, BCE_1
;   return false;
	ld	l, 0x00
	jp	BCE_EXIT

BCE_1:
;chip8/byte_code_executor.c:58: fourthNibble = readFourthNibble;
	ld	bc, _currentInstruction
	ld	a, (_currentInstruction + 1)
	and	a, 0x0f
	ld	(_fourthNibble), a

;chip8/byte_code_executor.c:64: switch (firstNibble) {
	ld	a, (bc)

	rlca
	rlca
	rlca
	rlca
	and	a,0x0f

	ld	l, a
	ld	h, 0
	add	hl, hl
	add	hl, hl
	ld	de, BCE_FIRST_NIBBLE_TABLE
	add	hl, de
	jp	(hl)

BCE_FIRST_NIBBLE_TABLE:
	jp	BCE_0XXX
	db	0
	jp	l_executeSingleInstruction_00121
	db	0
	jp	l_executeSingleInstruction_00124
	db	0
	jp	l_executeSingleInstruction_00125
	db	0
	jp	l_executeSingleInstruction_00126
	db	0
	jp	BCE_5XXX
	db	0
	jp	l_executeSingleInstruction_00133
	db	0
	jp	l_executeSingleInstruction_00134
	db	0
	jp	l_executeSingleInstruction_00135
	db	0
	jp	l_executeSingleInstruction_00147
	db	0
	jp	l_executeSingleInstruction_00151
	db	0
	jp	l_executeSingleInstruction_00152
	db	0
	jp	l_executeSingleInstruction_00153
	db	0
	jp	l_executeSingleInstruction_00154
	db	0
	jp	l_executeSingleInstruction_00155
	db	0
	jp	l_executeSingleInstruction_00160

;chip8/byte_code_executor.c:65: case 0x0: {
BCE_0XXX:
;chip8/byte_code_executor.c:66: switch (highByte) {
	ld	a, (bc)
	or	a, a
	jp	NZ, BCE_BAD_INSTRUCTION

; BCE_00XX (PART 1):
;chip8/byte_code_executor.c:68: switch (lowByte) {
	ld	a, (_currentInstruction + 1)
	cp	a, 0xe0
	jr	Z, BCE_00E0
	cp	a,0xee
	jr	Z,l_executeSingleInstruction_00108
	cp	a,0xfb
	jr	Z,l_executeSingleInstruction_00111
	cp	a,0xfc
	jr	Z,l_executeSingleInstruction_00112
	inc	a
	jr	nz, BCE_00XX
BCE_00FF:
;chip8/byte_code_executor.c:70: videoHigh();
	call	_videoHigh
;chip8/byte_code_executor.c:71: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:73: case 0xE0:
BCE_00E0:
;chip8/byte_code_executor.c:74: cls();
	call	_cls
;chip8/byte_code_executor.c:75: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:77: case 0xEE:
l_executeSingleInstruction_00108:
;chip8/byte_code_executor.c:78: if (ret())
	call	_popPc
	ld	a, l
	or	a, a
	jp	Z, BCE_POST_PROCESS
;chip8/byte_code_executor.c:79: return false;
	ld	l,0x00
	jp	BCE_EXIT
;chip8/byte_code_executor.c:82: case 0xFB:
l_executeSingleInstruction_00111:
;chip8/byte_code_executor.c:83: scrlRight();
	call	_scrlRight
;chip8/byte_code_executor.c:84: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:86: case 0xFC:
l_executeSingleInstruction_00112:
;chip8/byte_code_executor.c:87: scrlLeft();
	call	_scrlLeft
;chip8/byte_code_executor.c:88: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:90: default:
BCE_00XX:
;chip8/byte_code_executor.c:91: switch (nibble3rd) {
	inc	bc
	ld	a, (bc)
	AND	A, 0xF0

	CP	A, 0xC0
	jr	Z, BCE_00CX

	CP	A, 0xD0
	jr	Z, BCE_00DX

	jp	BCE_BAD_INSTRUCTION

BCE_00CX:
	call	_scrlDown
	jp	BCE_POST_PROCESS

BCE_00DX:
	call	_scrlUp
	jp	BCE_POST_PROCESS

;chip8/byte_code_executor.c:113: case 0x1: {
l_executeSingleInstruction_00121:
;chip8/instr_pc.h:47: const uint16_t a = addr12Bit;
	ld	a, (_currentInstruction + 1)
	ld	l, a
	ld	h,0x00
	ld	a, (bc)
	and	a,0x0f
	ld	b, a
	ld	c,0x00
	add	hl, bc
	ex	de, hl
;chip8/instr_pc.h:48: if (a < 0x200) {
	ld	a, d
	sub	a,0x02
	jr	NC,l_executeSingleInstruction_00188
;chip8/instr_pc.h:49: printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
	exx
	push	hl
	exx
	pop	bc
	dec	bc
	dec	bc
	push	bc
	push	de
	ld	hl,___str_1
	push	hl
	call	_applicationExit
	ld	hl,6
	add	hl, sp
	ld	sp, hl
;chip8/byte_code_executor.c:50: bool executeSingleInstruction() {
	jr	l_executeSingleInstruction_00122
l_executeSingleInstruction_00188:
;chip8/instr_pc.h:53: chip8PC = (uint16_t *)a;
	push	de
	exx
	pop	hl
	exx
;chip8/byte_code_executor.c:114: if (jp())
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00122:
;chip8/byte_code_executor.c:115: return false;
	ld	l,0x00
	jp	BCE_EXIT
;chip8/byte_code_executor.c:119: case 0x2: {
l_executeSingleInstruction_00124:
;chip8/instr_pc.h:2: const uint16_t a = addr12Bit;
	ld	a, (_currentInstruction + 1)
	ld	l, a
	ld	h,0x00
	ld	a, (bc)
	and	a,0x0f
	ld	b, a
	ld	c,0x00
	add	hl, bc
	ex	de, hl
;chip8/instr_pc.h:4: if (a < 0x200) {
	ld	a, d
	sub	a,0x02
	jr	NC,l_executeSingleInstruction_00191
;chip8/instr_pc.h:5: printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
	exx
	push	hl
	exx
	pop	bc
	dec	bc
	dec	bc
	push	bc
	push	de		; Address to jump to
	ld	hl,___str_1
	push	hl
	call	_applicationExit
	ld	hl,6
	add	hl, sp
	ld	sp, hl
;chip8/byte_code_executor.c:6: #include "systimer.h"
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00191:
;chip8/instr_pc.h:9: pushPc();
	push	de
	call	_pushPc
;chip8/instr_pc.h:10: chip8PC = (uint16_t *)a;
	exx
	pop	hl
	exx
;chip8/byte_code_executor.c:121: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:124: case 0x3: {
l_executeSingleInstruction_00125:
;chip8/instr_pc.h:27: if (registers[nibble2nd] == lowByte)
	ld	e, c
	ld	d, b
	ld	a, (de)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	e, (hl)
	inc	bc
	ld	a, (bc)
	sub	a, e
	jp	NZ,BCE_POST_PROCESS
;chip8/instr_pc.h:28: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00194
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00194:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:126: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:129: case 0x4: {
l_executeSingleInstruction_00126:
;chip8/instr_pc.h:37: if (registers[nibble2nd] != lowByte)
	ld	e, c
	ld	d, b
	ld	a, (de)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	e, (hl)
	inc	bc
	ld	a, (bc)
	sub	a, e
	jp	Z,BCE_POST_PROCESS
;chip8/instr_pc.h:38: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00200
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx

	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00200:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:131: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:134: case 0x5: {

BCE_5XXX:
;chip8/byte_code_executor.c:136: switch (readFourthNibble) {
	ld	a, (_currentInstruction + 1)
	and	a,0x0f
	; or	a
	; jr	Z, BCE_5XX0
	; cp	2
	; jr	Z, BCE_5XX2
	; cp	3
	; jr	Z, BCE_5XX3
	; jp	BCE_BAD_INSTRUCTION

	ld	e, a
	ld	d,0x00
	ld	a, e
	or	a, a
	or	a, d
	jr	Z, BCE_5XX0
	ld	a, e
	sub	a,0x02
	or	a, d
	jr	Z, BCE_5XX2
	ld	a, e
	sub	a,0x03
	or	a, d
	jr	Z, BCE_5XX3
	jp	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:137: case 0x0:
BCE_5XX0:
;chip8/instr_pc.h:32: if (registers[nibble2nd] == registers[nibble3rd])
	ld	e, c
	ld	d, b
	ld	a, (de)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	e, (hl)
	inc	bc
	ld	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	ld	c, a
	ld	a,0x00
	inc	a
	ld	b, a
	ld	a, (bc)
	sub	a, e
	jp	NZ,BCE_POST_PROCESS
;chip8/instr_pc.h:33: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00206
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00206:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:139: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:141: case 0x2:
 BCE_5XX2:
;chip8/byte_code_executor.c:142: ldIVxVy();
	call	_ldIVxVy
;chip8/byte_code_executor.c:143: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:145: case 0x3:
 BCE_5XX3:
;chip8/byte_code_executor.c:146: ldVxVyI();
	call	_ldVxVyI
;chip8/byte_code_executor.c:147: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:155: case 0x6:
l_executeSingleInstruction_00133:
;chip8/byte_code_executor.c:156: ldVxByte();
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	d,0x00
	ld	e, a
	ld	a, d
	inc	a
	ld	d, a
	inc	bc
	ld	a, (bc)
	ld	(de), a
;chip8/byte_code_executor.c:157: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:159: case 0x7: {
l_executeSingleInstruction_00134:
;chip8/byte_code_executor.c:160: addVxByte();
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	d,0x00
	ld	e, a
	ld	a, d
	inc	a
	ld	d, a
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	a, (hl)
	ld	l, c
	ld	h, b
	inc	hl
	ld	c, (hl)
	add	a, c
	ld	(de), a
;chip8/byte_code_executor.c:161: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:164: case 0x8: {
l_executeSingleInstruction_00135:
;chip8/byte_code_executor.c:166: switch (readFourthNibble) {
	ld	a, (_currentInstruction + 1)
	and	a,0x0f
	ld	e, a
	ld	d,0x00
	ld	a, e
	or	a, a
	or	a, d
	jr	Z,l_executeSingleInstruction_00136
	ld	a, e
	dec	a
	or	a, d
	jr	Z,l_executeSingleInstruction_00137
	ld	a, e
	sub	a,0x02
	or	a, d
	jr	Z,l_executeSingleInstruction_00138
	ld	a, e
	sub	a,0x03
	or	a, d
	jr	Z,l_executeSingleInstruction_00139
	ld	a, e
	sub	a,0x04
	or	a, d
	jp	Z,l_executeSingleInstruction_00140
	ld	a, e
	sub	a,0x05
	or	a, d
	jp	Z,l_executeSingleInstruction_00141
	ld	a, e
	sub	a,0x06
	or	a, d
	jp	Z,l_executeSingleInstruction_00142
	ld	a, e
	sub	a,0x07
	or	a, d
	jp	Z,l_executeSingleInstruction_00143
	ld	a, e
	sub	a,0x0e
	or	a, d
	jp	Z,l_executeSingleInstruction_00144
	jp	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:167: case 0x0: {
l_executeSingleInstruction_00136:
;chip8/instr_registers.h:46: __endasm;
	ld	hl, (_currentInstruction)
	ld	a, l
	and	a, 0x0f
	ld	d,0x100 / 256
	ld	e, a
	ld	a, h
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	ld	c, a
	ld	b, 0x100 / 256
	ld	a, (bc)
	ld	(de), a
;chip8/byte_code_executor.c:169: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:172: case 0x1: {
l_executeSingleInstruction_00137:
;chip8/instr_registers.h:138: __endasm;
;
	ld	d, 0x100 / 256
	ld	a, (_currentInstruction)
	and	a, 0x0F
	ld	e, a
;
	ld	h, d
	ld	a, (_currentInstruction + 1)
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	ld	l, a
	ld	a, (de)
	or	a, (hl)
	ld	(de), a
;chip8/byte_code_executor.c:174: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:177: case 0x2: {
l_executeSingleInstruction_00138:
;chip8/instr_registers.h:111: __endasm;
;
	ld	d, 0x100 / 256
	ld	a, (_currentInstruction)
	and	a, 0x0F
	ld	e, a
;
	ld	h, d
	ld	a, (_currentInstruction + 1)
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	ld	l, a
	ld	a, (de)
	and	a, (hl)
	ld	(de), a
;chip8/byte_code_executor.c:179: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:182: case 0x3: {
l_executeSingleInstruction_00139:
;chip8/instr_registers.h:211: inline void xorVxVy() { registers[nibble2nd] ^= registers[nibble3rd]; }
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	d,0x00
	ld	e, a
	ld	a, d
	inc	a
	ld	d, a
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	a, (hl)
	ld	(ix-1),a
	inc	bc
	ld	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	ld	c, a
	ld	a,0x00
	inc	a
	ld	b, a
	ld	a, (bc)
	xor	a,(ix-1)
	ld	(de), a
;chip8/byte_code_executor.c:184: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:187: case 0x4: {
l_executeSingleInstruction_00140:
;chip8/byte_code_executor.c:188: addVxVy();
	call	_addVxVy
;chip8/byte_code_executor.c:189: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:192: case 0x5: {
l_executeSingleInstruction_00141:
;chip8/byte_code_executor.c:193: subVxVy();
	call	_subVxVy
;chip8/byte_code_executor.c:194: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:197: case 0x6: {
l_executeSingleInstruction_00142:
;chip8/instr_registers.h:143: uint8_t *register2ndNibble = &registers[nibble2nd];
	ld	a, (bc)
	and	a,0x0f
	ld	b,0x00
	ld	c, a
	ld	a, b
	inc	a
	ld	b, a
;chip8/instr_registers.h:144: registers[0xF] = *register2ndNibble & 0x1;
	ld	a, (bc)
	and	a,0x01
	ld	(0x010f),a
;chip8/instr_registers.h:145: *register2ndNibble >>= 1;
	ld	a, (bc)
	srl	a
	ld	(bc), a
;chip8/byte_code_executor.c:199: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:202: case 0x7:
l_executeSingleInstruction_00143:
;chip8/byte_code_executor.c:203: subnVxVy();
	call	_subnVxVy
;chip8/byte_code_executor.c:204: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:206: case 0xE: {
l_executeSingleInstruction_00144:
;chip8/instr_registers.h:149: uint8_t *register2ndNibble = &registers[nibble2nd];
	ld	a, (bc)
	and	a,0x0f
	ld	b,0x00
	ld	c, a
	ld	a, b
	inc	a
	ld	b, a
;chip8/instr_registers.h:151: registers[0xF] = !!(*register2ndNibble & 0x80);
	ld	a, (bc)
	rlca
	and	a,0x01
	xor	a,0x01
	xor	a,0x01
	ld	(0x010f),a
;chip8/instr_registers.h:152: *register2ndNibble <<= 1;
	ld	a, (bc)
	add	a, a
	ld	(bc), a
;chip8/byte_code_executor.c:208: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:217: case 0x9: {
l_executeSingleInstruction_00147:
;chip8/byte_code_executor.c:218: if (readFourthNibble == 0)
	ld	a, (_currentInstruction + 1)
	and	a,0x0f
	jp	NZ,BCE_BAD_INSTRUCTION
;chip8/instr_pc.h:42: if (registers[nibble2nd] != registers[nibble3rd])
	ld	e, c
	ld	d, b
	ld	a, (de)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	e, (hl)
	inc	bc
	ld	a, (bc)
	rlca
	rlca
	rlca
	rlca
	and	a,0x0f
	ld	c, a
	ld	a,0x00
	inc	a
	ld	b, a
	ld	a, (bc)
	sub	a, e
	jp	Z,BCE_POST_PROCESS
;chip8/instr_pc.h:43: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00218
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00218:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:219: sneVxVy();
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:225: case 0xA:
l_executeSingleInstruction_00151:
;chip8/byte_code_executor.c:226: ldIAddr();
	ld	a, (_currentInstruction + 1)
	ld	e, a
	ld	d,0x00
	ld	a, (bc)
	and	a,0x0f
	ld	b, a
	ld	c,0x00
	ld	a, e
	ld	hl,_registerI
	add	a, c
	ld	(hl), a
	ld	a, d
	adc	a, b
	inc	hl
	ld	(hl), a
;chip8/byte_code_executor.c:229: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:231: case 0xB:
l_executeSingleInstruction_00152:
;chip8/instr_pc.h:58: chip8PC = (uint16_t *)(addr12Bit + registers[0]);
	ld	a, (_currentInstruction + 1)
	ld	l, a
	ld	h,0x00
	ld	e, c
	ld	d, b
	ld	a, (de)
	and	a,0x0f
	ld	d, a
	ld	e,0x00
	add	hl, de
	ex	de, hl
	ld	hl,0x0100
	ld	l, (hl)
	ld	h,0x00
	add	hl, de
	push	hl
	exx
	pop	hl
	exx
;chip8/instr_pc.h:60: if ((uint16_t)chip8PC < 0x200) {
	exx
	ld	a, h
	exx
	sub	a,0x02
	jp	NC, BCE_POST_PROCESS
;chip8/instr_pc.h:61: printf("Illegal jump to %04X at %p\r\n", addr12Bit, chip8PC - 1);
	exx
	push	hl
	exx
	pop	de
	dec	de
	dec	de
	ld	a, (_currentInstruction + 1)
	ld	l, a
	ld	h,0x00
	ld	a, (bc)
	and	a,0x0f
	ld	c, a
	ld	a,0x00
	ld	b, c
	ld	c, a
	add	hl, bc
	ld	bc,___str_1+0
	push	de
	push	hl
	push	bc
	call	_applicationExit
	ld	hl,6
	add	hl, sp
	ld	sp, hl
;chip8/byte_code_executor.c:62: #endif
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:235: case 0xC: {
l_executeSingleInstruction_00153:
;chip8/byte_code_executor.c:236: rnd();
	ld	l, c
	ld	h, b
	ld	a, (hl)
	and	a,0x0f
	ld	d,0x00
	ld	e, a
	ld	a, d
	inc	a
	ld	d, a
	push	bc
	push	de
	call	_chip8Rand
	ld	a, l
	pop	de
	pop	hl
	inc	hl
	ld	c, (hl)
	and	a, c
	ld	(de), a
;chip8/byte_code_executor.c:237: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:240: case 0xD: {
l_executeSingleInstruction_00154:
;chip8/byte_code_executor.c:241: draw();
	call	_draw
;chip8/byte_code_executor.c:242: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:245: case 0xE: {
l_executeSingleInstruction_00155:
;chip8/byte_code_executor.c:246: switch (lowByte) {
	ld	a, (_currentInstruction + 1)
	cp	a,0x9e
	jr	Z,l_executeSingleInstruction_00156
	sub	a,0xa1
	jr	Z,l_executeSingleInstruction_00157
	jp	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:247: case 0x9E: {
l_executeSingleInstruction_00156:
;chip8/instr_pc.h:69: if (isKeyDown(registers[nibble2nd]))
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	l, (hl)
	call	_isKeyDown
	ld	a, l
	or	a, a
	jp	Z, BCE_POST_PROCESS
;chip8/instr_pc.h:70: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00227
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx

	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00227:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:249: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:252: case 0xA1: {
l_executeSingleInstruction_00157:
;chip8/instr_pc.h:74: if (!isKeyDown(registers[nibble2nd]))
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	l, (hl)
	call	_isKeyDown
	ld	a, l
	or	a, a
	jp	NZ, BCE_POST_PROCESS
;chip8/instr_pc.h:75: skipNextInstruction();
	exx
	push	hl
	exx
	pop	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	a, c
	sub	a,0xf0
	or	a, b
	jr	NZ,l_executeSingleInstruction_00233
	exx
	inc	hl
	inc	hl
	inc	hl
	inc	hl
	exx
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00233:
	exx
	inc	hl
	inc	hl
	exx
;chip8/byte_code_executor.c:254: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:264: case 0xF: {
l_executeSingleInstruction_00160:
;chip8/byte_code_executor.c:265: switch (lowByte) {
	ld	a, (_currentInstruction + 1)
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
	jp	Z,l_executeSingleInstruction_00172
	cp	a,0x18
	jp	Z,l_executeSingleInstruction_00173
	cp	a,0x1e
	jp	Z,l_executeSingleInstruction_00174
	cp	a,0x29
	jp	Z,l_executeSingleInstruction_00175
	cp	a,0x33
	jp	Z,l_executeSingleInstruction_00176
	cp	a,0x55
	jp	Z,l_executeSingleInstruction_00177
	sub	a,0x65
	jp	Z,l_executeSingleInstruction_00178
	jp	BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:267: case 0x00:
l_executeSingleInstruction_00161:
;chip8/byte_code_executor.c:268: if (highByte == 0xF0)
	ld	a, (bc)
	sub	a,0xf0
	jp	NZ,BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:270: ldILargeAddr();
	exx
	ld	d, (hl)
	inc	hl
	ld	e, (hl)
	inc	hl
	ld	(_registerI), de
	exx
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:275: case 0x01: {
l_executeSingleInstruction_00165:
;chip8/byte_code_executor.c:276: videoPlane();
	ld	a, (bc)
	and	a,0x0f
	ld	(__color+0), a
;chip8/byte_code_executor.c:277: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:280: case 0x02:
l_executeSingleInstruction_00166:
;chip8/byte_code_executor.c:281: if (highByte == 0xF0)
	ld	a, (bc)
	sub	a,0xf0
	jp	NZ,BCE_BAD_INSTRUCTION
;chip8/byte_code_executor.c:282: audio();
	call	_audio
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:287: case 0x07: {
l_executeSingleInstruction_00170:
;chip8/byte_code_executor.c:288: ldVxDt();
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	a,(_delayTimer + 0)
	ld	(hl), a
;chip8/byte_code_executor.c:289: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:292: case 0x0A: {
l_executeSingleInstruction_00171:
;chip8/instr_pc.h:79: const uint8_t b = currentKey();
	push	bc
	call	_currentKey
	ld	a, l
	pop	bc
;chip8/instr_pc.h:80: if (b != 255) {
	ld	e,a
	inc	a
	jr	Z,l_executeSingleInstruction_00239
;chip8/instr_pc.h:81: registers[nibble2nd] = b;
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	(hl), e
;chip8/byte_code_executor.c:82: case 0xFB:
	jp	BCE_POST_PROCESS
l_executeSingleInstruction_00239:
;chip8/instr_pc.h:84: chip8PC -= 1;
	exx
	dec	hl
	dec	hl
	exx
;chip8/byte_code_executor.c:294: break;
	jp	BCE_POST_PROCESS
;chip8/byte_code_executor.c:297: case 0x15: {
l_executeSingleInstruction_00172:
;chip8/byte_code_executor.c:298: ldDtVx();
	push	bc
	call	_initTimers
	pop	bc
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	a, (hl)
	ld	(_delayTimer+0), a
;chip8/byte_code_executor.c:299: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:302: case 0x18: {
l_executeSingleInstruction_00173:
;chip8/byte_code_executor.c:303: ldStVx();
	call	_ldStVx
;chip8/byte_code_executor.c:304: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:307: case 0x1E: {
l_executeSingleInstruction_00174:
;chip8/byte_code_executor.c:308: addIVx();
	ld	a, (bc)
	and	a,0x0f
	ld	l, a
	ld	a,0x00
	inc	a
	ld	h, a
	ld	c, (hl)
	ld	b,0x00
	ld	hl,_registerI
	ld	a, (hl)
	add	a, c
	ld	(hl), a
	inc	hl
	ld	a, (hl)
	adc	a, b
	ld	(hl), a
;chip8/byte_code_executor.c:309: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:312: case 0x29: {
l_executeSingleInstruction_00175:
;chip8/byte_code_executor.c:313: ldfIVx();
	call	_ldfIVx
;chip8/byte_code_executor.c:314: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:317: case 0x33: {
l_executeSingleInstruction_00176:
;chip8/byte_code_executor.c:318: bcdIVx();
	call	_bcdIVx
;chip8/byte_code_executor.c:319: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:322: case 0x55: {
l_executeSingleInstruction_00177:
;chip8/instr_registers.h:82: __endasm;
	ld	de, (_registerI)
	ld	hl, 0x100
	ld	a, (_currentInstruction)
	and	a, 0x0F
	inc	a
	ld	c, a
	ld	b, 0
	ldir
;chip8/byte_code_executor.c:324: break;
	jr	BCE_POST_PROCESS
;chip8/byte_code_executor.c:327: case 0x65: {
l_executeSingleInstruction_00178:
;chip8/instr_registers.h:64: __endasm;
	ld	hl, (_registerI)
	ld	de, 0x100
	ld	a, (_currentInstruction)
	and	a, 0x0F
	inc	a
	ld	c, a
	ld	b, 0
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
	ld	hl, (_currentInstruction)
	call	_invertByteOrder
	pop	de
	ld	bc,___str_2+0
	push	de
	push	hl
	push	bc
	call	_applicationExit		; ONLY RETURNS WHEN RUNNING UNDER TESTS
	ld	hl,6
	add	hl, sp
	ld	sp, hl
;chip8/byte_code_executor.c:341: return false;
	ld	l,0x00
	jr	BCE_EXIT
;chip8/byte_code_executor.c:342: }

BCE_POST_PROCESS:
;chip8/byte_code_executor.c:344: if ((uint16_t)chip8PC < 0x200) {
	exx
	ld	a, h
	exx
	sub	a, 0x02
	jr	NC, BCE_EXIT_ERROR
;chip8/byte_code_executor.c:345: printf("PC counter below 0x200 - %p\r\n", chip8PC);
	exx
	push	hl
	exx
	pop	hl
	push	hl
	ld	hl,___str_3
	push	hl
	call	_applicationExit
	pop	af
	pop	af
;chip8/byte_code_executor.c:346: return false;
	ld	l,0x00
	jr	BCE_EXIT
BCE_EXIT_ERROR:
;chip8/byte_code_executor.c:349: return true;
	ld	l, 1
BCE_EXIT:
;chip8/byte_code_executor.c:350: }
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
