FLAGS = -Wall -g

all : asvm

asvm : main.o ReadBin.o Exec64.o
	gcc main.o ReadBin.o Exec64.o $(FLAGS) -o asvm

main.o : main.c ReadBin.h
	gcc -c main.c $(FLAGS) -o main.o

ReadBin.o : ReadBin.c ReadBin.h
	gcc -c ReadBin.c $(FLAGS) -o ReadBin.o

Exec64.o : Exec64.c Exec64.h opperand.h ReadBin.h
	gcc -c Exec64.c $(FLAGS) -o Exec64.o

clean : 
	rm -f *.o
	rm -f asvm
