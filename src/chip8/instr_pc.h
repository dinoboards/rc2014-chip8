#define call() \
  pushPc();    \
  chip8PC = (uint16_t *)addr

#define ret() popPc()

#define seVxByte()                        \
  if (registers[secondNibble] == lowByte) \
  chip8PC += 1
