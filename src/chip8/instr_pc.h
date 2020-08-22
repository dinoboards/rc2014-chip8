#define call() \
  pushPc();    \
  chip8PC = (uint16_t *)addr

#define ret() popPc()

#define seVxByte()                        \
  if (registers[secondNibble] == lowByte) \
  chip8PC += 1

#define seVxVy()                                         \
  if (registers[secondNibble] == registers[thirdNibble]) \
  chip8PC += 1

#define sneVxByte()                       \
  if (registers[secondNibble] != lowByte) \
  chip8PC += 1

#define sneVxVy()                                        \
  if (registers[secondNibble] != registers[thirdNibble]) \
  chip8PC += 1

#define jp() chip8PC = (uint16_t *)addr;

#define skpVx()                                                   \
  if (keyPressed && registers[secondNibble] == currentPressedKey) \
    chip8PC += 1;

#ifdef DIAGNOSTICS_ON
#define sknpVx()                                                   \
  if (!keyPressed || registers[secondNibble] != currentPressedKey) \
    chip8PC += 1;                                                  \
  else                                                             \
    startCounting = true

#else
#define sknpVx()                                                   \
  if (!keyPressed || registers[secondNibble] != currentPressedKey) \
    chip8PC += 1;
#endif

#define andVxVy() registers[secondNibble] &= registers[thirdNibble];
