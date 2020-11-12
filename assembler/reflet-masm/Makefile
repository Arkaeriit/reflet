FLAGS = -Wall -Werror -g
LUA = -llua -lm -ldl   
REFLETPASM = -lreflet-pasm
LIBPATH = /usr/lib64
INCLUDEPATH = /usr/include

all : reflet-masm.bin libreflet-masm.so

libreflet-masm.so : libreflet-masm.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc -shared libreflet-masm.o $(REFLETPASM) -o libreflet-masm.so

reflet-masm.bin : macro_assembler.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc macro_assembler.o $(LUA) $(REFLETPASM) -o reflet-masm.bin

macro_assembler.o : macro_assembler.c
	gcc -c macro_assembler.c $(FLAGS) -o macro_assembler.o

libreflet-masm.o : libreflet-masm.c libreflet-masm.h
	gcc -c -fPIC libreflet-masm.c $(FLAGS) -o libreflet-masm.o

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
	mkdir -p /usr/local/share/reflet
	cp -f reflet-masm.bin /usr/local/bin/reflet-masm
	cp -f lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac /usr/local/share/reflet/

install_lib :
	mkdir -p $(LIBPATH) $(INCLUDEPATH)
	cp -f libreflet-masm.h $(INCLUDEPATH)/reflet-masm.h
	cp -f libreflet-masm.so $(LIBPATH)/libreflet-masm.so

uninstall : uninstall_macro uninstall_lib

uninstall_macro :
	rm -f /usr/local/bin/reflet-masm
	rm -f /usr/local/share/reflet/lib.luac /usr/local/share/reflet/link.luac /usr/local/share/reflet/macro.luac /usr/local/share/reflet/readingCode.luac /usr/local/share/reflet/macro_assembler.luac 

uninstall_lib :
	rm -f $(INCLUDEPATH)/reflet-masm.h
	rm -f $(LIBPATH)/libreflet-masm.so

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.so
	rm -f *.luac

