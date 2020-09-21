FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h 

mini_assembler.bin : mini_assembler_main.o mini_assembler.o
	gcc mini_assembler.o mini_assembler_main.o -o mini_assembler.bin

mini_assembler.o : mini_assembler.h mini_assembler.c $(CONSTANTS)
	gcc -c mini_assembler.c $(FLAGS) -o mini_assembler.o

mini_assembler_main.o : mini_assembler_main.c
	gcc -c mini_assembler_main.c $(FLAGS) -o mini_assembler_main.o

clean:
	rm -f *.o
	rm *.bin

