FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h
LUA = -llua -lm -ldl   
ASRMPASM = -lasrmpasm

all : asrmmasm.bin

asrmmasm.bin : macro_assembler.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc macro_assembler.o $(LUA) $(ASRMPASM) -o asrmmasm.bin

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

install : install_pico install_macro

install_pico :
	cp -f asrmpasm.bin /usr/local/bin/asrmpasm

install_macro :
	mkdir -p /usr/local/share/asrm
	cp -f asrmmasm.bin /usr/local/bin/asrmmasm
	cp -f lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac /usr/local/share/asrm/

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.luac

