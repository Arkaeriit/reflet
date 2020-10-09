#FLAGS = -Wall -Werror -g
FLAGS = -Wall -g
CONSTANTS = opperand.h constants.h

asrm-sim.bin : main.o asrm.o loader.o config.o debug.o
	gcc asrm.o main.o loader.o config.o debug.o -o asrm-sim.bin

main.o : main.c
	gcc -c main.c $(FLAGS) -o main.o

asrm.o : asrm.c asrm.h $(CONSTANTS)
	gcc -c asrm.c $(FLAGS) -o asrm.o

loader.o : loader.h loader.c $(CONSTANTS)
	gcc -c loader.c $(FLAGS) -o loader.o

config.o : config.h config.c $(CONSTANTS)
	gcc -c config.c $(FLAGS) -o config.o	

debug.o : debug.h debug.c $(CONSTANTS)
	gcc -c debug.c $(FLAGS) -o debug.o	

install :
	cp -f asrm-sim.bin /usr/local/bin/asrm-sim

uninstall :
	rm -f /usr/local/bin/asrm-sim

clean:
	rm -f *.o
	rm *.bin

