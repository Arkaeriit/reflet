FLAGS = -Wall -Werror -g
LUA = -llua -lm -ldl   
REFLETPASM = -lreflet-pasm
LIBPATH = /usr/lib64
INCLUDEPATH = /usr/include

all : reflet-masp.bin libreflet-masp.so

libreflet-masp.so : libreflet-masp.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc -shared libreflet-masp.o $(REFLETPASM) -o libreflet-masp.so

reflet-masp.bin : macro_assembler.o lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac
	gcc macro_assembler.o $(LUA) $(REFLETPASM) -o reflet-masp.bin

macro_assembler.o : macro_assembler.c
	gcc -c macro_assembler.c $(FLAGS) -o macro_assembler.o

libreflet-masp.o : libreflet-masp.c libreflet-masp.h
	gcc -c -fPIC libreflet-masp.c $(FLAGS) -o libreflet-masp.o

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
	cp -f reflet-masp.bin /usr/local/bin/reflet-masp
	cp -f lib.luac link.luac macro.luac readingCode.luac macro_assembler.luac /usr/local/share/reflet/

install_lib :
	mkdir -p $(LIBPATH) $(INCLUDEPATH)
	cp -f libreflet-masp.h $(INCLUDEPATH)/reflet-masp.h
	cp -f libreflet-masp.so $(LIBPATH)/libreflet-masp.so

uninstall : uninstall_macro uninstall_lib

uninstall_macro :
	rm -f /usr/local/bin/reflet-masp
	rm -f /usr/local/share/reflet/lib.luac /usr/local/share/reflet/link.luac /usr/local/share/reflet/macro.luac /usr/local/share/reflet/readingCode.luac /usr/local/share/reflet/macro_assembler.luac 

uninstall_lib :
	rm -f $(INCLUDEPATH)/reflet-masp.h
	rm -f $(LIBPATH)/libreflet-masp.so

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.so
	rm -f *.luac

