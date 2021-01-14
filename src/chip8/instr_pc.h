inline uint8_t call() {
  if (addr < 0x200) {
    xprintf("Illegal jump to %04X at %p\r\n", addr, chip8PC - 1);
    return true;
  }

  pushPc();
  chip8PC = (uint16_t *)addr;

  return false;
}

#define ret() popPc()

#define seVxByte()                     \
  if (registers[nibble2nd] == lowByte) \
  chip8PC += 1

#define seVxVy()                                    \
  if (registers[nibble2nd] == registers[nibble3rd]) \
  chip8PC += 1

#define sneVxByte()                    \
  if (registers[nibble2nd] != lowByte) \
  chip8PC += 1

#define sneVxVy()                                   \
  if (registers[nibble2nd] != registers[nibble3rd]) \
  chip8PC += 1

inline uint8_t jp() {
  if (addr < 0x200) {
    xprintf("Illegal jump to %04X at %p\r\n", addr, chip8PC - 1);
    return true;
  }

  chip8PC = (uint16_t *)addr;
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

#define skpVx()                                                \
  if (keyPressed && registers[nibble2nd] == currentPressedKey) \
    chip8PC += 1;

#ifdef DIAGNOSTICS_ON
#define sknpVx()                                                \
  if (!keyPressed || registers[nibble2nd] != currentPressedKey) \
    chip8PC += 1;                                               \
  else                                                          \
    startCounting = true

#else
#define sknpVx()                                                \
  if (!keyPressed || registers[nibble2nd] != currentPressedKey) \
    chip8PC += 1;
#endif

inline void keyVx() {
  if (keyPressed) {
    registers[nibble2nd] = currentPressedKey;
    return;
  }
  chip8PC -= 1;
}
