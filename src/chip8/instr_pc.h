inline uint8_t call(void) {
  const uint16_t a = addr12Bit;

  if (a < 0x200) {
    printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
    return true;
  }

  pushPc();
  chip8PC = (uint16_t *)a;

  return false;
}

#define ret() popPc()

#define skipNextInstruction()                 \
  {                                           \
    if (*chip8PC == LOAD_I_LARGE_INSTRUCTION) \
      chip8PC += 2;                           \
    else                                      \
      chip8PC += 1;                           \
  }

inline void seVxByte(void) {
  // printf("  seVxByte(%d, %d) R[x]=%d", nibble2nd, lowByte, registers[nibble2nd]);
  if (registers[nibble2nd] == lowByte)
    skipNextInstruction();
}

inline void seVxVy(void) {
  if (registers[nibble2nd] == registers[nibble3rd])
    skipNextInstruction();
}

inline void sneVxByte(void) {
  if (registers[nibble2nd] != lowByte)
    skipNextInstruction();
}

inline void sneVxVy(void) {
  if (registers[nibble2nd] != registers[nibble3rd])
    skipNextInstruction();
}

inline uint8_t jp(void) {
  const uint16_t a = addr12Bit;
  if (a < 0x200) {
    printf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
    return true;
  }

  chip8PC = (uint16_t *)a;
  return false;
}

inline uint8_t jpV0Addr(void) {
  chip8PC = (uint16_t *)(addr12Bit + registers[0]);

  if ((uint16_t)chip8PC < 0x200) {
    printf("Illegal jump to %04X at %p\r\n", addr12Bit, chip8PC - 1);
    return true;
  }

  return false;
}

inline void skpVx(void) {
  if (isKeyDown(registers[nibble2nd]))
    skipNextInstruction();
}

inline void sknpVx(void) {
  if (!isKeyDown(registers[nibble2nd]))
    skipNextInstruction();
}

inline void keyVx(void) {
  const uint8_t b = currentKey();
  if (b != 255) {
    registers[nibble2nd] = b;
    return;
  }
  chip8PC -= 1;
}
