2. update config file to control quirks behaviour
  2.1.  <<= and >>= modify vx in place and ignore vy.
  2.2.  load and store operations leave i unchanged.
  2.3.  4 high bits of target address determines the offset register of jump0 instead of v0.
  2.4.  clear vF after vx |= vy, vx &= vy, and vx ^= vy.
  2.5.  clip sprites at screen edges instead of wrapping.
  2.6.  vblank after drawing sprites????
4. update config file to also read a global cfg (eg chip8.com)
9. add missing opcode support
10. add support for super-chip/octo instructions
11. Create seperate MSX/HBIOS builds
13. serial input/controller needs to be made more faster
14. colour mapping needs to be completed (for standard 16 pallet mode)
15. more refined colour pallet map to rgb values for v9958
16. Documentation/samples for configuration file
17. Add support for dual controller mapping

Issues:
  * review use of chk and chkMsg
  * fix chip8asm/filereader's use of chk
  * Controller/serial input mappings of more than 64 will cause memory corruption
  *
v9958:


BUG:
  VDP lores app will crash, if run after using hires mode - seems vdp is not correctly re-initialised
  ./ Controller only detect single button pressing - need to support concurrent button


Done:
1. ./ load an optional config file (eg: invaders.cfg) to control colour mapping
3. ./ update config file to control serial keyboard mapping
5. ./ command line switch for TMS/serial output
6. ./ auto detect if tms available
7. ./ auto detect if timer available
8. ./ add sound support, thru hbios calls
12. ./ Add support for MSX Controller
  ./ Config loading may not work for game with extension in filename
  ./ Make -t default, if TMS is detected
  ./ remove use of getLineNumber()
  ./ de-dup the filereader.* code
  ./ look for other potential deduping re the tokening processing
