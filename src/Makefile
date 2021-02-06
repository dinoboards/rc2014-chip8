SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

VIDEO ?= V9958

.PHONY: all
all: ../bin/chip8-${VIDEO}.com ../bin/chip8asm.com ../bin/tstasmbl.com ../bin/tstinstr.com

MEASURE=
#-D_MEASURE_PERFORMANCE

LINKER_FLAGS=-mz80 \
	-D__SDCC -D__SDCC_IY ${MEASURE} \
	-D${VIDEO} \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
	-I"${ZCCCFG}/../../lib" \
	-I"chip8/" \
	-L. \
	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
	-lmath32 \
	-lcpm \
	-mz80 \
	-opt-speed

ZSDCPP_FLAGS=-iquote"." \
	-D__Z88DK -D__EMBEDDED -DEMBEDDED -D__EMBEDDED_Z80 -D__Z80 -DZ80 \
	${MEASURE} \
	-D${VIDEO} \
	-D__SDCC -D__SDCC_IY \
	-D__SDCC 						\
	-isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc"

ZCCRELFLAGS=

ifdef RELEASE
ZCCRELFLAGS=" -SO3 --max-allocs-per-node200000 -Cs\"--opt-code-speed\" --allow-unsafe-read"
endif

ZCCFLAGS="${MEASURE} ${ZCCRELFLAGS} "

ASSEMBLER=zcc +embedded -subtype=none -D${VIDEO} -Ca="-opt-speed" -c -Ca="-I./chip8/"
CCOMP=zcc +embedded -subtype=none -lm -compiler=sdcc -clib=sdcc_iy -m -S -Cs --Werror --c-code-in-asm $(ZCCFLAGS)
LINK_WITH_RELOCATOR=node ./apploader.js --page-align
LINKER=z80asm

include ./depends.d

chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
chip8asm_objects = crt.o memap.o cpm.o cpmasm.o hbios_cio.o hbios.o memap.o nonreloccrt.o xstdio.o chartesters.o token.o filereader.o xstrtol.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))
exclude_from_tests = %main.o %filereader.o %exit.o %error.o %tty.o %keys.o %systimer.o %random.o %ym2149.o %ym2149asm.o
exclude_from_tstinstr = $(exclude_from_tests)  %error_reports.o

chip8_sources = $(wildcard ./chip8/*.c) $(wildcard ./chip8/*.asm) $(wildcard ./chip8/instr_v9958_output/*.asm)
chip8_objects = memap.o relocmem.asm crt.o reloccrt.o hbios.o hbios_cio.o hbios_snd.o cpm.o cpmasm.o xstdio.o chartesters.o token.o filereader.o xstrtol.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8_sources)))

../bin/chip8asm.com: $(filter-out %test_assembler.o, $(chip8asm_objects))
	@$(LINKER) -o$(basename $<).bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
	mkdir -p ../bin
	mv crt_code_crt_init.bin ../bin/chip8asm.com
	echo -e "\nBuilt ./bin/chip8asm.com"

../bin/tstasmbl.com: test_expectations.o $(filter-out $(exclude_from_tests),$(chip8asm_objects))
	@$(LINKER) -otstasmbl.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
	mkdir -p ../bin
	mv tstasmbl_code_crt_init.bin ../bin/tstasmbl.com
	echo -e "\nBuilt ./bin/tstasmbl.com"

../bin/chip8-${VIDEO}.com: $(filter-out ./chip8/test_%.o, $(chip8_objects))
	@mkdir -p ../bin
	$(LINK_WITH_RELOCATOR) -ochip8-${VIDEO}.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
	mv chip8-${VIDEO}.bin ../bin/chip8-${VIDEO}.com
	cp ../bin/chip8-${VIDEO}.com ../bin/chip8.com
	echo -e "\nBuilt ./bin/chip8-${VIDEO}.com (./bin/chip8.com)"

../bin/tstinstr.com: test_expectations.o $(filter-out $(exclude_from_tstinstr),$(chip8_objects))
	@$(LINK_WITH_RELOCATOR) -otstinstr.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
	mkdir -p ../bin
	mv tstinstr.bin ../bin/tstinstr.com
	echo -e "\nBuilt ./bin/tstinstr.com"

%.asm: %.c
	@$(CCOMP) $< -o $@
	echo "compiled $< to $@"

%.o: %.asm
	@$(ASSEMBLER) $<
	echo "Assembled $< to $@"

deps:
	@echo "" > ./depends.d
	find -name "*.c" | while read -r file; do
		file_no_ext="$${file%.*}"
		filename=$$(basename $$file_no_ext)
		from="$$filename.rel"
		to="$$file_no_ext.asm"
		zsdcpp ${ZSDCPP_FLAGS} -MM -MF /tmp/deps.deps $$file
		sed "s+$$from+$$to+g" /tmp/deps.deps >> ./depends.d
	done
	echo "./depends.d created"

clean:
	@git clean -Xf
