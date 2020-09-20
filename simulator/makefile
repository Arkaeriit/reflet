FLAGS = -Wall -Werror -g
CONSTANTS = opperand.h constants.h

asrm.o : asrm.c asrm.h $(CONSTANTS)
	gcc -c asrm.c $(FLAGS) -o asrm.o

clean:
	rm -f *.o
	rm *.bin

