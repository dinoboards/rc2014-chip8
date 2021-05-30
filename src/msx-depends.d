
./bin/msx/chip8/test_instruction.c.asm: chip8/test_instruction.c chip8/byte_code_executor.h \
 datatypes.h chip8/fontsets.h chip8/msx_keyboard.h chip8/stack.h \
 chip8/systemstate.h terminal_codes.h test_expectations.h datatypes.h \
 terminal_codes.h test_opcodes.h chip8/test_system.h xstdio.h
./bin/msx/chip8/tms.c.asm: chip8/tms.c chip8/tms.h datatypes.h chip8/systemstate.h
./bin/msx/chip8/ym2149.c.asm: chip8/ym2149.c chip8/ym2149.h chip8/audio.h datatypes.h \
 chip8/systemstate.h chip8/timers.h
./bin/msx/chip8/key_monitor.c.asm: chip8/key_monitor.c chip8/key_monitor.h datatypes.h \
 charconstants.h chip8/msx.h chip8/msx_keyboard.h chip8/systemstate.h \
 chip8/timers.h chip8/ym2149.h chip8/audio.h
./bin/msx/chip8/msx_keyboard.c.asm: chip8/msx_keyboard.c chip8/msx_keyboard.h
./bin/msx/chip8/instr_sound.c.asm: chip8/instr_sound.c chip8/instr_sound.h datatypes.h \
 chip8/audio.h chip8/systemstate.h chip8/timers.h
./bin/msx/chip8/v9958.c.asm: chip8/v9958.c chip8/v9958.h datatypes.h chip8/systemstate.h
./bin/msx/chip8/instr_tms_output.c.asm: chip8/instr_tms_output.c chip8/instr_tms_output.h \
 datatypes.h chip8/systemstate.h chip8/tms.h
./bin/msx/chip8/test_system.c.asm: chip8/test_system.c chip8/test_system.h datatypes.h \
 xstdio.h chip8/msx.h chip8/systemstate.h chip8/timers.h chip8/ym2149.h \
 chip8/audio.h
./bin/msx/chip8/configuration_loader.c.asm: chip8/configuration_loader.c \
 chip8/configreader.h token.h datatypes.h chip8/error_reports.h \
 datatypes.h filenames.h filereader.h chip8/msx_keyboard.h \
 chip8/systemstate.h chip8/v9958.h
./bin/msx/chip8/instr_v9958_output.c.asm: chip8/instr_v9958_output.c \
 chip8/instr_v9958_output.h datatypes.h chip8/instr_output.h \
 chip8/v9958.h chip8/systemstate.h
./bin/msx/chip8/hello2.c.asm: chip8/hello2.c
./bin/msx/chip8/error_reports.c.asm: chip8/error_reports.c chip8/error_reports.h \
 datatypes.h chip8/configreader.h token.h datatypes.h xstdio.h
./bin/msx/chip8/stack.c.asm: chip8/stack.c chip8/stack.h datatypes.h chip8/systemstate.h
./bin/msx/chip8/random.c.asm: chip8/random.c chip8/random.h datatypes.h chip8/systimer.h \
 chip8/msx.h chip8/systemstate.h
./bin/msx/chip8/test_configuration.c.asm: chip8/test_configuration.c \
 chip8/test_configuration.h chip8/configreader.h token.h datatypes.h \
 chip8/configuration_loader.h chip8/systemstate.h datatypes.h \
 test_expectations.h terminal_codes.h chip8/tms.h chip8/v9958.h
./bin/msx/chip8/configreader.c.asm: chip8/configreader.c chip8/configreader.h token.h \
 datatypes.h charconstants.h datatypes.h chip8/error_reports.h \
 filereader.h chip8/systemstate.h chip8/tms.h xstdlib.h
./bin/msx/chip8/byte_code_executor.c.asm: chip8/byte_code_executor.c datatypes.h \
 chip8/key_monitor.h chip8/stack.h chip8/systemstate.h chip8/systimer.h \
 chip8/msx.h chip8/timers.h xstdio.h chip8/instr_output.h chip8/v9958.h \
 chip8/instr_pc.h chip8/instr_random.h chip8/random.h \
 chip8/instr_registers.h chip8/fontsets.h chip8/instr_sound.h
./bin/msx/chip8/fontsets.c.asm: chip8/fontsets.c chip8/fontsets.h datatypes.h
./bin/msx/chip8/msx.c.asm: chip8/msx.c chip8/msx.h
./bin/msx/chip8/timers.c.asm: chip8/timers.c chip8/timers.h datatypes.h chip8/instr_sound.h \
 chip8/msx.h chip8/systemstate.h chip8/systimer.h
./bin/msx/chip8/instr_output.c.asm: chip8/instr_output.c chip8/instr_output.h chip8/v9958.h \
 datatypes.h chip8/error_reports.h chip8/instr_tms_output.h \
 chip8/instr_v9958_output.h chip8/systemstate.h chip8/tms.h chip8/vdp.h \
 xstdio.h
./bin/msx/chip8/audio.c.asm: chip8/audio.c chip8/audio.h datatypes.h chip8/systemstate.h \
 chip8/ym2149.h
./bin/msx/chip8/systemstate.c.asm: chip8/systemstate.c chip8/systemstate.h datatypes.h \
 chip8/tms.h
./bin/msx/chip8/main.c.asm: chip8/main.c chip8/audio.h datatypes.h \
 chip8/byte_code_executor.h chip8/configuration_loader.h \
 chip8/error_reports.h filenames.h chip8/instr_output.h chip8/v9958.h \
 chip8/instr_sound.h chip8/random.h chip8/systemstate.h chip8/systimer.h \
 chip8/msx.h chip8/timers.h chip8/tms.h xstdio.h xstdlib.h
./bin/msx/chip8asm/assembler.c.asm: chip8asm/assembler.c chip8asm/assembler.h datatypes.h \
 chip8asm/emitters.h chip8asm/error_reports.h filereader.h \
 chip8asm/labels.h chip8asm/systemstate.h chip8asm/token_parser.h \
 chip8asm/tokenreader.h token.h datatypes.h
./bin/msx/chip8asm/test_assembler.c.asm: chip8asm/test_assembler.c chip8asm/assembler.h \
 datatypes.h chip8asm/labels.h chip8asm/systemstate.h chip8asm/expr.h \
 test_opcodes.h xstdio.h test_expectations.h datatypes.h terminal_codes.h \
 chip8asm/test_helper.h terminal_codes.h chip8asm/tokenreader.h token.h
./bin/msx/chip8asm/expr.c.asm: chip8asm/expr.c chip8asm/expr.h datatypes.h chartesters.h \
 chip8asm/error.h chip8asm/error_reports.h chip8asm/exit.h \
 chip8asm/labels.h xstdlib.h
./bin/msx/chip8asm/exit.c.asm: chip8asm/exit.c
./bin/msx/chip8asm/labels.c.asm: chip8asm/labels.c chip8asm/labels.h datatypes.h \
 chip8asm/error_reports.h chip8asm/systemstate.h xstdio.h
./bin/msx/chip8asm/error_reports.c.asm: chip8asm/error_reports.c chip8asm/error_reports.h \
 datatypes.h chip8asm/error.h chip8asm/exit.h chip8asm/tokenreader.h \
 token.h datatypes.h
./bin/msx/chip8asm/tokenreader.c.asm: chip8asm/tokenreader.c chip8asm/tokenreader.h token.h \
 datatypes.h chartesters.h datatypes.h chip8asm/error.h chip8asm/exit.h \
 filereader.h
./bin/msx/chip8asm/token_parser.c.asm: chip8asm/token_parser.c chip8asm/token_parser.h \
 datatypes.h chip8asm/error_reports.h chip8asm/expr.h \
 chip8asm/tokenreader.h token.h datatypes.h
./bin/msx/chip8asm/systemstate.c.asm: chip8asm/systemstate.c chip8asm/systemstate.h \
 datatypes.h
./bin/msx/chip8asm/emitters.c.asm: chip8asm/emitters.c chip8asm/emitters.h datatypes.h \
 chip8asm/systemstate.h
./bin/msx/chip8asm/main.c.asm: chip8asm/main.c chip8asm/assembler.h datatypes.h \
 chip8asm/labels.h chip8asm/systemstate.h filenames.h filereader.h
./bin/msx/token.c.asm: token.c token.h datatypes.h charconstants.h chartesters.h \
 filereader.h
./bin/msx/test_expectations.c.asm: test_expectations.c test_expectations.h \
 datatypes.h terminal_codes.h
./bin/msx/test.c.asm: test.c chip8/msx.h
./bin/msx/hello.c.asm: hello.c chip8/msx.h
./bin/msx/filenames.c.asm: filenames.c filenames.h
./bin/msx/filereader.c.asm: filereader.c filereader.h charconstants.h datatypes.h
./bin/msx/chartesters.c.asm: chartesters.c chartesters.h
./bin/msx/crt.o: ./crt.asm
./bin/msx/chip8/tmsasm.o: ./chip8/tmsasm.asm ./chip8/v9958.inc
./bin/msx/chip8/video_detection.o: ./chip8/video_detection.asm
./bin/msx/chip8/instr_v9958_output/draw_rowasm.o: ./chip8/instr_v9958_output/draw_rowasm.asm ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/scroll_commonasm.o: ./chip8/instr_v9958_output/scroll_commonasm.asm ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/draw_planeasm.o: ./chip8/instr_v9958_output/draw_planeasm.asm ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/test_segmentasm.o: ./chip8/instr_v9958_output/test_segmentasm.asm ./chip8/v9958.inc
./bin/msx/chip8/ym2149asm.o: ./chip8/ym2149asm.asm
./bin/msx/chip8/timerasm.o: ./chip8/timerasm.asm
./bin/msx/chip8/sub_vx_vy_asm.o: ./chip8/sub_vx_vy_asm.asm
./bin/msx/chip8/v9958asm.o: ./chip8/v9958asm.asm ./chip8/v9958.inc
./bin/msx/chip8/instr_registerasm.o: ./chip8/instr_registerasm.asm
./bin/msx/xstdio.o: ./xstdio.asm
./bin/msx/chip8asm/error.o: ./chip8asm/error.asm
./bin/msx/chip8asm/data.o: ./chip8asm/data.asm
./bin/msx/relocmem.o: ./relocmem.asm
./bin/msx/memap.o: ./memap.asm
./bin/msx/debug.o: ./debug.asm
./bin/msx/reloccrt.o: ./reloccrt.asm
./bin/msx/nonreloccrt.o: ./nonreloccrt.asm
./bin/msx/xstrtol.o: ./xstrtol.asm
./bin/msx/chip8/instr_v9958_output/scrl_rightasm.o: ./chip8/instr_v9958_output/scrl_rightasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/scrl_upasm.o: ./chip8/instr_v9958_output/scrl_upasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/scrl_leftasm.o: ./chip8/instr_v9958_output/scrl_leftasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/draw_segmentasm.o: ./chip8/instr_v9958_output/draw_segmentasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/msx/chip8/instr_v9958_output/scrl_downasm.o: ./chip8/instr_v9958_output/scrl_downasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/msx/hello.o: ./hello.asm.m4 ./chip8/v9958.inc
./bin/msx/loader.o: ./loader.asm.m4
