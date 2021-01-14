1. ./ load an optional config file (eg: invaders.cfg) to control colour mapping
2. update config file to control quirks behaviour
  2.1.  <<= and >>= modify vx in place and ignore vy.
  2.2.  load and store operations leave i unchanged.
  2.3.  4 high bits of target address determines the offset register of jump0 instead of v0.
  2.4.  clear vF after vx |= vy, vx &= vy, and vx ^= vy.
  2.5.  clip sprites at screen edges instead of wrapping.
  2.6.  vblank after drawing sprites????
3. update config file to control serial keyboard mapping
4. update config file to also read a global cfg (eg chip8.com)
5. ./ command line switch for TMS/serial output
6. ./ auto detect if tms available
7. ./ auto detect if timer available
8. add sound support, thru hbios calls
9. add missing opcode support
10. add support for super-chip/octo instructions
11. Create seperate MSX/HBIOS builds

Issues:
  ./ Config loading may not work for game with extension in filename
  ./ Make -t default, if TMS is detected
  ./ remove use of getLineNumber()
  ./ de-dup the filereader.* code
  ./ look for other potential deduping re the tokening processing
  * review use of chk and chkMsg
  * fix chip8asm/filereader's use of chk

v9958:
