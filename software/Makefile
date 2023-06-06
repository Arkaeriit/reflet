# This makefile containts comands to assemble and test the
# various files of this folder.

all : libtesting.bin exampleIO.bin primes.bin fibonacci.bin test_jump.bin test_instructions.bin test_inst_mini_str.bin test_inst_mini_load.bin test_align_fix.bin test_interrupt.bin

.PHONY: libtesting
libtesting : libtesting.bin
	reflet-sim -c config16 libtesting.bin

exampleIO : exampleIO.bin
	reflet-sim exampleIO.bin

primes : primes.bin
	reflet-sim -c config16 primes.bin

fibonacci : fibonacci.bin
	reflet-sim -c config16 fibonacci.bin

test_jump : test_jump.bin
	reflet-sim -c config16 test_jump.bin

test_instructions : test_instructions.bin
	reflet-sim -c config16 test_instructions.bin

test_inst_mini_str: test_inst_mini_str.bin
	reflet-sim test_inst_mini_str.bin

test_inst_mini_load: test_inst_mini_load.bin
	reflet-sim test_inst_mini_load.bin

libtesting.bin : libtesting.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm libs/import_all_libs.asm
	reflet-asm libtesting.asm -word-size 16 -o libtesting.bin

exampleIO.bin : exampleIO.asm
	reflet-asm exampleIO.asm -o exampleIO.bin -word-size 8

primes.bin : primes.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm  libs/import_all_libs.asm
	reflet-asm primes.asm -o primes.bin -word-size 16

fibonacci.bin : fibonacci.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm libs/import_all_libs.asm
	reflet-asm fibonacci.asm -o fibonacci.bin -word-size 16

test_jump.bin : test_jump.asm
	reflet-asm test_jump.asm -o test_jump.bin -word-size 16 -no-stack-init -label-dump

test_instructions.bin : test_instructions.asm
	reflet-asm test_instructions.asm -o test_instructions.bin -word-size 16

test_inst_mini_str.bin: test_inst_mini_str.asm
	reflet-asm test_inst_mini_str.asm -o test_inst_mini_str.bin -no-stack-init -ignore-start -word-size 8

test_inst_mini_load.bin: test_inst_mini_load.asm
	reflet-asm test_inst_mini_load.asm -o test_inst_mini_load.bin -no-stack-init -ignore-start -word-size 8

test_align_fix.bin: test_align_fix.asm
	reflet-asm test_align_fix.asm -o test_align_fix.bin -no-stack-init -ignore-start -word-size 32

.PHONY: test_align_fix
test_align_fix: test_align_fix.bin
	reflet-sim -c config32 test_align_fix.bin

../rtl/simu/rom11.v: test_align_fix.bin
	ROM_maker -name rom11 -input_file $< -output_file $@ -wordsize 4 -min_addr_size 30

test_interrupt.bin: test_interrupt.asm
	reflet-asm $< -o $@ -word-size 16

test_interrupt: test_interrupt.bin
	reflet-sim -c config16 $<

hello_world.bin : hello_world.asm
	reflet-asm $< -o $@ -word-size 32

.PHONY: hello_world
hello_world: hello_world.bin
	reflet-sim -w 32 -r 3000 $<

clean :
	rm -f *.bin

