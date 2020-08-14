.PHONY: all
all:
	@mkdir -p ./bin
	@$(MAKE) -C ./src all -s -j 4
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

.PHONY: chip8asm
chip8asm:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8asm.com -s -j 4
	@cp ./test-samples/*.cas ./bin/
	@cp ./test-samples/*.ch8 ./bin/

test:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/tstasmbl.com -s -j 4
	@(cd bin && cpm tstasmbl)

.PHONY: chip8
chip8:
	@mkdir -p ./bin
	@$(MAKE) -C ./src ../bin/chip8.com -s -j 4

.PHONY: clean
clean:
	@$(MAKE) -C ./src clean

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

.PHONY: cleanbin
cleanbin:
	@rm -f ./bin/*.cas
	@rm -f ./bin/*.ch8

.PHONY: spike
spike:
	@cd bin && cpm chip8asm spike.cas
	@cd bin && cpm chip8 spike.ch8

.PHONY: ret
ret:
	@cd bin && cpm chip8asm ret.cas
	@cd bin && cpm chip8 ret.ch8

.PHONY: duplabl
duplabl:
	@cd bin && cpm chip8asm duplabl.cas

.PHONY: regs
regs:
	@cd bin && cpm chip8asm regs.cas
	@cd bin && cpm chip8 regs.ch8

.PHONY: symbols
symbols:
	@cd bin && cpm chip8asm symbols.cas
	@cd bin && cpm chip8 symbols.ch8

.PHONY: draw
draw:
	@cd bin && cpm chip8asm draw.cas
	@cd bin && cpm chip8 draw.ch8

.PHONY: c8pic
c8pic:
	@cp ./test-samples/*.ch8 ./bin/
	@cd bin && cpm chip8 C8PIC.ch8
