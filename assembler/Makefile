FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h 

pico_assembler.bin : pico_assembler_main.o pico_assembler.o
	gcc pico_assembler.o pico_assembler_main.o -o pico_assembler.bin

pico_assembler.o : pico_assembler.h pico_assembler.c $(CONSTANTS)
	gcc -c pico_assembler.c $(FLAGS) -o pico_assembler.o

pico_assembler_main.o : pico_assembler_main.c
	gcc -c pico_assembler_main.c $(FLAGS) -o pico_assembler_main.o

clean:
	rm -f *.o
	rm -f *.bin

