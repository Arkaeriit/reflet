# This makefile containts comands to assemble and test the
# various files of this folder.

all : clean

libtesting : libtesting.bin
	reflet-sim libtesting.bin config16

exampleIO : exampleIO.bin
	reflet-sim exampleIO.bin

libtesting.bin : libtesting.asm
	reflet-masm libtesting.asm math.asm printing.asm -o libtesting.bin

exampleIO.bin : exampleIO.asm
	reflet-masm exampleIO.asm -o exampleIO.bin

clean :
	rm -f *.bin

