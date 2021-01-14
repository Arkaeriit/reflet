# This makefile containts comands to assemble and test the
# various files of this folder.

all : clean

libtesting : libtesting.bin
	reflet-sim libtesting.bin config16

exampleIO : exampleIO.bin
	reflet-sim exampleIO.bin

primes : primes.bin
	reflet-sim primes.bin config16

fibonachi : fibonachi.bin
	reflet-sim fibonachi.bin config16

libtesting.bin : libtesting.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm 
	reflet-masm libtesting.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm -o libtesting.bin

exampleIO.bin : exampleIO.asm
	reflet-masm exampleIO.asm -o exampleIO.bin

primes.bin : primes.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm 
	reflet-masm primes.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm -o primes.bin -wordsize 16

fibonachi.bin : fibonachi.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm 
	reflet-masm fibonachi.asm libs/math.asm libs/printing.asm libs/basicIO.asm libs/string.asm -o fibonachi.bin -wordsize 16

clean :
	rm -f *.bin

