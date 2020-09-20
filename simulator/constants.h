/*---------------------------------------------------------\
|This file provide various canstants that could be modified|
|to specify the behavious of the simulator.                |
\---------------------------------------------------------*/

#ifndef CONSTANTS_SIMU
#define CONSTANTS_SIMU

#include <inttypes.h>
#include <stdbool.h>

#define WORD_SIZE 8
typedef uint8_t word_t;
#define RAM_SIZE 256
#define RAM_WORD_SIZE 8
typedef uint8_t ram_word_t;

#define MAGIC_WORD_CHEKING true
#define MAGIC_WORD_SIZE 4
#define MAGIC_WORD "ASRM"

#define START_CHAR 4
#define NUMBER_OF_REGISTERS 16

#endif

