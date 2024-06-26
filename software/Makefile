# This makefile containts comands to assemble and test the
# various files of this folder.

all : libtesting.bin exampleIO.bin primes.bin fibonacci.bin test_jump.bin test_instructions.bin test_inst_mini_str.bin test_inst_mini_load.bin test_align_fix.bin test_interrupt.bin hello_world.bin test_comparison.bin test_align_trap.bin test_fancy_interrupt_use.bin basic_context_switching.bin

REFLET_SIM ?= reflet-sim

REFLET_ASM ?= reflet-asm

.PHONY: libtesting
libtesting : libtesting.bin
	$(REFLET_SIM) -c config16 libtesting.bin

exampleIO : exampleIO.bin
	$(REFLET_SIM) exampleIO.bin

primes : primes.bin
	$(REFLET_SIM) -c config16 primes.bin

fibonacci : fibonacci.bin
	$(REFLET_SIM) -c config16 fibonacci.bin

test_jump : test_jump.bin
	$(REFLET_SIM) -c config16 test_jump.bin

test_instructions : test_instructions.bin
	$(REFLET_SIM) -c config16 test_instructions.bin

test_inst_mini_str: test_inst_mini_str.bin
	$(REFLET_SIM) test_inst_mini_str.bin

test_inst_mini_load: test_inst_mini_load.bin
	$(REFLET_SIM) test_inst_mini_load.bin

libtesting.bin : libtesting.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm libs/import_all_libs.asm
	$(REFLET_ASM) libtesting.asm -D MESSAGE='"Reflet is a neat ISA!"' -word-size 16 -o libtesting.bin

exampleIO.bin : exampleIO.asm
	$(REFLET_ASM) exampleIO.asm -o exampleIO.bin -word-size 8

primes.bin : primes.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm  libs/import_all_libs.asm
	$(REFLET_ASM) primes.asm -o primes.bin -word-size 16

fibonacci.bin : fibonacci.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm libs/import_all_libs.asm
	$(REFLET_ASM) fibonacci.asm -o fibonacci.bin -word-size 16

test_jump.bin : test_jump.asm
	$(REFLET_ASM) test_jump.asm -o test_jump.bin -word-size 16 -no-stack-init -label-dump

../rtl/simu/rom03.v: test_jump.bin
	ROM_maker -name rom03 -input_file $< -output_file $@ -wordsize 2 -min_addr_size 15

test_instructions.bin : test_instructions.asm
	$(REFLET_ASM) test_instructions.asm -o test_instructions.bin -word-size 16

../rtl/simu/rom04.v: test_instructions.bin
	ROM_maker -name rom04 -input_file $< -output_file $@ -wordsize 2 -min_addr_size 14

test_inst_mini_str.bin: test_inst_mini_str.asm
	$(REFLET_ASM) test_inst_mini_str.asm -o test_inst_mini_str.bin -no-stack-init -ignore-start -word-size 8

../rtl/simu/rom01.v: test_inst_mini_str.bin
	ROM_maker -name rom01 -input_file $< -output_file $@ -wordsize 1 -min_addr_size 7

test_inst_mini_load.bin: test_inst_mini_load.asm
	$(REFLET_ASM) test_inst_mini_load.asm -o test_inst_mini_load.bin -no-stack-init -ignore-start -word-size 8

../rtl/simu/rom02.v: test_inst_mini_load.bin
	ROM_maker -name rom02 -input_file $< -output_file $@ -wordsize 1 -min_addr_size 7

test_align_fix.bin: test_align_fix.asm
	$(REFLET_ASM) test_align_fix.asm -o test_align_fix.bin -no-stack-init -ignore-start -word-size 32

.PHONY: test_align_fix
test_align_fix: test_align_fix.bin
	$(REFLET_SIM) -c config32 test_align_fix.bin

../rtl/simu/rom05.v: test_align_fix.bin
	ROM_maker -name rom05 -input_file $< -output_file $@ -wordsize 4 -min_addr_size 30

test_interrupt.bin: test_interrupt.asm
	$(REFLET_ASM) $< -o $@ -word-size 16 -set-stack-to 0x1000

test_interrupt: test_interrupt.bin
	$(REFLET_SIM) -c config16 $<

../rtl/simu/rom06.v: test_interrupt.bin
	ROM_maker -name rom06 -input_file $< -output_file $@ -wordsize 4 -min_addr_size 30

hello_world.bin : hello_world.asm
	$(REFLET_ASM) $< -o $@ -word-size 32

.PHONY: hello_world
hello_world: hello_world.bin
	$(REFLET_SIM) -w 32 -r 3000 $<

test_comparison.bin : test_comparison.asm
	$(REFLET_ASM) $< -o $@ -word-size 16

test_comparison : test_comparison.bin
	$(REFLET_SIM) -c config16 $<

test_align_trap.bin : test_align_trap.asm
	$(REFLET_ASM) $< -o $@ -word-size 16 -no-stack-init

../rtl/simu/rom07.v: test_align_trap.bin
	ROM_maker -name rom07 -input_file $< -output_file $@ -wordsize 2 -min_addr_size 15

test_align_trap : test_align_trap.bin
	$(REFLET_SIM) -w 16 -r 3000 $<

test_fancy_interrupt_use.bin : test_fancy_interrupt_use.asm
	$(REFLET_ASM) $< -o $@ -word-size 8 -no-stack-init

test_fancy_interrupt_use : test_fancy_interrupt_use.bin
	$(REFLET_SIM) -w 16 -r 256 $<

../rtl/simu/rom08.v: test_fancy_interrupt_use.bin
	ROM_maker -name rom08 -input_file $< -output_file $@ -wordsize 2 -min_addr_size 15

basic_context_switching.bin: os/tests/basic_context_switching.asm os/reflet_os.asm
	$(REFLET_ASM) $< -word-size 16 -o $@ -no-stack-init

basic_context_switching: basic_context_switching.bin
	$(REFLET_SIM) -w 16 -r 60000 $<

clean :
	rm -f *.bin

