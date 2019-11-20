FLAGS = -Wall -g

all : asvm asasm

asvm : mainVM.o ReadBin.o Exec64.o
	gcc mainVM.o ReadBin.o Exec64.o $(FLAGS) -o asvm

mainVM.o : mainVM.c ReadBin.h
	gcc -c mainVM.c $(FLAGS) -o mainVM.o

ReadBin.o : ReadBin.c ReadBin.h
	gcc -c ReadBin.c $(FLAGS) -o ReadBin.o

Exec64.o : Exec64.c Exec64.h opperand.h ReadBin.h
	gcc -c Exec64.c $(FLAGS) -o Exec64.o

asasm : mainASM.o assemble64.o
	gcc mainASM.o assemble64.o $(FLAGS) -o asasm

mainASM.o : mainASM.c mainASM.h
	gcc -c mainASM.c $(FLAGS) -o mainASM.o

assemble64.o : assemble64.c assemble64.h
	gcc -c assemble64.c $(FLAGS) -o assemble64.o

clean : 
	rm -f *.o
	rm -f asvm
	rm -f asasm

