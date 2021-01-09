1. ./ load an optional config file (eg: invaders.cfg) to control colour mapping
2. update config file to control quirks behaviour
3. update config file to control serial keyboard mapping
4. update config file to also read a global cfg (eg chip8.com)
5. ./ command line switch for TMS/serial output
6. ./ auto detect if tms available
7. ./ auto detect if timer available
8. add sound support, thru hbios calls
9. add missing opcode support
10. add support for super-chip 48 instructions


Issues:
  ./ Config loading may not work for game with extension in filename
  ./ Make -t default, if TMS is detected
  ./ remove use of getLineNumber()
  ./ de-dup the filereader.* code
  ./ look for other potential deduping re the tokening processing
  * review use of chk and chkMsg
  * fix chip8asm/filereader's use of chk

v9958:
  1. spike use of high speed CPU to VRAM functions - do they wrap as required - yes - overheads start to add up.
  2. Disable sprite processing? Already disabled.
  3. Disable vdp during processing?
  4. Disable ints during processing?

  5. Add colour planes
  6. Can we use VDP to CPU to read XOR impact? no!
