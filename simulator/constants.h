/*---------------------------------------------------------\
|This file provide various canstants that could be modified|
|to specify the behavious of the simulator.                |
\---------------------------------------------------------*/

#ifndef CONSTANTS_SIMU
#define CONSTANTS_SIMU

#include <inttypes.h>

//Default set-up config, can be changed
#define WORD_SIZE 8 /*Actual number of bits in a processor register*/
#define WORD_SIZE_BYTE 1 /*Number of bytes in a word*/
#define WORD_MASK 0xFF /*MASK used to ensure the use of the correct number of bits*/
typedef uint8_t word_t; //Type used in the simulator
#define WORD_P PRIu64
#define RAM_SIZE 255
typedef uint8_t ram_word_t;

//Mgic word settings, unchangeable
#define NO_MAGIC_WORD_CHECKING 0
#define MAGIC_WORD_WARNING 1
#define MAGIC_WORD_ERROR 2
#define MAGIC_WORD_CHECKING MAGIC_WORD_ERROR
#define MAGIC_WORD_SIZE 4
#define MAGIC_WORD "ASRM"

//Magic numbers
#define START_CHAR 4
#define NUMBER_OF_REGISTERS 16
#define LINE_BUFF 10000 /*Max char in a line of config file*/

//Return value
#define RET_INVALID_ARGS 1
#define RET_NO_VM 2
#define RET_UNOPEN_FILE 3
#define RET_CONFIG 5

#endif

