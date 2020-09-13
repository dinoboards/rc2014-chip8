#define call() \
  pushPc();    \
  chip8PC = (uint16_t *)addr

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

#define jp() chip8PC = (uint16_t *)addr;

inline void jpV0Addr() { chip8PC = (uint16_t *)(addr + registers[0]); }

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
