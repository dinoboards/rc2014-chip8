// #include "charconstants.h"
#include "assembler.h"
#include "chip8asm/emitters.h"
#include "chip8asm/error_reports.h"
#include "chip8asm/systemstate.h"
#include "chip8asm/token_parser.h"
#include "chip8asm/tokenreader.h"
#include "datatypes.h"
#include "labels.h"
#include "xstdio.h"

inline static void assLabel(int parseCount) { addLabel(token.value, currentAddress, parseCount != 1); }

inline static void assRet() { emit(0x00EE); }

/*
6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.
--------------
8xy0 - LD Vx, Vy
Set Vx = Vy.

Stores the value of register Vy in register Vx.
*/
inline static void assLdVx() {
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();

  if (currentIsVRegister()) {
    const byte y = expectToBeVRegister();

    emitNibbles(0x8, x, y, 0);
    return;
  }

  const byte data = expectToBeByte();

  emit((0x6000 | (((int)x) << 8)) | data);
}

/*
Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.
*/
inline static void assLdI() {
  expectToBeIRegister();

  getNext();
  expectToBeComma();

  getNext();
  const int x = expectToBeInt();

  emit(0xA000 | x);
}

inline static void assLd() {
  getNext();

  if (currentIsIRegister())
    assLdI();
  else
    assLdVx();
}

inline static void assDb() {
  getNext();
  int  x = expectToBeByte();
  bool isMore;

  do {
    emitByte(x);

    isMore = token.terminatorChar == ',';
    if (isMore) {
      getNext();
      expectToBeComma();
      getNext();
      x = expectToBeByte();
    }

  } while (isMore);
}

// Dxyn - DRW Vx, Vy, nibble
inline static void assDrw() {
  getNext();
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte y = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte nibble = expectToBeNibble();

  emitNibbles(0x0D, x, y, nibble);
}

/*
2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
*/
inline static void assCall() {
  getNext();
  const int16_t addr = expectToBeInt();

  int b = (0x2000 | addr);

  emit(b);
}

/*
00E0 - CLS
Clear the display.
*/
inline static void assCls() { emit(0x00E0); }

/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.
*/
inline static void assAddVx() {
  getNext();
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte b = expectToBeByte();

  emit2Nibble(0x7, x);
  emitByte(b);
}

/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
--------
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
*/
inline static void assSe() {
  getNext();
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  if (currentIsVRegister()) {
    const byte y = expectToBeVRegister();
    emit2Nibble(0x5, x);
    emit2Nibble(y, 0);

    return;
  }

  const byte b = expectToBeByte();

  emit2Nibble(0x3, x);
  emitByte(b);
}

/*
1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.
*/
inline static void assJp() {
  getNext();
  const uint16_t addr = expectToBeInt();

  emit(0x1000 | addr);
}

/*
Ex9E - SKP Vx
Skip next instruction if key with the value of Vx is pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.*/
inline static void assSkpVx() {
  getNext();
  const byte x = expectToBeVRegister();

  emit2Nibble(0xE, x);
  emitByte(0x9E);
}

void assemble(int parseCount) __z88dk_fastcall {
  currentAddress = 0x200;
  programPtr = programStorage;

  openTokenStream();

  getNextToken();

  while (token.type != TokenEnd) {
    switch (token.type) {
    case TokenLabel:
      assLabel(parseCount);
      break;

    case InstructionCls:
      assCls();
      break;

    case InstructionRet:
      assRet();
      break;

    case InstructionLd:
      assLd();
      break;

    case InstructionDb:
      assDb();
      break;

    case InstructionDrw:
      assDrw();
      break;

    case InstructionCall:
      assCall();
      break;

    case InstructionAdd:
      assAddVx();
      break;

    case InstructionSe:
      assSe();
      break;

    case InstructionJp:
      assJp();
      break;

    case InstructionSkp:
      assSkpVx();
      break;

    default:
      return expectedError("Instruction");
    }

    // xtracef("%02X: %d : '%s'\r\n", (int)token.terminatorChar, (int)token.type, token.value);
    getNextToken();
  }

  closeTokenStream();
}
