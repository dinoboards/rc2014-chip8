# Chip-8 assembler and interpeter for RC2014

[![Build Status](https://travis-ci.com/vipoo/rc2014-chip8.svg?branch=master)](https://travis-ci.com/vipoo/rc2014-chip8)

Chip-8 is a byte code interpreter to execute small games and graphic demos.  Originally designed for early 8 bit computer, its been ported to many platforms.  See here for more information of [Chip 8](https://en.wikipedia.org/wiki/CHIP-8)

This is an implementation for the [RC2014 retro computer kit](https://rc2014.co.uk/), paired with the [TMS9918 Video chip)(https://github.com/jblang/TMS9918A).

## Running on the z80

Copy the pre-built released units, and install them on your RC2014.  Also grab a Chip-8 binaries (search the net to find em).

The chip8.com file is designed to be started from CPM (any version).

> Remeber it does require RomWBW hbios and the TMS graphics module (lores only) or the [V9958 module](https://www.tindie.com/products/dinotron/v9958-msx-video-board-for-rc2014/) (highres)

To run a prebuilt ROM, on your RC2014:

`chip8 invaders`

## Prerequisites

* z88dk: You can manual install as per the instruction at [z88dk](https://github.com/z88dk/z88dk/wiki). Or as per the `.travis.yml`, grab the compiler binaries.
* nodejs: see `.nvmrc` for specific version required.
* gpp

> **z88dk Issue**

> At the moment, the latest release of z88dk includes and patches a version of SDCC that has a bug.  You need to adjust the SDCC version number before building z88dk.
> In the Makefile within the z88dk project, adjust the SDCC_VERSION as below:

> `SDCC_VERSION    = 12085 # 4.1.0 Release`

To run the tests, you also need:
* cpm (https://github.com/jhallen/cpm or see travel.yml for pre-built linux binary)


For running, you need a RC2014 compatible Z80 based kit, with a Video output module.

### Building

To cross compile the binaries on a linux machine:

`make all`

To run the test suites

`make test`

## chip8asm - assembler

`chip8asm.com mygame.cas`

## chip8 - interpreter

This is the main interpreter for running a CHIP-8 game rom.

Command Line Options:
```
chip8 <gamefile> [-X <number>]

  -X <number>   Same speed adjuster - larger numbers slower game

```

CTRL+Z to exit the game.

Example:

`chip8 mygame`

## Current Status:
* All standard original chip8 instructions implemented.
* Most of the Extended and Octo instructions implemented.
* Still need to enable ability to activate various quirks modes.
* Improve general stability.
* See [todo.md](todo.md) for my scratch notes on whats next to work on.

## Hardware

As part of the hardware development I have been undertaking to develop MSX compatible modules for the RC2014 platform, I have used this chip8 port to test and explore the V9958 and other MSX modules I have been designing:
* [V9958 video board](https://www.tindie.com/products/dinotron/v9958-msx-video-board-for-rc2014/)
* [MSX compatible GAME card](https://github.com/vipoo/rc2014-game)

### Video Output devices

I have tested this code on my RC2014 system with a [TMS9918 video board](https://github.com/jblang/TMS9918A) and the [V9958 video board](https://www.tindie.com/products/dinotron/v9958-msx-video-board-for-rc2014/).

Chip8 represents the screen as a simple bitmap (64x32) or (128x64) pixels. Due to the way the TMS9918A chip works, its not actually able to generate the hires image.  The V9958 is more than capable of generating the required resolution/colours.

### Audio Output

Chip8 only supported a simple 'buzzer' sound output.  The Octo extended this with a [16 byte sample system](https://github.com/JohnEarnest/Octo/blob/gh-pages/docs/XO-ChipSpecification.md#audio). This extended instruction
has been mapped to an approximation sound with a YM2149 audio chip.  I have tested with the [MSX compatible GAME card](https://github.com/vipoo/rc2014-game) module, but I would expect it to work with the [YM2149/AY-3-8190 Module](https://github.com/electrified/rc2014-ym2149), if configured with MSX ports.

### Input devices

The code supports accepting inputs from the standard RomWBW (HBIOS) serial interface.  I have tested using the SIO/2 module - but I would expect it to work with any serial device with an appropriate hbios driver enabled.

The system also support game pad control using the [MSX compatible GAME card](https://github.com/vipoo/rc2014-game).

### Included chip binaries

* INVADERS.CH8:  An original lores space invaders like game.  Controls: LEFT (A), RIGHT (D), FIRE (SPACE)

* SUPER.CH8: A hires game using the extended instructions. Controls: LEFT (A), RIGHT (D), FIRE (SPACE).  (https://internet-janitor.itch.io/super-neatboy)

* CHICK.CH8: A hires game using the extended instructions. Controls: LEFT (A), RIGHT (D), UP (W), DOWN (S), EAT (SPACE), CALL (M). (https://internet-janitor.itch.io/chicken-scratch)

* F8Z.CH8: A lores demo. Controls LEFT(A), RIGHT (D), ROTATE (K and L).  (https://github.com/JohnEarnest/Octo)

### Configuration Files

Chip8 was originally designed for systems that only had a HEX keypad and screen colours were unspecified.  For this port, the keys and colours can be configured for each game, with a specific configuration file.

For example, the change the colours or key mappings for INVADERS.CH8, you would create a file called INVADERS.CFG

See the test-samples for some examples of colour and key mappings.



### See Also

https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set     a  sdddddddddddddddddd

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

https://github.com/JohnEarnest/Octo
