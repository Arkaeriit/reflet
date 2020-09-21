/*---------------------------------------------------------\
|This file provide various canstants that could be modified|
|to specify the behavious of the simulator.                |
\---------------------------------------------------------*/

#ifndef CONSTANTS_SIMU
#define CONSTANTS_SIMU

#include <inttypes.h>

#define WORD_SIZE 8
typedef uint8_t word_t;
#define RAM_SIZE 255
#define RAM_WORD_SIZE 8
typedef uint8_t ram_word_t;
#define RAM_SIZE_BYTE (sizeof(ram_word_t) * RAM_SIZE)

#define NO_MAGIC_WORD_CHECKING 0
#define MAGIC_WORD_WARNING 1
#define MAGIC_WORD_ERROR 2
#define MAGIC_WORD_CHECKING MAGIC_WORD_ERROR
#define MAGIC_WORD_SIZE 4
#define MAGIC_WORD "ASRM"

#define START_CHAR 4
#define NUMBER_OF_REGISTERS 16

#define RET_INVALID_ARGS 1
#define RET_NO_VM 2

#endif

