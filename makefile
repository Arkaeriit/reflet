FLAGS = -Wall -g -Og #for debugging
#FLAGS = -Wall -O3 #for normal use

all : asvm asasm

#The VM
asvm : mainVM.o ReadBin.o Exec64.o stack.o
	gcc mainVM.o ReadBin.o Exec64.o stack.o $(FLAGS) -o asvm

mainVM.o : mainVM.c ReadBin.h
	gcc -c mainVM.c $(FLAGS) -o mainVM.o

ReadBin.o : ReadBin.c ReadBin.h
	gcc -c ReadBin.c $(FLAGS) -o ReadBin.o

Exec64.o : Exec64.c Exec64.h ReadBin.h
	gcc -c Exec64.c $(FLAGS) -o Exec64.o

stack.o : stack.c stack.h
	gcc -c stack.c $(FLAGS) -o stack.o

#Assembler and linker
asasm : mainASM.o assemble64.o assembleXX.o link64.o assemble64math.o assemble64jump.o
	gcc mainASM.o assemble64.o assembleXX.o link64.o assemble64math.o assemble64jump.o $(FLAGS) -o asasm

mainASM.o : mainASM.c mainASM.h
	gcc -c mainASM.c $(FLAGS) -o mainASM.o

assemble64.o : assemble64.c assemble64.h
	gcc -c assemble64.c $(FLAGS) -o assemble64.o

assemble64math.o : assemble64math.c assemble64math.h
	gcc -c assemble64math.c $(FLAGS) -o assemble64math.o

assemble64jump.o : assemble64jump.c assemble64jump.h
	gcc -c assemble64jump.c $(FLAGS) -o assemble64jump.o
assembleXX.o : assembleXX.c assembleXX.h
	gcc -c assembleXX.c $(FLAGS) -o assembleXX.o

link64.o : link64.c link64.h
	gcc -c link64.c $(FLAGS) -o link64.o

#misc
clean : 
	rm -f *.o
	rm -f asvm
	rm -f asasm

#instalation
install :
	mv -f asvm /usr/local/bin/asvm
	mv -f asasm /usr/local/bin/asasm

uninstall :
	rm -f /usr/local/bin/asvm
	rm -f /usr/local/bin/asasm

