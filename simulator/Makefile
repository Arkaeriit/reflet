FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h constants.h

asrm.o : asrm.c asrm.h $(CONSTANTS)
	gcc -c asrm.c $(FLAGS) -o asrm.o

loader.o : loader.h loader.c $(CONSTANTS)
	gcc -c loader.c $(FLAGS) -o loader.o

clean:
	rm -f *.o
	rm *.bin

