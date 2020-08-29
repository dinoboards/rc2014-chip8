# Chip-8 assembler and interpeter for RC2014

![Build Status](https://travis-ci.org/vipoo/rc2014-chip8.svg?branch=master)

Chip-8 is a byte code interpreter to execute small games and graphic demos.  Originally designed for ealry 8 bit computer, its been ported to many platforms.  For more information of [Chip 8](https://en.wikipedia.org/wiki/CHIP-8)

This is an implementation for the [RC2014 retro computer kit](https://rc2014.co.uk/), paired with the [TMS9918 Video chip)(https://github.com/jblang/TMS9918A).

It is a work in progress, so at this point in time, does not implement all required byte code.  So far the INVADERS image has been tested and implemented.

## Running on the z80

Copy the pre-built released units, and install them on your RC2014.  Also grab a Chip-8 binary rom (search the net to find em).

The chip8.com file is designed to be started from CPM (any version).

> Remeber it does require RomWBW hbios and the TMS graphics card.

To run a prebuilt ROM, on your RC2014:

`chip8 invaders`

## Prerequisites

For building, you need the z88dk cross compiler.  You can manual install as per the instruction at [z88dk](https://github.com/z88dk/z88dk/wiki).
Or as per the `.travis.yml`, grab the compiler from the [pre-built units](https://github.com/vipoo/z88dk/releases/download/v2.0.0-dino.2/z88dk-v2.0.0-dino.2.tar.gz)

For testing, you need a custom build of the cpm emulator.
Can be found at [custom-cpm-emulator](https://github.com/vipoo/cpm/releases/download/v0.0.6/cpm-v0.0.6.tar.gz)

### Building

To cross compile the binaries on a linux machine:

`make all`

To run the test suites

`make test`

## chip8asm - assembler

`chip8asm.com mygame.src`

- goal to procude a mygame.ch8 binary

## chip8 - interpreter

This is the main interpreter for running a CHIP-8 game rom.


Command Line Options:
```
chip8 <gamefile> [-T|--TMS] [-S|--SERIAL] [-X <number>]

  -T, --TMS     Run game on a TMS (requires TMS card and HBIOS support)
  -S, --SERIAL  (default) Run game on serial.
  -X <number>   Same speed adjuster - larger numbers slower game

```

Example:

`chip8 mygame.ch8`

### See Also

https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
