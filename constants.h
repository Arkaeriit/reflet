/*-------------------------------------------------------------\
|This file stores constants that are ment to be used in all the|
|programm.                                                     |
\-------------------------------------------------------------*/

#ifndef CONSTANT
#define CONSTANT

//Opperand for the 64 bits mode
#include "opperand64.h"


//Magic words
#define BINARY_MW_SIZE 4
#define BINARY_MW 0x4D565341 /*ASVM*/
#define BINARY_32_MW 0x32335341 /*AS32*/
#define OBJECT_MW_SIZE 4
#define OBJECT_MW 0x424F5341 /*ASOB*/


//Error codes
//Compiler
#define COMPILED_LINE_NOT_OK 0
#define COMPILED_LINE_INSTRUCTION 1
#define COMPILED_LINE_BRANCH 2
//Interpreter
#define EXECUTE_OK 0
#define EXECUTE_UNKNOWN_OPPERAND 1
//binary file reader
#define READ_OK 0
#define READ_NOT_OK 1


//Compilation constants
#define SIZELINE 255 /*Size of a line of object code that will be read*/

#endif

