.PHONY: all
all:
	@mkdir -p ./bin
	@$(MAKE) -C ./src all -s -j 4
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

header_files = $(wildcard ./chip8asm/*.h) $(wildcard ./chip8/*.h) $(wildcard ./*.h)

.PHONY: deps
deps:
	@$(MAKE) -C ./src deps -s -j 4

.PHONY: chip8asm
chip8asm:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8asm.com -s -j 4
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

test:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstinstr.com ../bin/tstasmbl.com -s -j 4
	@(cd bin && cpm tstasmbl && cpm tstinstr)

tstinstr:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstinstr.com -s -j 4
	@(cd bin && cpm tstinstr)

tstasmbl:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstasmbl.com -s -j 4
	@(cd bin && cpm tstasmbl)


.PHONY: chip8
chip8:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8.com -s -j 4

.PHONY: chip8s
chip8s:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8s.com -s -j 4

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
	@cd bin && cpm chip8s regs.ch8

.PHONY: symbols
symbols:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm chip8asm symbols.cas
	@cd bin && cpm chip8s symbols.ch8

.PHONY: draw
draw:
	@cp ./test-samples/draw.cas ./bin/
	@rm -f ./test-samples/draw.ch8
	@cd bin && cpm chip8asm draw.cas
	@cd bin && cpm chip8s draw.ch8

.PHONY: c8pic
c8pic:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm chip8s C8PIC.ch8

.PHONY: invaders
invaders:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm chip8s invaders.ch8

.PHONY: chip8s
tictac: chip8s
	@cp ./test-samples/TICTAC ./bin/
	@cd bin && cpm chip8s TICTAC
