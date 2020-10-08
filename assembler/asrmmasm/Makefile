FLAGS = -Wall -Werror -g
LUA = -llua -lm -ldl   
ASRMPASM = -lasrmpasm
LIBPATH = /usr/lib64
INCLUDEPATH = /usr/include

all : asrmmasm.bin libasrmmasm.so

libasrmmasm.so : libasrmmasm.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc -shared libasrmmasm.o $(LUA) $(ASRMPASM) -o libasrmmasm.so

asrmmasm.bin : macro_assembler.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc macro_assembler.o $(LUA) $(ASRMPASM) -o asrmmasm.bin

macro_assembler.o : macro_assembler.c
	gcc -c macro_assembler.c $(FLAGS) -o macro_assembler.o

libasrmmasm.o : libasrmmasm.c libasrmmasm.h
	gcc -c -fPIC libasrmmasm.c $(FLAGS) -o libasrmmasm.o

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

install : install_macro install_lib

install_macro :
	mkdir -p /usr/local/share/asrm
	cp -f asrmmasm.bin /usr/local/bin/asrmmasm
	cp -f lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac /usr/local/share/asrm/

install_lib :
	mkdir -p $(LIBPATH) $(INCLUDEPATH)
	cp -f libasrmmasm.h $(INCLUDEPATH)/asrmmasm.h
	cp -f libasrmmasm.so $(LIBPATH)/libasrmmasm.so

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.so
	rm -f *.luac

