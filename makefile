FLAGS = -Wall -g

all : asvm

asvm : main.o ReadBin.o
	gcc main.o ReadBin.o $(FLAGS) -o asvm

main.o : main.c ReadBin.h
	gcc -c main.c $(FLAGS) -o main.o

ReadBin.o : ReadBin.c ReadBin.h
	gcc -c ReadBin.c $(FLAGS) -o ReadBin.o

clean : 
	rm -f *.o
	rm -f asvm
