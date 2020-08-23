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
	-lmath32 \
	-lcpm \
	-mz80 \
	-opt-speed

ZSDCPP_FLAGS=-iquote"." \
	-D__Z88DK -D__EMBEDDED -DEMBEDDED -D__EMBEDDED_Z80 -D__Z80 -DZ80 \
	-D__SDCC -D__SDCC_IY \
	-D__SDCC 						\
	-isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc"

ZCCRELFLAGS=

ifdef RELEASE
ZCCRELFLAGS=" -SO3 --max-allocs-per-node200000 -Cs"--opt-code-speed" --allow-unsafe-read"
endif

ZCCFLAGS="${ZCCRELFLAGS}"

include ./depends.d

chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
chip8asm_objects = crt.o memap.o cpm.o hbios_cio.o hbios.o memap.o nonreloccrt.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))
exclude_from_tests = %main.o %filereader.o %exit.o %error.o %tty.o %keys.o %systimer.o %random.o %diagnostics.o

chip8_sources = $(wildcard ./chip8/*.c) $(wildcard ./chip8/*.asm)
chip8_objects = memap.o relocmem.asm crt.o reloccrt.o hbios.o hbios_cio.o hbios_sys.o cpm.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8_sources)))
chip8_tms = $(filter-out %byte_code_executor_serial.o %diagnostics.o %instr_serial_output.o, $(chip8_objects))
chip8_serial = $(filter-out %byte_code_executor_tms.o %instr_tms_output.o, $(chip8_objects))

../bin/chip8asm.com: $(filter-out %test_assembler.o, $(chip8asm_objects))
	@$(ASSEMBLER) -o$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv crt_code_crt_init.bin ../bin/chip8asm.com
	@echo "\nBuilt ./bin/chip8asm.com"

../bin/tstasmbl.com: $(filter-out $(exclude_from_tests),$(chip8asm_objects))
	@$(ASSEMBLER) -otstasmbl.bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv tstasmbl_code_crt_init.bin ../bin/tstasmbl.com
	@echo "\nBuilt ./bin/tstasmbl.com"

../bin/chip8.com: $(filter-out %test_instruction.o, $(chip8_tms))
	@$(RELOCASSEMBLER) -o$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv memap.bin ../bin/chip8.com
	@echo "\nBuilt ./bin/chip8.com"

../bin/chip8s.com: $(filter-out %test_instruction.o, $(chip8_serial))
	@$(RELOCASSEMBLER) -o$(basename $<).bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv memap.bin ../bin/chip8s.com
	@echo "\nBuilt ./bin/chip8s.com"

../bin/tstinstr.com: $(filter-out $(exclude_from_tests),$(chip8_serial))
	@$(RELOCASSEMBLER) -otstinstr.bin -b $(ASS_FLAGS) memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv tstinstr.bin ../bin/tstinstr.com
	@echo "\nBuilt ./bin/tstinstr.com"

#-DDIAGNOSTICS_ON
%.asm: %.c
	@zcc +embedded -subtype=none -lm -clib=sdcc_iy -vn -cleanup -m -S --list $< -o $@  -create-app  -Cs --Werror --c-code-in-asm $(ZCCFLAGS)
	@echo "compiled $< to $@"

%.o: %.asm
	@$(ASSEMBLER) -o$(basename $<).bin $(ASS_FLAGS) $(basename $<).asm
	@echo "Assembled $< to $@"

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


clean:
	@git clean -xf
