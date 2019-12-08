/*-------------------------------------------------------------\
|This file stores constants that are ment to be used in all the|
|programm.                                                     |
\-------------------------------------------------------------*/

#ifndef CONSTANT
#define CONSTANT

#include <inttypes.h>

//Opperand for the 64 bits mode
#include "opperand64.h"


//Magic words
#define BINARY_MW_SIZE 4
#define BINARY_MW 0x4D565341 /*ASVM*/
typedef uint32_t BINARY_MW_TYPE;
#define BINARY_32_MW 0x32335341 /*AS32*/
#define OBJECT_MW_SIZE 4
#define OBJECT_MW 0x424F5341 /*ASOB*/
typedef uint32_t OBJECT_MW_TYPE;


//Error codes
//Compiler
#define COMPILED_LINE_NOT_OK 0
#define COMPILED_LINE_INSTRUCTION 1
#define COMPILED_LINE_BRANCH 2
#define COMPILED_LINE_DATA 3
#define COMPILE_NOPE 1
#define COMPILE_R 2
#define COMPILE_RR 3
#define COMPILE_RN 4
#define COMPILE_RRR 5
#define COMPILATION_ERROR 1
#define COMPILATION_OK 0
//Interpreter
#define EXECUTE_OK 0
#define EXECUTE_UNKNOWN_OPPERAND 1
#define EXECUTE_INVALID_FILE 2
#define EXECUTE_ERROR_VM 3
//binary file reader
#define READ_OK 0
#define READ_NOT_OK 1
//linker
#define LINK_INVALID_FILE 1
#define LINK_INVALID_LABEL 2
#define LINK_OK 0
#define LINK_ERROR_DOUBLE_DEFINITION 1
#define LABEL_OK 0
#define LINK_UNDEFINED_LABEL 1


//Compilation constants
#define NUM1_MASK 0xFFFFFFFFFFFF /*assert the max size of a number*/
#define REG_MASK 0x3F /*assert the max size of a register*/
#define MAX_SIZE_LABEL 150 /*Max number of char in a label name*/
#define MAX_CALL_NUMBER 1000 /*Maximun number of position from where a label can be called*/
#define SIZELINE_LABEL MAX_SIZE_LABEL + 3 /*Size of a line of object code used for label*/ /*It is the longest name a label can habe + a indicating byte + 2 opperand bytes*/
#define SIZELINE SIZELINE_LABEL + 1024 /*Max size for lablel, data or code*/


//Execution constants
#define OPP 0x3FF /*Mask to get the opperand*/
#define REG1 0xFC00 /*Mask to get the 1st register*/
#define REG2 0x3F0000 /*Mask to get the 2nd register*/
#define REG3 0xFC00000 /*Mask to get the 3rd register*/
#define NUM0 0xFFFFFFFFFFFFFC00/*Mask to get the numeric value if there is 0*/
#define NUM1 0xFFFFFFFFFFFF0000/*Mask to get the numeric value if there is only 1 register*/


//Compilation and execution constants
#define ARG_SHIFT_1 10
#define ARG_SHIFT_2 16
#define ARG_SHIFT_3 22

//Flags constants
#define FLAGS_NUMBER 3
#define FLAG_ZERO 0
#define FLAG_BIGGER 1
#define FLAG_SMALLER_OR_EQUAL 2

//misc constants
#define REGISTERS_NUMBER 64

#endif

