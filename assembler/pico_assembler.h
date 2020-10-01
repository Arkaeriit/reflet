/*--------------------------------------\
|This file contain most of the functions|
|used by the basic assembler.           |
\--------------------------------------*/

#ifndef MINI_ASM
#define MINI_ASM

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "opperand.h"

void mini_assembleFile(const char* fileIn, const char* fileOut);

#define RET_INVALID_ARGS 1
#define RET_UNOPEN_FILE 3
#define LINE_BUFF 10000

#endif

