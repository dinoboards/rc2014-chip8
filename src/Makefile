.PHONY: all
all: ../bin/chip8.com ../bin/chip8asm.com

RELOCASSEMBLER=node ./apploader.js --page-align
ASSEMBLER=z80asm

# C HEADER INCLUDE DEPENDENCIES
chip8/instr_output.asm: chip8/instr_output.h datatypes.h xstdio.h chip8/systemstate.h hbios.h
chip8/main.asm: cpm.h hbios.h xstdio.h datatypes.h chip8/systemstate.h chip8/stack.h chip8/instr_output.h chip8/instr_registers.h chip8/instr_pc.h
chip8/stack.asm: chip8/stack.h datatypes.h xstdio.h chip8/systemstate.h
chip8/systemstate.asm: chip8/systemstate.h datatypes.h
chip8asm/chartesters.asm: chip8asm/chartesters.h
chip8asm/emitters.asm: chip8asm/emitters.h datatypes.h chip8asm/systemstate.h
chip8asm/expr.asm: xstdio.h chip8asm/labels.h chip8asm/expr.h chip8asm/chartesters.h datatypes.h
chip8asm/filereader.asm: chip8asm/filereader.h charconstants.h cpm.h datatypes.h
chip8asm/labels.asm: chip8asm/labels.h datatypes.h xstdio.h
chip8asm/main.asm: cpm.h hbios.h xstdio.h charconstants.h chip8asm/tokenreader.h datatypes.h chip8asm/systemstate.h chip8asm/emitters.h chip8asm/token_parser.h
chip8asm/systemstate.asm: chip8asm/systemstate.h datatypes.h
chip8asm/token_parser.asm: chip8asm/token_parser.h datatypes.h chip8asm/expr.h
chip8asm/tokenreader.asm: chip8asm/tokenreader.h chip8asm/chartesters.h datatypes.h
hbios.asm: hbios.h
xstdio.asm: xstdio.h hbios.h datatypes.h

chip8asm_sources = $(wildcard ./chip8asm/*.c) $(wildcard ./chip8asm/*.asm)
chip8asm_objects = crt.o cpm.o hbios_cio.o hbios.o memap.o nonreloccrt.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8asm_sources)))

../bin/chip8asm.com: $(chip8asm_objects)
	@$(ASSEMBLER) --output=$(basename $<).bin -mz80 -b \
	-D__SDCC -D__SDCC_IY \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
	-I"${ZCCCFG}/../../lib" \
	-L. \
	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
	-imath32 \
	--list -iz80 memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv crt_code_crt_init.bin ../bin/chip8asm.com
	@echo "\nBuilt ./bin/chip8asm.com"

chip8_sources = $(wildcard ./chip8/*.c) $(wildcard ./chip8/*.asm)
chip8_objects = memap.o relocmem.asm crt.o reloccrt.o hbios.o hbios_cio.o cpm.o xstdio.o $(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(chip8_sources)))

../bin/chip8.com:  $(chip8_objects)
	@$(RELOCASSEMBLER) --output=$(basename $<).bin -mz80 -b  \
	-D__SDCC -D__SDCC_IY \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
	-I"${ZCCCFG}/../../lib" \
	-L. \
	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
	--list -iz80 memap.o $(filter-out memap.o,$^)
	@mkdir -p ../bin
	@mv memap.bin ../bin/chip8.com
	@echo "\nBuilt ./bin/chip8.com"

#--max-allocs-per-node200000
%.asm: %.c
	@zcc +embedded -subtype=none -lm -clib=sdcc_iy -v -cleanup -m -S  --list $< -o $@  -create-app -Cs --Werror -vn
	@echo "compiled $(basename $<).c to $(basename $<).asm"

%.o: %.asm
	@$(ASSEMBLER) --output=$(basename $<).bin -mz80 \
	-D__SDCC -D__SDCC_IY \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/cpm" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/z80" \
	-I"${ZCCCFG}/../../libsrc/_DEVELOPMENT/target/hbios" \
	-I"${ZCCCFG}/../../lib" \
	-L. \
	-L"${ZCCCFG}/../../libsrc/_DEVELOPMENT/lib/sdcc_iy" \
	--list -iz80 $(basename $<).asm
	@echo "Assembled $(basename $<).asm to $(basename $<).o"

clean:
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.bin' -delete
	@find . -type f -name '*.com' -delete
	@find . -type f -name '*.err' -delete
	@find . -type f -name '*.lis' -delete
	@find . -type f -name '*.sym' -delete
	@rm -f ../bin/*
	@rm -f ./chip8/instr_output.asm
	@rm -f ./chip8/main.asm
	@rm -f ./chip8/stack.asm
	@rm -f ./chip8/systemstate.asm
	@rm -f ./chip8asm/chartesters.asm
	@rm -f ./chip8asm/emitters.asm
	@rm -f ./chip8asm/expr.asm
	@rm -f ./chip8asm/filereader.asm
	@rm -f ./chip8asm/labels.asm
	@rm -f ./chip8asm/main.asm
	@rm -f ./chip8asm/token_parser.asm
	@rm -f ./chip8asm/tokenreader.asm
	@rm -f hbios.asm
	@rm -f xstdio.asm
	@rm -f zcc_opt.def
