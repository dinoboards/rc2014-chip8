
./bin/chip8/test_instruction.c.asm: chip8/test_instruction.c chip8/byte_code_executor.h \
 datatypes.h chip8/fontsets.h chip8/stack.h chip8/systemstate.h \
 terminal_codes.h test_expectations.h datatypes.h terminal_codes.h \
 test_opcodes.h chip8/test_system.h
./bin/chip8/tty.c.asm: chip8/tty.c chip8/tty.h hbios.h xstdio.h
./bin/chip8/keys.c.asm: chip8/keys.c chip8/keys.h datatypes.h hbios.h
./bin/chip8/tms.c.asm: chip8/tms.c chip8/tms.h datatypes.h hbios.h chip8/keys.h \
 chip8/systemstate.h
./bin/chip8/ym2149.c.asm: chip8/ym2149.c chip8/ym2149.h chip8/audio.h datatypes.h \
 chip8/systemstate.h chip8/timers.h
./bin/chip8/key_monitor.c.asm: chip8/key_monitor.c chip8/key_monitor.h datatypes.h \
 charconstants.h chip8/keys.h chip8/systemstate.h chip8/timers.h \
 chip8/ym2149.h chip8/audio.h
./bin/chip8/instr_sound.c.asm: chip8/instr_sound.c chip8/instr_sound.h datatypes.h \
 chip8/audio.h hbios.h chip8/systemstate.h chip8/timers.h
./bin/chip8/v9958.c.asm: chip8/v9958.c chip8/v9958.h datatypes.h chip8/systemstate.h
./bin/chip8/instr_tms_output.c.asm: chip8/instr_tms_output.c chip8/instr_tms_output.h \
 datatypes.h chip8/vid_int.h chip8/systemstate.h chip8/tms.h
./bin/chip8/test_system.c.asm: chip8/test_system.c chip8/test_system.h datatypes.h \
 chip8/systemstate.h chip8/timers.h chip8/ym2149.h chip8/audio.h
./bin/chip8/configuration_loader.c.asm: chip8/configuration_loader.c \
 chip8/configreader.h token.h datatypes.h cpm.h chip8/error_reports.h \
 datatypes.h filereader.h cpm.h chip8/systemstate.h chip8/v9958.h
./bin/chip8/instr_v9958_output.c.asm: chip8/instr_v9958_output.c \
 chip8/instr_v9958_output.h datatypes.h chip8/instr_output.h \
 chip8/v9958.h chip8/systemstate.h
./bin/chip8/hello2.c.asm: chip8/hello2.c
./bin/chip8/error_reports.c.asm: chip8/error_reports.c chip8/error_reports.h \
 datatypes.h chip8/configreader.h token.h datatypes.h xstdio.h
./bin/chip8/hbios_audio.c.asm: chip8/hbios_audio.c chip8/hbios_audio.h datatypes.h \
 hbios.h chip8/instr_sound.h chip8/systemstate.h chip8/timers.h
./bin/chip8/stack.c.asm: chip8/stack.c chip8/stack.h datatypes.h chip8/systemstate.h
./bin/chip8/random.c.asm: chip8/random.c chip8/random.h datatypes.h chip8/systimer.h \
 hbios.h chip8/systemstate.h
./bin/chip8/test_configuration.c.asm: chip8/test_configuration.c \
 chip8/test_configuration.h chip8/configreader.h token.h datatypes.h \
 chip8/configuration_loader.h cpm.h chip8/systemstate.h datatypes.h \
 test_expectations.h terminal_codes.h chip8/tms.h chip8/v9958.h
./bin/chip8/configreader.c.asm: chip8/configreader.c chip8/configreader.h token.h \
 datatypes.h charconstants.h datatypes.h chip8/error_reports.h \
 filereader.h cpm.h chip8/systemstate.h chip8/tms.h xstdlib.h
./bin/chip8/byte_code_executor.c.asm: chip8/byte_code_executor.c datatypes.h \
 chip8/key_monitor.h chip8/stack.h chip8/systemstate.h chip8/systimer.h \
 hbios.h chip8/timers.h chip8/tty.h xstdio.h chip8/instr_output.h \
 chip8/v9958.h chip8/instr_pc.h chip8/instr_random.h chip8/random.h \
 chip8/instr_registers.h chip8/fontsets.h chip8/instr_sound.h
./bin/chip8/fontsets.c.asm: chip8/fontsets.c chip8/fontsets.h datatypes.h
./bin/chip8/timers.c.asm: chip8/timers.c chip8/timers.h datatypes.h chip8/tty.h hbios.h \
 chip8/instr_sound.h chip8/systemstate.h chip8/systimer.h
./bin/chip8/instr_output.c.asm: chip8/instr_output.c chip8/instr_output.h chip8/v9958.h \
 datatypes.h chip8/error_reports.h hbios.h chip8/instr_tms_output.h \
 chip8/vid_int.h chip8/instr_v9958_output.h chip8/systemstate.h \
 chip8/tms.h chip8/vdp.h xstdio.h
./bin/chip8/audio.c.asm: chip8/audio.c chip8/audio.h datatypes.h hbios.h \
 chip8/hbios_audio.h chip8/systemstate.h chip8/ym2149.h
./bin/chip8/systemstate.c.asm: chip8/systemstate.c chip8/systemstate.h datatypes.h \
 chip8/tms.h
./bin/chip8/main.c.asm: chip8/main.c chip8/audio.h datatypes.h \
 chip8/byte_code_executor.h chip8/configuration_loader.h cpm.h \
 chip8/error_reports.h hbios.h chip8/instr_output.h chip8/v9958.h \
 chip8/instr_sound.h chip8/random.h chip8/systemstate.h chip8/systimer.h \
 chip8/timers.h chip8/tms.h xstdio.h xstdlib.h
./bin/chip8asm/error.c.asm: chip8asm/error.c chip8asm/error.h hbios.h xstdio.h
./bin/chip8asm/assembler.c.asm: chip8asm/assembler.c chip8asm/assembler.h datatypes.h \
 cpm.h chip8asm/emitters.h chip8asm/error_reports.h filereader.h cpm.h \
 chip8asm/labels.h chip8asm/systemstate.h chip8asm/token_parser.h \
 chip8asm/tokenreader.h token.h datatypes.h
./bin/chip8asm/test_assembler.c.asm: chip8asm/test_assembler.c chip8asm/assembler.h \
 datatypes.h chip8asm/labels.h chip8asm/systemstate.h cpm.h \
 chip8asm/expr.h test_opcodes.h xstdio.h test_expectations.h datatypes.h \
 terminal_codes.h chip8asm/test_helper.h terminal_codes.h \
 chip8asm/tokenreader.h token.h
./bin/chip8asm/expr.c.asm: chip8asm/expr.c chip8asm/expr.h chartesters.h datatypes.h \
 chip8asm/error.h chip8asm/error_reports.h chip8asm/exit.h \
 chip8asm/labels.h xstdlib.h
./bin/chip8asm/exit.c.asm: chip8asm/exit.c
./bin/chip8asm/labels.c.asm: chip8asm/labels.c chip8asm/labels.h datatypes.h \
 chip8asm/error_reports.h chip8asm/systemstate.h xstdio.h
./bin/chip8asm/error_reports.c.asm: chip8asm/error_reports.c chip8asm/error_reports.h \
 datatypes.h chip8asm/error.h chip8asm/exit.h chip8asm/tokenreader.h \
 token.h datatypes.h
./bin/chip8asm/tokenreader.c.asm: chip8asm/tokenreader.c chip8asm/tokenreader.h token.h \
 datatypes.h chartesters.h cpm.h datatypes.h chip8asm/error.h \
 chip8asm/exit.h filereader.h cpm.h
./bin/chip8asm/token_parser.c.asm: chip8asm/token_parser.c chip8asm/token_parser.h \
 datatypes.h chip8asm/error_reports.h chip8asm/expr.h hbios.h \
 chip8asm/tokenreader.h token.h datatypes.h xstdio.h
./bin/chip8asm/systemstate.c.asm: chip8asm/systemstate.c chip8asm/systemstate.h \
 datatypes.h
./bin/chip8asm/emitters.c.asm: chip8asm/emitters.c chip8asm/emitters.h datatypes.h \
 chip8asm/systemstate.h
./bin/chip8asm/main.c.asm: chip8asm/main.c chip8asm/assembler.h datatypes.h \
 chip8asm/labels.h chip8asm/systemstate.h cpm.h xstdio.h
./bin/token.c.asm: token.c token.h datatypes.h charconstants.h chartesters.h \
 filereader.h cpm.h
./bin/cpm.c.asm: cpm.c cpm.h hbios.h
./bin/test_expectations.c.asm: test_expectations.c test_expectations.h \
 datatypes.h terminal_codes.h
./bin/xstdio.c.asm: xstdio.c xstdio.h datatypes.h hbios.h
./bin/hello.c.asm: hello.c hello.h
./bin/filereader.c.asm: filereader.c filereader.h cpm.h charconstants.h \
 datatypes.h
./bin/hbios.c.asm: hbios.c hbios.h
./bin/chartesters.c.asm: chartesters.c chartesters.h
./bin/crt.o: ./crt.asm
./bin/chip8/tmsasm.o: ./chip8/tmsasm.asm ./chip8/v9958.inc
./bin/chip8/video_detection.o: ./chip8/video_detection.asm
./bin/chip8/instr_v9958_output/draw_rowasm.o: ./chip8/instr_v9958_output/draw_rowasm.asm ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/scroll_commonasm.o: ./chip8/instr_v9958_output/scroll_commonasm.asm ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/draw_planeasm.o: ./chip8/instr_v9958_output/draw_planeasm.asm ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/test_segmentasm.o: ./chip8/instr_v9958_output/test_segmentasm.asm ./chip8/v9958.inc
./bin/chip8/vid_intasm.o: ./chip8/vid_intasm.asm ./hbios_sys.inc ./chip8/v9958.inc
./bin/chip8/ym2149asm.o: ./chip8/ym2149asm.asm
./bin/chip8/timerasm.o: ./chip8/timerasm.asm
./bin/chip8/sub_vx_vy_asm.o: ./chip8/sub_vx_vy_asm.asm
./bin/chip8/v9958asm.o: ./chip8/v9958asm.asm ./chip8/v9958.inc
./bin/chip8/instr_registerasm.o: ./chip8/instr_registerasm.asm
./bin/chip8asm/data.o: ./chip8asm/data.asm
./bin/hbios_snd.o: ./hbios_snd.asm
./bin/relocmem.o: ./relocmem.asm
./bin/memap.o: ./memap.asm
./bin/debug.o: ./debug.asm
./bin/reloccrt.o: ./reloccrt.asm
./bin/nonreloccrt.o: ./nonreloccrt.asm
./bin/xstrtol.o: ./xstrtol.asm
./bin/cpmasm.o: ./cpmasm.asm
./bin/hbios_cio.o: ./hbios_cio.asm
./bin/chip8/instr_v9958_output/scrl_rightasm.o: ./chip8/instr_v9958_output/scrl_rightasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/scrl_upasm.o: ./chip8/instr_v9958_output/scrl_upasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/scrl_leftasm.o: ./chip8/instr_v9958_output/scrl_leftasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/draw_segmentasm.o: ./chip8/instr_v9958_output/draw_segmentasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/chip8/instr_v9958_output/scrl_downasm.o: ./chip8/instr_v9958_output/scrl_downasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/hello.o: ./hello.asm.m4 ./chip8/v9958.inc
./bin/loader.o: ./loader.asm.m4
