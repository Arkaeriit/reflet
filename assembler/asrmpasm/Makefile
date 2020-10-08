FLAGS = -Wall -Werror -g
LIBPATH = /usr/lib64
INCLUDEPATH = /usr/include

all : asrmpasm.bin libasrmpasm.so

libasrmpasm.so : pico_assembler_shared.o
	gcc -shared pico_assembler.o -o libasrmpasm.so

pico_assembler_shared.o : pico_assembler.h pico_assembler.c
	gcc -c -fPIC pico_assembler.c $(FLAGS) -o pico_assembler.o

asrmpasm.bin : pico_assembler_main.o pico_assembler.o
	gcc pico_assembler.o pico_assembler_main.o -o asrmpasm.bin

pico_assembler.o : pico_assembler.h pico_assembler.c $(CONSTANTS)
	gcc -c pico_assembler.c $(FLAGS) -o pico_assembler.o

pico_assembler_main.o : pico_assembler_main.c
	gcc -c pico_assembler_main.c $(FLAGS) -o pico_assembler_main.o

install : install_pico install_lib

install_pico :
	cp -f asrmpasm.bin /usr/local/bin/asrmpasm

install_lib :
	mkdir -p $(LIBPATH) $(INCLUDEPATH)
	cp -f pico_assembler.h $(INCLUDEPATH)/asrmpasm.h
	cp -f libasrmpasm.so $(LIBPATH)/libasrmpasm.so

uninstall : uninstall_lib uninstall_lib

uninstall_pico : 
	rm -f /usr/local/bin/asrmpasm

uninstall_lib :
	rm -f $(INCLUDEPATH)/asrmpasm.h
	rm -f $(LIBPATH)/libasrmpasm.so

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.bin

