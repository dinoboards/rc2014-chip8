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
	$(MAKE) -C ./src ../bin/chip8asm.com -s -j 4 -O

test: tstinstr tstasmbl

tstinstr:
	@mkdir -p ./bin
	TARGET=cpm $(MAKE) -C ./src ./bin/cpm/tstinstr.com -s -j 4 -O
	cp -u ./src/bin/cpm/tstinstr.com ./bin
	(cd bin && cpm tstinstr)

tstasmbl:
	@mkdir -p ./bin
	$(MAKE) -C ./src ../bin/tstasmbl.com -s -j 4 -O
	(cd bin && cpm tstasmbl)
	cp ./test-samples/draw.cas ./bin/
	./assembler.tests.js

.PHONY: chip8
chip8:
	@mkdir -p ./bin
	$(MAKE) -C ./src ../bin/chip8.com -s -j 4 -O

.PHONY: clean
clean:
	@$(MAKE) -C ./src clean
	rm -rf bin/*

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

.PHONY: testxor
testxor:
	@cp ./test-samples/testxor.cas ./bin/
	cd bin && cpm chip8asm testxor.cas

.PHONY: testscrl
testscrl:
	@cp ./test-samples/testscrl.cas ./bin/
	cd bin && cpm chip8asm testscrl.cas

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
	git clone --depth 1 git@github.com:jhallen/cpm.git
	cd cpm
	OS=linux MAKEFLAGS= make -B

.PHONY: deps
deps:
	TARGET=cpm $(MAKE) -s -C ./src deps
	TARGET=msx $(MAKE) -s -C ./src deps
