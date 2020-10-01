FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h
LUA = -llua -lm -ldl   

all : asrmmasm.bin asrmpasm.bin

asrmmasm.bin : macro_assembler.o pico_assembler.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc pico_assembler.o macro_assembler.o $(LUA) -o asrmmasm.bin

asrmpasm.bin : pico_assembler_main.o pico_assembler.o
	gcc pico_assembler.o pico_assembler_main.o -o asrmpasm.bin

pico_assembler.o : pico_assembler.h pico_assembler.c $(CONSTANTS)
	gcc -c pico_assembler.c $(FLAGS) -o pico_assembler.o

pico_assembler_main.o : pico_assembler_main.c
	gcc -c pico_assembler_main.c $(FLAGS) -o pico_assembler_main.o

macro_assembler.o : macro_assembler.c
	gcc -c macro_assembler.c $(FLAGS) -o macro_assembler.o

lib.luac : lib.lua
	luac -o lib.luac lib.lua

link.luac : link.lua
	luac -o link.luac link.lua

macro.luac : macro.lua
	luac -o macro.luac macro.lua

readingCode.luac : readingCode.lua
	luac -o readingCode.luac readingCode.lua

macro_assembler.luac : macro_assembler.lua
	luac -o macro_assembler.luac macro_assembler.lua

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.luac

