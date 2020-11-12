FLAGS = -Wall -Werror -g
LIBPATH = /usr/lib64
INCLUDEPATH = /usr/include

all : reflet-pasm.bin libreflet-pasm.so

libreflet-pasm.so : pico_assembler_shared.o
	gcc -shared pico_assembler.o -o libreflet-pasm.so

pico_assembler_shared.o : pico_assembler.h pico_assembler.c
	gcc -c -fPIC pico_assembler.c $(FLAGS) -o pico_assembler.o

reflet-pasm.bin : pico_assembler_main.o pico_assembler.o
	gcc pico_assembler.o pico_assembler_main.o -o reflet-pasm.bin

pico_assembler.o : pico_assembler.h pico_assembler.c $(CONSTANTS)
	gcc -c pico_assembler.c $(FLAGS) -o pico_assembler.o

pico_assembler_main.o : pico_assembler_main.c
	gcc -c pico_assembler_main.c $(FLAGS) -o pico_assembler_main.o

install : install_pico install_lib

install_pico :
	cp -f reflet-pasm.bin /usr/local/bin/reflet-pasm

install_lib :
	mkdir -p $(LIBPATH) $(INCLUDEPATH)
	cp -f pico_assembler.h $(INCLUDEPATH)/reflet-pasm.h
	cp -f libreflet-pasm.so $(LIBPATH)/libreflet-pasm.so

uninstall : uninstall_lib uninstall_pico

uninstall_pico : 
	rm -f /usr/local/bin/reflet-pasm

uninstall_lib :
	rm -f $(INCLUDEPATH)/reflet-pasm.h
	rm -f $(LIBPATH)/libreflet-pasm.so

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.bin

