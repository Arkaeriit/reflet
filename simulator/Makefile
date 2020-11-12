#FLAGS = -Wall -Werror -g
FLAGS = -Wall -g
CONSTANTS = opperand.h constants.h

reflet-sim.bin : main.o reflet.o loader.o config.o debug.o
	gcc reflet.o main.o loader.o config.o debug.o -o reflet-sim.bin

main.o : main.c
	gcc -c main.c $(FLAGS) -o main.o

reflet.o : reflet.c reflet.h $(CONSTANTS)
	gcc -c reflet.c $(FLAGS) -o reflet.o

loader.o : loader.h loader.c $(CONSTANTS)
	gcc -c loader.c $(FLAGS) -o loader.o

config.o : config.h config.c $(CONSTANTS)
	gcc -c config.c $(FLAGS) -o config.o	

debug.o : debug.h debug.c $(CONSTANTS)
	gcc -c debug.c $(FLAGS) -o debug.o	

install :
	cp -f reflet-sim.bin /usr/local/bin/reflet-sim

uninstall :
	rm -f /usr/local/bin/reflet-sim

clean:
	rm -f *.o
	rm -f *.bin

