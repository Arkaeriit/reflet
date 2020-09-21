FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h 

mini_assembler.o : mini_assembler.h mini_assembler.c $(CONSTANTS)
	gcc -c mini_assembler.c $(FLAGS) -o mini_assembler.o

clean:
	rm -f *.o
	rm *.bin

