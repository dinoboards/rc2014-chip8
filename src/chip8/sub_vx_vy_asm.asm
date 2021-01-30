	PUBLIC	_subVxVy
	EXTERN	_currentInstruction
	SECTION	CODE

_subVxVy:
;chip8/instr_registers.h:197: register2ndNibble = &registers[nibble2nd];
	ld	a, (_currentInstruction + 0)
	and	a,0x0f
	ld	l, a
	ld	h,0x01

;chip8/instr_registers.h:198: const uint8_t *register3rdNibble = &registers[nibble3rd];
	ld	a, (_currentInstruction + 1)
	rlca
	rlca
	rlca
	rlca
	and	a, 0x0f
	ld	c, a
	ld	b, 0x01

;chip8/instr_registers.h:202: const byte f = *register2ndNibble >= *register3rdNibble;
	ex	de, hl
	ld	a, (de)
	ld	l, a
	ld	a, (bc)
	ld	c, a
	ld	a, l
	sub	a, c
	ld	a,0x00
	rla
	xor	a,0x01
	ld	b, a

;chip8/instr_registers.h:203: *register2ndNibble -= *register3rdNibble;
	ld	a, l
	sub	a, c
	ld	(de), a

;chip8/instr_registers.h:204: registers[0xF] = f;
	ld	hl,0x010f
	ld	(hl), b

;chip8/instr_registers.h:205: }
	ret

