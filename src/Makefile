.PHONY: all
all: ../bin/chip8.com ../bin/chip8s.com ../bin/chip8asm.com ../bin/tstasmbl.com ../bin/tstinstr.com

RELOCASSEMBLER=node ./apploader.js --page-align
ASSEMBLER=z80asm

ASS_FLAGS=-mz80 \
	-D__SDCC -D__SDCC_IY \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
	-I"${ZCCCFG}/../../lib" \
	-L. \
	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
	-imath32 \
	--list -iz80

ZSDCPP_FLAGS=-iquote"." \
	-D__Z88DK -D__EMBEDDED -DEMBEDDED -D__EMBEDDED_Z80 -D__Z80 -DZ80 \
	-D__SDCC -D__SDCC_IY \
	-D__SDCC 						\
	-isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc"

include ./depends.d

chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
chip8asm_objects = crt.o memap.o cpm.o hbios_cio.o hbios.o memap.o nonreloccrt.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))

../bin/chip8asm.com: $(filter-out %test_assembler.o, $(chip8asm_objects))
	@$(ASSEMBLER) --output=$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv crt_code_crt_init.bin ../bin/chip8asm.com
	@echo "\nBuilt ./bin/chip8asm.com"

exclude_from_tests = %main.o %filereader.o %exit.o %error.o %tty.o %keys.o %systimer.o %random.o %diagnostics.o
../bin/tstasmbl.com: $(filter-out $(exclude_from_tests),$(chip8asm_objects))
	@$(ASSEMBLER) --output=tstasmbl.bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv tstasmbl_code_crt_init.bin ../bin/tstasmbl.com
	@echo "\nBuilt ./bin/tstasmbl.com"

chip8_sources = $(wildcard ./chip8/*.c) $(wildcard ./chip8/*.asm)
chip8_objects = memap.o relocmem.asm crt.o reloccrt.o hbios.o hbios_cio.o hbios_sys.o cpm.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8_sources)))
chip8_tms = $(filter-out %byte_code_executor_serial.o %diagnostics.o %instr_serial_output.o, $(chip8_objects))
chip8_serial = $(filter-out %byte_code_executor_tms.o %instr_tms_output.o, $(chip8_objects))

../bin/chip8.com: $(filter-out %test_instruction.o, $(chip8_tms))
	@$(RELOCASSEMBLER) --output=$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv memap.bin ../bin/chip8.com
	@echo "\nBuilt ./bin/chip8.com"

../bin/chip8s.com: $(filter-out %test_instruction.o, $(chip8_serial))
	@$(RELOCASSEMBLER) --output=$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv memap.bin ../bin/chip8s.com
	@echo "\nBuilt ./bin/chip8s.com"

../bin/tstinstr.com: $(filter-out $(exclude_from_tests),$(chip8_serial))
	@$(RELOCASSEMBLER) --output=tstinstr.bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv tstinstr.bin ../bin/tstinstr.com
	@echo "\nBuilt ./bin/tstinstr.com"


ZCCRELFLAGS=

ifdef RELEASE
ZCCRELFLAGS=" -SO3 --max-allocs-per-node200000 -Cs"--opt-code-speed" --allow-unsafe-read"
endif

ZCCFLAGS="${ZCCRELFLAGS}"

#-DDIAGNOSTICS_ON
%.asm: %.c
	@zcc +embedded -subtype=none -lm -clib=sdcc_iy -vn -cleanup -m -S --list $< -o $@  -create-app  -Cs --Werror --c-code-in-asm $(ZCCFLAGS)
	@echo "compiled $< to $@"

deps: SHELL := bash
deps:
	@echo "" > ./depends.d && \
	find -name "*.c" | while read -r file; do \
		file_no_ext="$${file%.*}" 							; \
		filename=$$(basename $$file_no_ext)			; \
		from="$$filename.rel"										; \
		to="$$file_no_ext.asm"									; \
		zsdcpp ${ZSDCPP_FLAGS} -MM -MF /tmp/deps.deps $$file; \
		sed "s+$$from+$$to+g" /tmp/deps.deps >> ./depends.d; \
	done && \
	echo "./depends.d created"

%.o: %.asm
	@$(ASSEMBLER) --output=$(basename $<).bin $(ASS_FLAGS) $(basename $<).asm
	@echo "Assembled $< to $@"

clean:
	@find . -type f -name '*.bin' -delete
	@find . -type f -name '*.com' -delete
	@find . -type f -name '*.err' -delete
	@find . -type f -name '*.lis' -delete
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.sym' -delete
	@rm -f ../bin/*
	@rm -f ./chip8/byte_code_executor.asm
	@rm -f ./chip8/diagnostics.asm
	@rm -f ./chip8/instr_serial_output.asm
	@rm -f ./chip8/instr_tms_output.asm
	@rm -f ./chip8/key_monitor.asm
	@rm -f ./chip8/keys.asm
	@rm -f ./chip8/main.asm
	@rm -f ./chip8/random.asm
	@rm -f ./chip8/stack.asm
	@rm -f ./chip8/systemstate.asm
	@rm -f ./chip8/systimer.asm
	@rm -f ./chip8/timers.asm
	@rm -f ./chip8/tms.asm
	@rm -f ./chip8/tty.asm
	@rm -f ./chip8asm/assembler.asm
	@rm -f ./chip8asm/chartesters.asm
	@rm -f ./chip8asm/emitters.asm
	@rm -f ./chip8asm/error_reports.asm
	@rm -f ./chip8asm/error.asm
	@rm -f ./chip8asm/exit.asm
	@rm -f ./chip8asm/expr.asm
	@rm -f ./chip8asm/filereader.asm
	@rm -f ./chip8asm/labels.asm
	@rm -f ./chip8asm/main.asm
	@rm -f ./chip8asm/test_assembler.asm
	@rm -f ./chip8asm/test_instruction.asm
	@rm -f ./chip8asm/token_parser.asm
	@rm -f ./chip8asm/tokenreader.asm
	@rm -f hbios.asm
	@rm -f xstdio.asm
	@rm -f zcc_opt.def
