inline uint8_t call() {
  const uint16_t a = addr;

  if (a < 0x200) {
    xprintf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
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

inline void seVxByte() {
  if (registers[nibble2nd] == lowByte)
    skipNextInstruction();
}

inline void seVxVy() {
  if (registers[nibble2nd] == registers[nibble3rd])
    skipNextInstruction();
}

inline void sneVxByte() {
  if (registers[nibble2nd] != lowByte)
    skipNextInstruction();
}

inline void sneVxVy() {
  if (registers[nibble2nd] != registers[nibble3rd])
    skipNextInstruction();
}

inline uint8_t jp() {
  const uint16_t a = addr;
  if (a < 0x200) {
    xprintf("Illegal jump to %04X at %p\r\n", a, chip8PC - 1);
    return true;
  }

  chip8PC = (uint16_t *)a;
  return false;
}

inline uint8_t jpV0Addr() {
  chip8PC = (uint16_t *)(addr + registers[0]);

  if ((uint16_t)chip8PC < 0x200) {
    xprintf("Illegal jump to %04X at %p\r\n", addr, chip8PC - 1);
    return true;
  }

  return false;
}

inline void skpVx() {
  if (isKeyDown(registers[nibble2nd]))
    skipNextInstruction();
}

inline void sknpVx() {
  if (!isKeyDown(registers[nibble2nd]))
    skipNextInstruction();
}

inline void keyVx() {
  const uint8_t b = currentKey();
  if (b != 255) {
    registers[nibble2nd] = b;
    return;
  }
  chip8PC -= 1;
}
