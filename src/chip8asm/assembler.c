// #include "charconstants.h"
#include "assembler.h"
#include "datatypes.h"
#include "emitters.h"
#include "error_reports.h"
#include "labels.h"
#include "systemstate.h"
#include "token_parser.h"
#include "tokenreader.h"
#include "xstdio.h"

#define getNext() getNextToken()

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
--------------
Fx07 - LD Vx, DT
Set Vx = delay timer value.

The value of DT is placed into Vx.
-------------
Fx65 - LD Vx, [I]
Read registers V0 through Vx from memory starting at location I.

The interpreter reads values from memory starting at location I into registers V0 through Vx.
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
  } else if (currentIsDT()) {
    expectToBeDT();

    emit2Nibble(0xF, x);
    emitByte(0x07);
    return;
  } else if (currentIsIndexedI()) {
    expectToBeIndexedI();

    emit2Nibble(0xF, x);
    emitByte(0x65);
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

/*
Fx18 - LD ST, Vx
Set sound timer = Vx.

ST is set equal to the value of Vx.
*/
inline static void assLdStVx() {
  expectToBeST();

  getNext();
  expectToBeComma();

  getNext();
  const int x = expectToBeVRegister();

  emit2Nibble(0xF, x);
  emitByte(0x18);
}

/*
Fx15 - LD DT, Vx
Set delay timer = Vx.

DT is set equal to the value of Vx.
*/
inline static void assLdDtVx() {
  expectToBeDT();

  getNext();
  expectToBeComma();

  getNext();
  const int x = expectToBeVRegister();

  emit2Nibble(0xF, x);
  emitByte(0x15);
}
/*
  Ld Vx, Vy
  Ld Vx, <byte>
  ld Vx, [i]
  LD Vx, DT
  Ld I, <addr>
  ld st, Vx
*/
inline static void assLd() {
  getNext();
  expectToBeOneOfVxOrIOrStOrDt();

  if (currentIsIRegister())
    assLdI();
  else if (currentIsST())
    assLdStVx();
  else if (currentIsDT())
    assLdDtVx();
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
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte b = expectToBeByte();

  emit2Nibble(0x7, x);
  emitByte(b);
}

/*Fx1E - ADD I, Vx
Set I = I + Vx.

The values of I and Vx are added, and the results are stored in I.
*/
inline static void assAddI() {
  expectToBeIRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte x = expectToBeVRegister();

  emit2Nibble(0xF, x);
  emitByte(0x1E);
}

inline static void assAdd() {
  getNext();
  expectToBeVxOrIRegister();

  if (currentIsIRegister())
    assAddI();
  else
    assAddVx();
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
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
--------
9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2..
*/
inline static void assSne() {
  getNext();
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  if (currentIsVRegister()) {
    const byte y = expectToBeVRegister();
    emit2Nibble(0x9, x);
    emit2Nibble(y, 0);

    return;
  }

  const byte b = expectToBeByte();

  emit2Nibble(0x4, x);
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

/*
ExA1 - SKNP Vx
Skip next instruction if key with the value of Vx is not pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2
*/
inline static void assSknpVx() {
  getNext();
  const byte x = expectToBeVRegister();

  emit2Nibble(0xE, x);
  emitByte(0xA1);
}

/*
Cxkk - RND Vx, byte
Set Vx = random byte AND kk.

The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
*/
inline static void assRndVx() {
  getNext();
  const byte x = expectToBeVRegister();

  getNext();
  expectToBeComma();

  getNext();
  const byte b = expectToBeByte();

  emit2Nibble(0xC, x);
  emitByte(b);
}

void assemble(byte pc) __z88dk_fastcall {
  parseCount = pc;
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
      assAdd();
      break;

    case InstructionSe:
      assSe();
      break;

    case InstructionSne:
      assSne();
      break;

    case InstructionJp:
      assJp();
      break;

    case InstructionSkp:
      assSkpVx();
      break;

    case InstructionSknp:
      assSknpVx();
      break;

    case InstructionRnd:
      assRndVx();
      break;

    default:
      return expectedError("Instruction");
    }

    // xtracef("%02X: %d : '%s'\r\n", (int)token.terminatorChar, (int)token.type, token.value);
    getNextToken();
  }

  closeTokenStream();
}
