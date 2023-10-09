
test_instruction.o: chip8/test_instruction.c chip8/byte_code_executor.h \
 datatypes.h chip8/fontsets.h chip8/msx_keyboard.h chip8/stack.h \
 chip8/systemstate.h terminal_codes.h test_expectations.h datatypes.h \
 terminal_codes.h test_opcodes.h chip8/test_system.h xstdio.h
tms.o: chip8/tms.c chip8/tms.h datatypes.h chip8/systemstate.h
ym2149.o: chip8/ym2149.c chip8/ym2149.h chip8/audio.h datatypes.h \
 chip8/systemstate.h chip8/timers.h
key_monitor.o: chip8/key_monitor.c chip8/key_monitor.h datatypes.h \
 charconstants.h chip8/msx.h chip8/msx_keyboard.h chip8/systemstate.h \
 chip8/timers.h chip8/ym2149.h chip8/audio.h
msx_keyboard.o: chip8/msx_keyboard.c chip8/msx_keyboard.h
instr_sound.o: chip8/instr_sound.c chip8/instr_sound.h datatypes.h \
 chip8/audio.h chip8/systemstate.h chip8/timers.h
v9958.o: chip8/v9958.c chip8/v9958.h datatypes.h chip8/systemstate.h
instr_tms_output.o: chip8/instr_tms_output.c chip8/instr_tms_output.h \
 datatypes.h chip8/systemstate.h chip8/tms.h
test_system.o: chip8/test_system.c chip8/test_system.h datatypes.h \
 xstdio.h chip8/msx.h chip8/systemstate.h chip8/timers.h chip8/ym2149.h \
 chip8/audio.h
configuration_loader.o: chip8/configuration_loader.c chip8/configreader.h \
 token.h datatypes.h chip8/error_reports.h datatypes.h filenames.h \
 filereader.h chip8/msx_keyboard.h chip8/systemstate.h chip8/v9958.h
instr_v9958_output.o: chip8/instr_v9958_output.c \
 chip8/instr_v9958_output.h datatypes.h chip8/instr_output.h \
 chip8/v9958.h chip8/systemstate.h
hello2.o: chip8/hello2.c
error_reports.o: chip8/error_reports.c chip8/error_reports.h datatypes.h \
 chip8/configreader.h token.h datatypes.h xstdio.h
stack.o: chip8/stack.c chip8/stack.h datatypes.h chip8/systemstate.h
random.o: chip8/random.c chip8/random.h datatypes.h chip8/systimer.h \
 chip8/msx.h chip8/systemstate.h
test_configuration.o: chip8/test_configuration.c \
 chip8/test_configuration.h chip8/configreader.h token.h datatypes.h \
 chip8/configuration_loader.h chip8/systemstate.h datatypes.h \
 test_expectations.h terminal_codes.h chip8/tms.h chip8/v9958.h
configreader.o: chip8/configreader.c chip8/configreader.h token.h \
 datatypes.h charconstants.h datatypes.h chip8/error_reports.h \
 filereader.h chip8/systemstate.h chip8/tms.h xstdlib.h
byte_code_executor.o: chip8/byte_code_executor.c datatypes.h \
 chip8/key_monitor.h chip8/msx_keyboard.h chip8/stack.h \
 chip8/systemstate.h chip8/systimer.h chip8/msx.h chip8/timers.h xstdio.h \
 chip8/instr_output.h chip8/v9958.h chip8/instr_pc.h chip8/instr_random.h \
 chip8/random.h chip8/instr_registers.h chip8/fontsets.h \
 chip8/instr_sound.h
fontsets.o: chip8/fontsets.c chip8/fontsets.h datatypes.h
msx.o: chip8/msx.c chip8/msx.h
timers.o: chip8/timers.c chip8/timers.h datatypes.h chip8/instr_sound.h \
 chip8/msx.h chip8/systemstate.h chip8/systimer.h
instr_output.o: chip8/instr_output.c chip8/instr_output.h chip8/v9958.h \
 datatypes.h chip8/error_reports.h chip8/instr_tms_output.h \
 chip8/instr_v9958_output.h chip8/systemstate.h chip8/tms.h chip8/vdp.h
audio.o: chip8/audio.c chip8/audio.h datatypes.h chip8/systemstate.h \
 chip8/ym2149.h
systemstate.o: chip8/systemstate.c chip8/systemstate.h datatypes.h \
 chip8/tms.h
main.o: chip8/main.c chip8/audio.h datatypes.h chip8/byte_code_executor.h \
 chip8/configuration_loader.h cpm.h chip8/error_reports.h filenames.h \
 chip8/instr_output.h chip8/v9958.h chip8/instr_sound.h \
 chip8/msx_keyboard.h msxdos.h chip8/random.h chip8/systemstate.h \
 chip8/systimer.h chip8/msx.h chip8/timers.h chip8/tms.h xstdio.h \
 xstdlib.h
assembler.o: chip8asm/assembler.c chip8asm/assembler.h datatypes.h \
 chip8asm/emitters.h chip8asm/error_reports.h filereader.h \
 chip8asm/labels.h chip8asm/systemstate.h chip8asm/token_parser.h \
 chip8asm/tokenreader.h token.h datatypes.h
test_assembler.o: chip8asm/test_assembler.c chip8asm/assembler.h \
 datatypes.h chip8asm/labels.h chip8asm/systemstate.h chip8asm/expr.h \
 test_opcodes.h xstdio.h test_expectations.h datatypes.h terminal_codes.h \
 chip8asm/test_helper.h terminal_codes.h chip8asm/tokenreader.h token.h
expr.o: chip8asm/expr.c chip8asm/expr.h datatypes.h chartesters.h \
 chip8asm/error.h chip8asm/error_reports.h chip8asm/exit.h \
 chip8asm/labels.h xstdlib.h
exit.o: chip8asm/exit.c
labels.o: chip8asm/labels.c chip8asm/labels.h datatypes.h \
 chip8asm/error_reports.h chip8asm/systemstate.h xstdio.h
error_reports.o: chip8asm/error_reports.c chip8asm/error_reports.h \
 datatypes.h chip8asm/error.h chip8asm/exit.h chip8asm/tokenreader.h \
 token.h datatypes.h
tokenreader.o: chip8asm/tokenreader.c chip8asm/tokenreader.h token.h \
 datatypes.h chartesters.h datatypes.h chip8asm/error.h chip8asm/exit.h \
 filereader.h
token_parser.o: chip8asm/token_parser.c chip8asm/token_parser.h \
 datatypes.h chip8asm/error_reports.h chip8asm/expr.h \
 chip8asm/tokenreader.h token.h datatypes.h
systemstate.o: chip8asm/systemstate.c chip8asm/systemstate.h datatypes.h
emitters.o: chip8asm/emitters.c chip8asm/emitters.h datatypes.h \
 chip8asm/systemstate.h
main.o: chip8asm/main.c chip8asm/assembler.h datatypes.h \
 chip8asm/labels.h chip8asm/systemstate.h filenames.h filereader.h
token.o: token.c token.h datatypes.h charconstants.h chartesters.h \
 filereader.h
cpm.o: cpm.c cpm.h
test_expectations.o: test_expectations.c test_expectations.h datatypes.h \
 terminal_codes.h
test.o: test.c chip8/msx.h
hello.o: hello.c chip8/msx.h hello.h
filenames.o: filenames.c filenames.h
filereader.o: filereader.c filereader.h charconstants.h datatypes.h
chartesters.o: chartesters.c chartesters.h
./bin/cpm/crt.o: ./crt.asm
./bin/cpm/chip8/tmsasm.o: ./chip8/tmsasm.asm ./chip8/v9958.inc
./bin/cpm/chip8/video_detection.o: ./chip8/video_detection.asm
./bin/cpm/chip8/instr_v9958_output/draw_rowasm.o: ./chip8/instr_v9958_output/draw_rowasm.asm ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/scroll_commonasm.o: ./chip8/instr_v9958_output/scroll_commonasm.asm ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/draw_planeasm.o: ./chip8/instr_v9958_output/draw_planeasm.asm ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/test_segmentasm.o: ./chip8/instr_v9958_output/test_segmentasm.asm ./chip8/v9958.inc
./bin/cpm/chip8/ym2149asm.o: ./chip8/ym2149asm.asm
./bin/cpm/chip8/timerasm.o: ./chip8/timerasm.asm
./bin/cpm/chip8/sub_vx_vy_asm.o: ./chip8/sub_vx_vy_asm.asm
./bin/cpm/chip8/v9958asm.o: ./chip8/v9958asm.asm ./chip8/v9958.inc
./bin/cpm/chip8/instr_registerasm.o: ./chip8/instr_registerasm.asm
./bin/cpm/xstdio.o: ./xstdio.asm
./bin/cpm/chip8asm/error.o: ./chip8asm/error.asm
./bin/cpm/chip8asm/data.o: ./chip8asm/data.asm
./bin/cpm/relocmem.o: ./relocmem.asm
./bin/cpm/memap.o: ./memap.asm
./bin/cpm/msxdos.o: ./msxdos.asm
./bin/cpm/debug.o: ./debug.asm
./bin/cpm/reloccrt.o: ./reloccrt.asm
./bin/cpm/nonreloccrt.o: ./nonreloccrt.asm
./bin/cpm/xstrtol.o: ./xstrtol.asm
./bin/cpm/cpm.o: ./cpm.asm
./bin/cpm/chip8/instr_v9958_output/scrl_rightasm.o: ./chip8/instr_v9958_output/scrl_rightasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/scrl_upasm.o: ./chip8/instr_v9958_output/scrl_upasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/scrl_leftasm.o: ./chip8/instr_v9958_output/scrl_leftasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/draw_segmentasm.o: ./chip8/instr_v9958_output/draw_segmentasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/cpm/chip8/instr_v9958_output/scrl_downasm.o: ./chip8/instr_v9958_output/scrl_downasm.asm.m4 ./chip8/macros.inc.m4 ./chip8/v9958.inc
./bin/cpm/hello.o: ./hello.asm.m4 ./chip8/v9958.inc
./bin/cpm/loader.o: ./loader.asm.m4
