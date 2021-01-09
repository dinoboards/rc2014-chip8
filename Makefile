.PHONY: all
all:
	@mkdir -p ./bin
	@$(MAKE) -C ./src all -s -j 4
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

header_files = $(wildcard ./chip8asm/*.h) $(wildcard ./chip8/*.h) $(wildcard ./*.h)

.PHONY: deps
deps:
	@$(MAKE) -C ./src deps -s -j 4 -O

.PHONY: chip8asm
chip8asm:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8asm.com -s -j 4 -O
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

test: tstinstr tstasmbl

tstinstr:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstinstr.com -s -j 4 -O
	@(cd bin && cpm tstinstr)

tstasmbl:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstasmbl.com -s -j 4 -O
	@(cd bin && cpm tstasmbl)
	@./assembler.tests.js


.PHONY: chip8
chip8:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8.com -s -j 4 -O

.PHONY: clean
clean:
	@$(MAKE) -C ./src clean

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

.PHONY: ret
ret:
	@cd bin && cpm chip8asm ret.cas
	@cd bin && cpm chip8s ret.ch8

.PHONY: duplabl
duplabl:
	@cd bin && cpm chip8asm duplabl.cas

.PHONY: regs
regs:
	@cp ./test-samples/regs.cas ./bin/
	@rm -f ./test-samples/regs.ch8
	@cd bin && cpm chip8asm regs.cas
	@cd bin && cpm chip8 regs.ch8

.PHONY: symbols
symbols:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm chip8asm symbols.cas
	@cd bin && cpm "chip8 symbols.ch8"

.PHONY: draw
draw:
	@cp ./test-samples/draw.cas ./bin/
	@cd bin && cpm chip8asm draw.cas
	@cd bin && cpm "chip8 draw.ch8 -x 100000"

.PHONY: c8pic
c8pic:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm "chip8 C8PIC.ch8"

.PHONY: invaders
invaders:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm "chip8 invaders.ch8 -x 10000"

.PHONY: tictac
tictac: chip8
	@cp ./test-samples/TICTAC* ./bin/
	@cd bin && cpm "chip8 TICTAC"


.PHONY: blinky
blinky: chip8
	@cp ./test-samples/BLINKY* ./bin/
	@cd bin && cpm "chip8 BLINKY -x 2000"

.PHONY: music
music: chip8
	@cp ./test-samples/music* ./bin/
	@cd bin && cpm "chip8 music.ch8"
