
./chartesters.asm: chartesters.c chartesters.h
./chip8/audio.asm: chip8/audio.c chip8/audio.h datatypes.h hbios.h \
 chip8/hbios_audio.h chip8/systemstate.h chip8/ym2149.h
./chip8/byte_code_executor.asm: chip8/byte_code_executor.c datatypes.h \
 chip8/key_monitor.h chip8/stack.h chip8/systemstate.h chip8/systimer.h \
 hbios.h chip8/timers.h chip8/tty.h xstdio.h chip8/instr_output.h \
 chip8/v9958.h chip8/instr_pc.h chip8/instr_random.h chip8/random.h \
 chip8/instr_registers.h chip8/fontsets.h chip8/instr_sound.h
./chip8/configreader.asm: chip8/configreader.c chip8/configreader.h token.h \
 datatypes.h charconstants.h datatypes.h chip8/error_reports.h \
 filereader.h cpm.h chip8/systemstate.h chip8/tms.h xstdlib.h
./chip8/configuration_loader.asm: chip8/configuration_loader.c \
 chip8/configreader.h token.h datatypes.h cpm.h chip8/error_reports.h \
 datatypes.h filereader.h cpm.h chip8/systemstate.h
./chip8/error_reports.asm: chip8/error_reports.c chip8/error_reports.h \
 datatypes.h chip8/configreader.h token.h datatypes.h xstdio.h
./chip8/fontsets.asm: chip8/fontsets.c chip8/fontsets.h datatypes.h
./chip8/hbios_audio.asm: chip8/hbios_audio.c chip8/hbios_audio.h datatypes.h \
 hbios.h chip8/instr_sound.h chip8/systemstate.h chip8/timers.h
./chip8/instr_output.asm: chip8/instr_output.c chip8/instr_output.h chip8/v9958.h \
 datatypes.h chip8/error_reports.h hbios.h chip8/instr_tms_output.h \
 chip8/vid_int.h chip8/instr_v9958_output.h chip8/systemstate.h \
 chip8/tms.h chip8/vdp.h
./chip8/instr_sound.asm: chip8/instr_sound.c chip8/instr_sound.h datatypes.h \
 chip8/audio.h hbios.h chip8/systemstate.h chip8/timers.h
./chip8/instr_tms_output.asm: chip8/instr_tms_output.c chip8/instr_tms_output.h \
 datatypes.h chip8/vid_int.h chip8/systemstate.h chip8/tms.h
./chip8/instr_v9958_output.asm: chip8/instr_v9958_output.c \
 chip8/instr_v9958_output.h datatypes.h chip8/instr_output.h \
 chip8/v9958.h chip8/systemstate.h
./chip8/keys.asm: chip8/keys.c chip8/keys.h datatypes.h hbios.h
./chip8/key_monitor.asm: chip8/key_monitor.c chip8/key_monitor.h datatypes.h \
 charconstants.h chip8/keys.h chip8/systemstate.h chip8/timers.h
./chip8/main.asm: chip8/main.c chip8/audio.h datatypes.h \
 chip8/byte_code_executor.h chip8/configuration_loader.h cpm.h \
 chip8/error_reports.h hbios.h chip8/instr_output.h chip8/v9958.h \
 chip8/instr_sound.h chip8/random.h chip8/systemstate.h chip8/systimer.h \
 chip8/timers.h chip8/tms.h xstdio.h xstdlib.h
./chip8/random.asm: chip8/random.c chip8/random.h datatypes.h chip8/systimer.h \
 hbios.h chip8/systemstate.h
./chip8/stack.asm: chip8/stack.c chip8/stack.h datatypes.h chip8/systemstate.h \
 xstdio.h
./chip8/systemstate.asm: chip8/systemstate.c chip8/systemstate.h datatypes.h \
 chip8/tms.h
./chip8/test_configuration.asm: chip8/test_configuration.c \
 chip8/test_configuration.h chip8/configreader.h token.h datatypes.h \
 chip8/configuration_loader.h cpm.h chip8/systemstate.h datatypes.h \
 test_expectations.h terminal_codes.h chip8/tms.h xstdio.h
./chip8/test_instruction.asm: chip8/test_instruction.c chip8/byte_code_executor.h \
 datatypes.h chip8/fontsets.h chip8/stack.h chip8/systemstate.h \
 terminal_codes.h test_opcodes.h chip8/timers.h xstdio.h \
 chip8/test_helpers.h chip8/keys.h test_expectations.h datatypes.h \
 terminal_codes.h
./chip8/timers.asm: chip8/timers.c chip8/timers.h datatypes.h chip8/tty.h hbios.h \
 chip8/instr_sound.h chip8/systemstate.h chip8/systimer.h
./chip8/tms.asm: chip8/tms.c chip8/tms.h datatypes.h hbios.h chip8/keys.h \
 chip8/systemstate.h
./chip8/tty.asm: chip8/tty.c chip8/tty.h hbios.h xstdio.h
./chip8/v9958.asm: chip8/v9958.c chip8/v9958.h datatypes.h chip8/systemstate.h
./chip8/ym2149.asm: chip8/ym2149.c chip8/ym2149.h chip8/audio.h datatypes.h \
 chip8/systemstate.h chip8/timers.h
./chip8asm/assembler.asm: chip8asm/assembler.c chip8asm/assembler.h datatypes.h \
 cpm.h chip8asm/emitters.h chip8asm/error_reports.h filereader.h cpm.h \
 chip8asm/labels.h chip8asm/systemstate.h chip8asm/token_parser.h \
 chip8asm/tokenreader.h token.h datatypes.h
./chip8asm/emitters.asm: chip8asm/emitters.c chip8asm/emitters.h datatypes.h \
 chip8asm/systemstate.h
./chip8asm/error.asm: chip8asm/error.c chip8asm/error.h hbios.h xstdio.h
./chip8asm/error_reports.asm: chip8asm/error_reports.c chip8asm/error_reports.h \
 datatypes.h chip8asm/error.h chip8asm/exit.h chip8asm/tokenreader.h \
 token.h datatypes.h
./chip8asm/exit.asm: chip8asm/exit.c
./chip8asm/expr.asm: chip8asm/expr.c chip8asm/expr.h chartesters.h datatypes.h \
 chip8asm/error.h chip8asm/error_reports.h chip8asm/exit.h \
 chip8asm/labels.h xstdlib.h
./chip8asm/labels.asm: chip8asm/labels.c chip8asm/labels.h datatypes.h \
 chip8asm/error_reports.h chip8asm/systemstate.h xstdio.h
./chip8asm/main.asm: chip8asm/main.c chip8asm/assembler.h datatypes.h \
 chip8asm/labels.h chip8asm/systemstate.h cpm.h xstdio.h
./chip8asm/systemstate.asm: chip8asm/systemstate.c chip8asm/systemstate.h \
 datatypes.h
./chip8asm/test_assembler.asm: chip8asm/test_assembler.c chip8asm/assembler.h \
 datatypes.h chip8asm/labels.h chip8asm/systemstate.h cpm.h \
 chip8asm/expr.h test_opcodes.h xstdio.h test_expectations.h datatypes.h \
 terminal_codes.h chip8asm/test_helper.h terminal_codes.h \
 chip8asm/tokenreader.h token.h
./chip8asm/tokenreader.asm: chip8asm/tokenreader.c chip8asm/tokenreader.h token.h \
 datatypes.h chartesters.h cpm.h datatypes.h chip8asm/error.h \
 chip8asm/exit.h filereader.h cpm.h
./chip8asm/token_parser.asm: chip8asm/token_parser.c chip8asm/token_parser.h \
 datatypes.h chip8asm/error_reports.h chip8asm/expr.h hbios.h \
 chip8asm/tokenreader.h token.h datatypes.h xstdio.h
./cpm.asm: cpm.c cpm.h hbios.h
./filereader.asm: filereader.c filereader.h cpm.h charconstants.h \
 datatypes.h
./hbios.asm: hbios.c hbios.h
./test_expectations.asm: test_expectations.c test_expectations.h \
 datatypes.h terminal_codes.h xstdio.h
./token.asm: token.c token.h datatypes.h charconstants.h chartesters.h \
 filereader.h cpm.h
./xstdio.asm: xstdio.c xstdio.h datatypes.h hbios.h
