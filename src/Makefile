SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

TARGET ?= msx
CPMBIN := ./bin/cpm/
BIN := ./bin/$(TARGET)/
SRC := ./
APPS := hello tstinstr
TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

MEASURE=
#-D_MEASURE_PERFORMANCE

ZCCRELFLAGS=
ifdef RELEASE
ZCCRELFLAGS=-SO3 --max-allocs-per-node200000 -Cs"--opt-code-speed" --allow-unsafe-read --opt-code-speed
endif

ifeq ($(TARGET),msx)
ZCC := zcc +msx -subtype=msxdos2 -compiler=sdcc -Cs --Werror --list -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 -Ca="-I./chip8/" -Cm="-I./chip8/" $(ZCCRELFLAGS)
else
ZCC := zcc +cpm  -compiler=sdcc $(ZCCRELFLAGS) -Cs --Werror --list -pragma-redirect:fputc_cons=fputc_cons_native  -Ca="-I./chip8/" -Cm="-I./chip8/" -pragma-define:noprotectmsdos -pragma-define:CRT_ENABLE_COMMANDLINE=1 -pragma-define:CRT_COMMANDLINE_REDIRECTION=0 -pragma-define:CRT_OPEN_MAX=1
endif
 #-Ca="-opt-speed"

chip8_sources = $(wildcard chip8/*.c) $(wildcard chip8/*.asm) $(wildcard chip8/*.asm.m4) $(wildcard chip8/instr_v9958_output/*.asm) $(wildcard chip8/instr_v9958_output/*.asm.m4)
chip8asm_sources = $(wildcard chip8asm/*.c) $(wildcard chip8asm/*.asm)
exclude_from_tests = %main.c.o %filereader.c.o %exit.c.o %error.o %tty.c.o %keys.c.o %random.c.o %ym2149.c.o %ym2149asm.o
exclude_from_tstinstr = $(exclude_from_tests) %error_reports.c.o
chip8_objects = filenames.c.o debug.o hbios.c.o hbios_cio.o hbios_snd.o xstdio.o xstdio.c.o chartesters.c.o token.c.o filereader.c.o xstrtol.o $(patsubst %.asm.m4,%.m4.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.c.o,$(chip8_sources))))
chip8asm_objects = filenames.c.o xstdio.o chartesters.c.o token.c.o filereader.c.o xstrtol.o $(patsubst %.asm,%.o,$(patsubst %.c,%.c.o,$(chip8asm_sources)))


.PHONY: all
all: $(TARGETS)

define assemble
	@mkdir -p $(dir $@)
	$(ZCC) --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define compile
	@mkdir -p $(dir $@)
	$(ZCC) --c-code-in-asm  --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

$(BIN)tstinstr100.bin: $(addprefix $(BIN),test_expectations.c.o $(filter-out $(exclude_from_tstinstr),$(chip8_objects)))
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 256  $^ -o $(BIN)tstinstr100.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)tstinstr200.bin: $(addprefix $(BIN),test_expectations.c.o $(filter-out $(exclude_from_tstinstr),$(chip8_objects)))
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 512  $^ -o $(BIN)tstinstr200.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)tstinstrrelocation-map.bin: $(addprefix $(BIN),tstinstr100.bin tstinstr200.bin)
	@node ./extract-relocation-map.js $^ $(BIN)tstinstrrelocation-map.bin

$(BIN)tstinstr.loader.asm: loader.asm.m4
	@m4 -D SRC=$(TARGET)/tstinstr $^ > $@

$(BIN)tstinstr.com: $(addprefix $(BIN),tstinstr100.bin tstinstr200.bin tstinstr.loader.asm tstinstrrelocation-map.bin)
	z80asm -r=256 -o"$@" -b "$(BIN)tstinstr.loader.asm"
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"

.PRECIOUS: $(BIN)%.c.asm
$(BIN)%.c.asm: %.c; $(compile)

.PRECIOUS: $(BIN)chip8/%.c.asm
$(BIN)chip8/%.c.asm: chip8/%.c; $(compile)

.PRECIOUS: $(BIN)chip8/instr_v9958_output/%.c.asm
$(BIN)chip8/instr_v9958_output/%.c.asm: chip8/instr_v9958_output/%.c; $(compile)

$(BIN)%.o: $(BIN)%.asm; $(assemble)
$(BIN)chip8/%.o: $(BIN)chip8/%.asm; $(assemble)
$(BIN)chip8asm/%.o: $(BIN)chip8asm/%.asm; $(assemble)
$(BIN)%.o: %.asm; $(assemble)
$(BIN)%.m4.o: %.asm.m4; $(assemble)
$(BIN)chip8/instr_v9958_output/%.o: $(BIN)chip8/instr_v9958_output/%.asm; $(assemble)


$(BIN)tstasmbl.com: $(addprefix $(BIN),test_expectations.c.o $(filter-out $(exclude_from_tests),$(chip8asm_objects)))
	mkdir -p $(dir $@)
	$(ZCC) -zorg 256 --list  $^ -o $(BIN)tstasmbl.com
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)chip8asm.com: $(filter-out %test_assembler.c.o, $(addprefix $(BIN),$(chip8asm_objects)))
	mkdir -p $(dir $@)
	$(ZCC) -zorg 256 --list  $^ -o $(BIN)chip8asm.com
	echo "Compiled $(notdir $@) from $(notdir $^)"






# $(BIN)chip8.com: $(filter-out ./bin/msx/chip8/test_%.c.o, $(addprefix $(BIN),$(chip8_objects)))
# 	mkdir -p $(dir $@)
# 	$(ZCC) -zorg 256 --list  $^ -o $(BIN)chip8.com
# 	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)chip8100.bin:$(filter-out ./bin/msx/chip8/test_%.c.o, $(addprefix $(BIN),$(chip8_objects)))
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 256  $^ -o $(BIN)chip8100.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)chip8200.bin: $(filter-out ./bin/msx/chip8/test_%.c.o, $(addprefix $(BIN),$(chip8_objects)))
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 512  $^ -o $(BIN)chip8200.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)chip8relocation-map.bin: $(addprefix $(BIN),chip8100.bin chip8200.bin)
	@node ./extract-relocation-map.js $^ $(BIN)chip8relocation-map.bin

$(BIN)chip8.loader.asm: loader.asm.m4
	@m4 -D SRC=$(TARGET)/chip8 $^ > $@

$(BIN)chip8.com: $(addprefix $(BIN),chip8100.bin chip8200.bin chip8.loader.asm chip8relocation-map.bin)
	z80asm -r=256 -o"$@" -b "$(BIN)chip8.loader.asm"
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"









$(BIN)test.com: $(addprefix $(BIN),test.c.o chip8/msx.c.o)
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 256  $^ -o $(BIN)test.com
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)hello100.bin: $(addprefix $(BIN),hello.c.o chip8/hello2.c.o hello.m4.o)
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 256  $^ -o $(BIN)hello100.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)hello200.bin: $(addprefix $(BIN),hello.c.o chip8/hello2.c.o hello.m4.o)
	@mkdir -p $(dir $@)
	$(ZCC) -zorg 512  $^ -o $(BIN)hello200.bin
	echo "Compiled $(notdir $@) from $(notdir $^)"

$(BIN)hello.loader.asm: loader.asm.m4
	@m4 -D SRC=$(TARGET)/hello $^ > $@

$(BIN)hello.com: $(addprefix $(BIN),hello100.bin hello200.bin hello.loader.asm)
	@node ./extract-relocation-map.js $^
	z80asm -r=256 -o"$@" -b "$(BIN)hello.loader.asm"
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"











# chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
# chip8asm_objects = xstdio.o chartesters.o token.o filereader.o xstrtol.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))
# exclude_from_tests = %main.o %filereader.o %exit.o %error.o %tty.o %keys.o %systimer.o %random.o %ym2149.o %ym2149asm.o
# exclude_from_tstinstr = $(exclude_from_tests)  %error_reports.o


# $(BIN)tstinstr.com: test_expectations.o $(filter-out $(exclude_from_tstinstr),$(chip8_objects))

# $(TARGETS):
# 	@mkdir -p $(dir $@)
# 	$(ZCC) $(filter-out %.inc,$(filter-out %.h,$^)) -o $(notdir $@)
# 	mv $(notdir $@) "$@"
# 	rm -f $(basename $(notdir $@)).img
# 	rm -f $(basename $(notdir $@))_BSS.bin
# 	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"





# SHELL := bash
# .SHELLFLAGS := -eu -o pipefail -c
# .ONESHELL:
# MAKEFLAGS += --warn-undefined-variables
# MAKEFLAGS += --no-builtin-rules

# .PHONY: all
# all: ../bin/chip8.com ../bin/chip8asm.com ../bin/tstasmbl.com ../bin/tstinstr.com

# MEASURE=
# #-D_MEASURE_PERFORMANCE

# LINKER_FLAGS=-mz80 \
# 	-D__SDCC -D__SDCC_IY ${MEASURE} \
# 	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
# 	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
# 	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
# 	-I"${ZCCCFG}/../../lib" \
# 	-I"chip8/" \
# 	-L. \
# 	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
# 	-lmath32 \
# 	-lcpm \
# 	-mz80 \
# 	-opt-speed


# ZCCRELFLAGS=


# ZCCFLAGS="${MEASURE} ${ZCCRELFLAGS} "

# ASSEMBLER=zcc +embedded -subtype=none -Ca="${MEASURE}" -Ca="-opt-speed" -c -Ca="-I./chip8/" -Cm="-I./chip8/"
# CCOMP=zcc +embedded -subtype=none -lm -compiler=sdcc -clib=sdcc_iy -m -S -Cs --Werror --c-code-in-asm $(ZCCFLAGS)
# LINK_WITH_RELOCATOR=node ./apploader.js --page-align
# LINKER=z80asm

include ./cpm-depends.d
include ./msx-depends.d

# chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
# chip8asm_objects = crt.o memap.o cpm.o cpmasm.o hbios_cio.o hbios.o memap.o nonreloccrt.o xstdio.o chartesters.o token.o filereader.o xstrtol.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))
# exclude_from_tests = %main.o %filereader.o %exit.o %error.o %tty.o %keys.o %systimer.o %random.o %ym2149.o %ym2149asm.o
# exclude_from_tstinstr = $(exclude_from_tests)  %error_reports.o

# chip8_sources = $(wildcard ./chip8/*.c) $(wildcard ./chip8/*.asm) $(wildcard ./chip8/*.asm.m4) $(wildcard ./chip8/instr_v9958_output/*.asm) $(wildcard ./chip8/instr_v9958_output/*.asm.m4)
# chip8_objects = memap.o relocmem.asm crt.o reloccrt.o hbios.o hbios_cio.o hbios_snd.o cpm.o cpmasm.o xstdio.o chartesters.o token.o filereader.o xstrtol.o $(patsubst %.asm.m4,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8_sources))))

# ../bin/chip8asm.com: $(filter-out %test_assembler.o, $(chip8asm_objects))
# 	@$(LINKER) -o$(basename $<).bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
# 	mkdir -p ../bin
# 	mv crt_code_crt_init.bin ../bin/chip8asm.com
# 	echo -e "\nBuilt ./bin/chip8asm.com"

# ../bin/tstasmbl.com: test_expectations.o $(filter-out $(exclude_from_tests),$(chip8asm_objects))
# 	@$(LINKER) -otstasmbl.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
# 	mkdir -p ../bin
# 	mv tstasmbl_code_crt_init.bin ../bin/tstasmbl.com
# 	echo -e "\nBuilt ./bin/tstasmbl.com"

# ../bin/chip8.com: $(filter-out ./chip8/test_%.o, $(chip8_objects))
# 	@mkdir -p ../bin
# 	$(LINK_WITH_RELOCATOR) -ochip8.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
# 	mv chip8.bin ../bin/chip8.com
# 	echo -e "\nBuilt ./bin/chip8.com"

# ../bin/tstinstr.com: test_expectations.o $(filter-out $(exclude_from_tstinstr),$(chip8_objects))
# 	@$(LINK_WITH_RELOCATOR) -otstinstr.bin -b $(LINKER_FLAGS) memap.o $(filter-out memap.o,$^)
# 	mkdir -p ../bin
# 	mv tstinstr.bin ../bin/tstinstr.com
# 	echo -e "\nBuilt ./bin/tstinstr.com"


# %.asm: %.c
# 	@$(CCOMP) $< -o $@
# 	echo "compiled $< to $@"

# %.o: %.asm.m4
# 	@$(ASSEMBLER) $<
# 	echo "Assembled $< to $@"

# %.o: %.asm
# 	@$(ASSEMBLER) $<
# 	echo "Assembled $< to $@"

ZSDCPP_FLAGS=-iquote"." -isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc"

deps:
	@echo "" > ./$(TARGET)-depends.d
	# C Dependencies
	find -name "*.c" | while read -r file; do
		file_no_ext="$${file%.*}"
		file_no_ext=$${file_no_ext#./}
		filename=$$(basename $$file_no_ext)
		from="$$filename.rel"
		to="$(BIN)$$file_no_ext.c.asm"
		zsdcpp ${ZSDCPP_FLAGS} -MM -MF /tmp/deps.deps $$file
		sed "s+$$from+$$to+g" /tmp/deps.deps >> ./$(TARGET)-depends.d
	done

	# ASM include dependencies
	find -name "*.asm" -not -path "./bin/*" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		printf "$(BIN)$$file_no_ext.o:" >> ./$(TARGET)-depends.d
		printf " $$file" >> ./$(TARGET)-depends.d
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./$(TARGET)-depends.d
		done
		echo "" >> ./$(TARGET)-depends.d
	done

	# ASM include dependencies
	find -name "*.asm.m4" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		file_no_ext="$(BIN)$${file_no_ext%.*}"
		printf "$$file_no_ext.o:" >> ./$(TARGET)-depends.d
		printf " $$file" >> ./$(TARGET)-depends.d
		sed  -n '/include/s/^[\t ]*include(.\(.*\)'\'')/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./$(TARGET)-depends.d
		done
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./$(TARGET)-depends.d
		done
		echo "" >> ./$(TARGET)-depends.d
	done

	echo "./$(TARGET)-depends.d created"


clean:
	@rm -rf bin
	rm *.asm.lis
