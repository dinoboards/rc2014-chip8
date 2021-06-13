SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

export PATH := $(PWD)/tools/prereq/cpm/:$(PATH)

.PHONY: all
all:
	@mkdir -p ./bin
	$(MAKE) -C ./src all -s -j 4

header_files = $(wildcard ./chip8asm/*.h) $(wildcard ./chip8/*.h) $(wildcard ./*.h)

.PHONY: chip8asm
chip8asm:
	@mkdir -p ./bin
	TARGET=cpm $(MAKE) -C ./src ./bin/cpm/chip8asm.com -s -j 6 -O
	cp -u ./src/bin/cpm/chip8asm.com ./bin

.PHONY: chip8
chip8:
	@mkdir -p ./bin
	TARGET=msx $(MAKE) -C ./src ./bin/msx/chip8.com -s -j 6 -O
	cp -u ./src/bin/msx/chip8.com ./bin

cpmchip8:
	@mkdir -p ./bin
	TARGET=cpm $(MAKE) -C ./src ./bin/cpm/chip8.com -s -j 6 -O
	cp -u ./src/bin/cpm/chip8.com ./bin/cpmchip8.com

test: tstinstr tstasmbl

tstinstr:
	@mkdir -p ./bin
	TARGET=cpm $(MAKE) -C ./src ./bin/cpm/tstinstr.com -s -j 6 -O
	cp -u ./src/bin/cpm/tstinstr.com ./bin
	(cd bin && cpm tstinstr)

tstasmbl: chip8asm
	@mkdir -p ./bin
	TARGET=cpm $(MAKE) -C ./src ./bin/cpm/tstasmbl.com -s #-j 4 -O
	cp -u ./src/bin/cpm/tstasmbl.com ./bin
	(cd bin && cpm tstasmbl)
	cp ./test-samples/draw.cas ./bin/
	./assembler.tests.js

.PHONY: clean
clean:
	@$(MAKE) -C ./src clean
	rm -rf bin/*

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

.PHONY: testxor
testxor: chip8asm
	@cp ./test-samples/testxor.cas ./bin/
	cd bin && cpm chip8asm testxor.cas
.PHONY1: test1
test1: chip8asm
	@cp ./test-samples/test1.cas ./bin/
	cd bin && cpm chip8asm test1.cas
.PHONY1: test2
test2: chip8asm
	@cp ./test-samples/test2.cas ./bin/
	cd bin && cpm chip8asm test2.cas
.PHONY1: testscrl
testscrl: chip8asm
	@cp ./test-samples/testscrl.cas ./bin/
	cd bin && cpm chip8asm testscrl.cas

test-super: cpmchip8
	@cp ./test-samples/super.ch8 ./bin/
	cd bin && cpm cpmchip8 super.ch8s

.PHONY: package
package: chip8 chip8asm
	@mkdir -p package
	cp ./bin/chip8.com ./package/
	cp ./bin/chip8asm.com ./package/
	cp ./test-samples/chick.* ./package/
	cp ./test-samples/f8z.* ./package/
	cp ./test-samples/super.* ./package/
	cp ./test-samples/invaders.* ./package/
	cp ./test-samples/garlic.* ./package/
	zip -Dj chip-8-$${VERSION}.zip package/*

./tools/cpm/cpm:
	@mkdir -p ./tools/prereq/
	cd ./tools/prereq
	git clone --branch dean/z80-undoc-instr --depth 1 git@github.com:vipoo/cpm.git
	cd cpm
	OS=linux MAKEFLAGS= make -B

.PHONY: deps
deps:
	TARGET=cpm $(MAKE) -s -C ./src deps
	TARGET=msx $(MAKE) -s -C ./src deps
