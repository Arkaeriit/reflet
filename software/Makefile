# This makefile containts comands to assemble and test the
# various files of this folder.

all : clean

libtesting : libtesting.bin
	reflet-sim libtesting.bin config16

exampleIO : exampleIO.bin
	reflet-sim exampleIO.bin

primes : primes.bin
	reflet-sim primes.bin config16

fibonacci : fibonacci.bin
	reflet-sim fibonacci.bin config16

test_jump : test_jump.bin
	reflet-sim test_jump.bin config16

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

clean :
	rm -f *.bin

